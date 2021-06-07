import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import i2c
from esphome.const import CONF_ID, CONF_VARIANT

CODEOWNERS = ["@slimcdk"]

AUTO_LOAD = ["sensor", "binary_sensor"]
DEPENDENCIES = ["i2c"]
MULTI_CONF = True


CONF_AS560X = "as560x"
CONF_AS5600 = "as5600"
CONF_AS5601 = "as5601"
CONF_AS560X_ID = f"{CONF_AS560X}_{CONF_ID}"


CONF_AB_RESOLUTION = "ab_resolution"
CONF_PUSH_THRESHOLD = "push_threshold"
CONF_ANGLE_ZERO_POSITION = "zero_angle_position"
CONF_ANGLE_STOP_POSITION = "angle_stop_position"
CONF_MAXIMUM_ANGLE = "maximum_angle"


as560x_ns = cg.esphome_ns.namespace("as560x")
AS560XComponent = as560x_ns.class_("AS560XComponent", cg.Component, i2c.I2CDevice)

VARIANT_COMP = {
    CONF_AS5600: as560x_ns.class_("AS5600Component", AS560XComponent),
    CONF_AS5601: as560x_ns.class_("AS5601Component", AS560XComponent),
}


def set_variant_comp(config):
    # Overwrite ID with appropriate as560x variant component
    config[CONF_ID] = cv.declare_id(VARIANT_COMP[config[CONF_VARIANT]])(
        config[CONF_ID].id
    )
    return config


def validate_config_and_set_defaults(config):
    # Keys will exist in the config dictonary and fail the validation if defaults are set in the CONFIG_SCHEMA instead of there

    # Validate AS5600
    if config[CONF_VARIANT] == CONF_AS5600:
        for conf in [CONF_AB_RESOLUTION, CONF_PUSH_THRESHOLD]:
            if conf in config:
                raise cv.Invalid(f"{conf} must not be specified with {CONF_AS5600}")
        if CONF_ANGLE_STOP_POSITION not in config:
            config[CONF_ANGLE_STOP_POSITION] = 4095
        if CONF_MAXIMUM_ANGLE not in config:
            config[CONF_MAXIMUM_ANGLE] = 0

    # Validate AS5601
    if config[CONF_VARIANT] == CONF_AS5601:
        for conf in [CONF_ANGLE_STOP_POSITION, CONF_MAXIMUM_ANGLE]:
            if conf in config:
                raise cv.Invalid(f"{conf} must not be specified with {CONF_AS5600}")
        if CONF_AB_RESOLUTION not in config:
            config[CONF_AB_RESOLUTION] = 8
        if CONF_PUSH_THRESHOLD not in config:
            config[CONF_PUSH_THRESHOLD] = 0

    return config


CONFIG_SCHEMA = cv.All(
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(AS560XComponent),
            cv.Required(CONF_VARIANT): cv.one_of(CONF_AS5600, CONF_AS5601, lower=True),
            cv.Optional(CONF_ANGLE_ZERO_POSITION, default=0): cv.int_range(
                min=0, max=2 ** 12 - 1, min_included=True, max_included=False
            ),
            # AS5600
            cv.Optional(CONF_ANGLE_STOP_POSITION): cv.int_,
            cv.Optional(CONF_MAXIMUM_ANGLE): cv.int_,
            # AS5601
            cv.Optional(CONF_AB_RESOLUTION): cv.one_of(
                *[2 ** i for i in range(3, 12)]
            ),  # 8,16,...,1024,2048
            cv.Optional(CONF_PUSH_THRESHOLD): cv.int_,
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
    .extend(i2c.i2c_device_schema(0x36)),
    set_variant_comp,
    validate_config_and_set_defaults,
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)

    cg.add(var.set_zero_position(config[CONF_ANGLE_ZERO_POSITION]))

    if config[CONF_VARIANT] == CONF_AS5600:
        cg.add(var.set_stop_position(config[CONF_ANGLE_STOP_POSITION]))
        cg.add(var.set_maximum_angle(config[CONF_MAXIMUM_ANGLE]))

    if config[CONF_VARIANT] == CONF_AS5601:
        cg.add(var.set_ab_resolution(config[CONF_AB_RESOLUTION]))
        cg.add(var.set_push_threshold(config[CONF_PUSH_THRESHOLD]))


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
    i2c_id: as5600_i2c_bus

  - id: main_as5601
    variant: as5601
    ab_resolution: 1024
    zero_angle_position: 0
    i2c_id: as5601_i2c_bus


sensor:
  - platform: as560x
    as5600_id: main_as5600
    orientation:
        name: AS5600 Magnet Angle
        id: as5600_magnet_angle
        filters:
          - lambda: return x / 4096 * 360;
    magnitude:
        id: as5600_magnet_field_strength
        name: AS5600 Magnet Field Strength

  - platform: as560x
    as5601_id: main_as5601
    orientation:
        name: AS5601 Magnet Angle
        id: as5601_magnet_angle
        filters:
          - lambda: return x / 4096 * 360;
    magnitude:
        id: as5601_magnet_field_strength
        name: AS5601 Magnet Field Strength


binary_sensor:
  - platform: as560x
    as5600_id: main_as5600
    id: as5600_magnet_detected
    name: AS5600 Magnet detected

  - platform: as560x
    as5601_id: main_as5601
    id: as5601_magnet_detected
    name: AS5601 Magnet detected
"""
