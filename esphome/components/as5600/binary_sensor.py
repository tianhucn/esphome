import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import binary_sensor
from . import AS5600, CONF_AS5600_ID

DEPENDENCIES = ["as5600"]


CONFIG_SCHEMA = binary_sensor.BINARY_SENSOR_SCHEMA.extend(
    {
        cv.GenerateID(CONF_AS5600_ID): cv.use_id(AS5600),
    }
)


def to_code(config):
    hub = yield cg.get_variable(config[CONF_AS5600_ID])
    var = yield binary_sensor.new_binary_sensor(config)
    cg.add(hub.init_presence_sensor_(var))
