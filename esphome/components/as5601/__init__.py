import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import i2c
from esphome.const import CONF_ID


CONF_AB_POSITIONS = "ab_positions"
CONF_ORIENTATION_OFFSET = "orientation_offset"

CONF_MAGNET_PRESENCE = "presence"
CONF_MGF_MAGNITUDE = "magnitude"
CONF_MGF_ORIENTATION = "orientation"


AUTO_LOAD = ["sensor", "binary_sensor"]
DEPENDENCIES = ["i2c"]
MULTI_CONF = True


CONF_AS5601_ID = "as5601_id"


as5601_ns = cg.esphome_ns.namespace("as5601")
AS5601 = as5601_ns.class_("AS5601", cg.Component, i2c.I2CDevice)


CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(AS5601),
            cv.Optional(CONF_AB_POSITIONS, default=8): cv.templatable(
                cv.one_of(2048, 1024, 512, 256, 128, 64, 32, 16, 8)
            ),
            cv.Optional(CONF_ORIENTATION_OFFSET, default=0): cv.int_range(
                min=0, max=4096, min_included=True, max_included=False
            ),
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
    .extend(i2c.i2c_device_schema(0x36))
)


def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    yield cg.register_component(var, config)
    yield i2c.register_i2c_device(var, config)

    cg.add(var.init_ab_positions(config[CONF_AB_POSITIONS]))
    cg.add(var.init_zero_position(config[CONF_ORIENTATION_OFFSET]))


""" Example configuration
i2c:
  sda: GPIO21
  scl: GPIO22


as5601: # supports multiconf
  ab_positions: 1024
  orientation_offset: 0


sensor:
  - platform: as5601
    orientation:
      name: AS5601 Magnet Angle
     filters:
       - lambda: return x / 4096 * 360;
    magnitude:
      name: AS5601 Magnet Field Strength


binary_sensor:
  - platform: as5601
    id: ${hostid}_magnet_detected
    name: ${hostname} Magnet detected
"""
