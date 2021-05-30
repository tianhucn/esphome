import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import i2c
from esphome.const import CONF_ID, CONF_VARIANT

AUTO_LOAD = ["sensor", "binary_sensor"]
DEPENDENCIES = ["i2c"]
MULTI_CONF = True


CONF_AS560X_ID = "as560x_id"

CONF_MAGNET_PRESENCE = "presence"
CONF_MGF_MAGNITUDE = "magnitude"
CONF_MGF_ORIENTATION = "orientation"
CONF_ORIENTATION_OFFSET = "orientation_offset"

# AS5600 only
CONF_AS5600 = "as5600"
CONF_MPOS = "end_position_offset"  # TODO: better naming
CONF_MANG = "as5601_mang"  # TODO: better naming

# AS5601 only
CONF_AS5601 = "as5601"
CONF_AB_POSITIONS = "ab_positions"
CONF_PUSH_THRESHOLD = "push_threshold"


as560x_ns = cg.esphome_ns.namespace("as560x")
AS560X = as560x_ns.class_("AS560X", cg.Component, i2c.I2CDevice)


def validate_config(config):
    variant = config[CONF_VARIANT].lower()

    # Validate AS5600 config
    if variant == CONF_AS5600.lower():
        for conf in [CONF_AB_POSITIONS, CONF_PUSH_THRESHOLD]:
            if conf in config:
                raise cv.Invalid(f"Cannot specify '{conf}' for {variant}")

    # Validate AS5601 config
    if variant == CONF_AS5601.lower():
        for conf in [CONF_MPOS, CONF_MANG]:
            if conf in config:
                raise cv.Invalid(f"Cannot specify '{conf}' for {variant}")

    return config


CONFIG_SCHEMA = cv.All(
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(AS560X),
            cv.Required(CONF_VARIANT): cv.one_of(CONF_AS5600, CONF_AS5601, lower=True),
            cv.Optional(CONF_ORIENTATION_OFFSET, default=0): cv.int_range(
                min=0, max=4096, min_included=True, max_included=False
            ),
            # AS5600 only
            cv.Optional(CONF_MPOS): cv.int_,
            cv.Optional(CONF_MANG): cv.int_,
            # AS5601 only
            cv.Optional(CONF_AB_POSITIONS, default=8): cv.one_of(
                *[2 ** i for i in range(3, 12)]
            ),  # 8,16,...,1024,2048
            cv.Optional(CONF_PUSH_THRESHOLD, default=0): cv.int_,
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
    .extend(i2c.i2c_device_schema(0x36)),
    validate_config,
)


def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    yield cg.register_component(var, config)
    yield i2c.register_i2c_device(var, config)

    variant = config[CONF_VARIANT].lower()
    if variant == CONF_AS5600.lower():
        cg.add(var.set_end_position(config[CONF_MPOS]))
        cg.add(var.set_mang(config[CONF_MANG]))

    if variant == CONF_AS5601.lower():
        cg.add(var.set_ab_positions(config[CONF_AB_POSITIONS]))
        # cg.add(var.set_push_threshold(config[CONF_PUSH_THRESHOLD]))

    cg.add(var.set_zero_position(config[CONF_ORIENTATION_OFFSET]))


""" Example configuration
i2c:
  - id: as5600_i2c_bus
    sda: GPIO21
    scl: GPIO22

  - id: as5601_i2c_bus
    sda: GPIO23
    scl: GPIO24


as560x:
  - id: main_as5600
    variant: as5600
    i2c_bus: as5600_i2c_bus

  - id: main_as5601
    variant: as5601
    ab_positions: 1024
    orientation_offset: 0
    i2c_bus: as5601_i2c_bus


sensor:
  - platform: as560x
    as560x_id: main_as5601
    orientation:
        name: AS560X Magnet Angle
    filters:
      - lambda: return x / 4096 * 360;
    magnitude:
        name: AS560X Magnet Field Strength


binary_sensor:
  - platform: as560x
    as560x_id: main_as5601
    id: as560x_magnet_detected
    name: AS560X Magnet detected
"""
