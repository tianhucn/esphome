import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import binary_sensor

from . import AS560XComponent, CONF_AS560X_ID

DEPENDENCIES = ["as560x"]


CONFIG_SCHEMA = binary_sensor.BINARY_SENSOR_SCHEMA.extend(
    {
        cv.GenerateID(CONF_AS560X_ID): cv.use_id(AS560XComponent),
    }
)


def to_code(config):
    hub = yield cg.get_variable(config[CONF_AS560X_ID])
    var = yield binary_sensor.new_binary_sensor(config)
    cg.add(hub.set_presence_sensor_(var))
