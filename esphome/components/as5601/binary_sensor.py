import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import binary_sensor
from . import AS5601, CONF_AS5601_ID

DEPENDENCIES = ["as5601"]


CONFIG_SCHEMA = binary_sensor.BINARY_SENSOR_SCHEMA.extend(
    {
        cv.GenerateID(CONF_AS5601_ID): cv.use_id(AS5601),
    }
)


def to_code(config):
    hub = yield cg.get_variable(config[CONF_AS5601_ID])
    var = yield binary_sensor.new_binary_sensor(config)
    cg.add(hub.init_presence_sensor_(var))
