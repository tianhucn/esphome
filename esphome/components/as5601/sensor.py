import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import UNIT_EMPTY, ICON_EMPTY

from . import AS5601, CONF_AS5601_ID, CONF_MGF_MAGNITUDE, CONF_MGF_ORIENTATION

DEPENDENCIES = ["as5601"]


CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_AS5601_ID): cv.use_id(AS5601),
        cv.Optional(CONF_MGF_MAGNITUDE): sensor.sensor_schema(
            UNIT_EMPTY, ICON_EMPTY, 0
        ).extend(),
        cv.Optional(CONF_MGF_ORIENTATION): sensor.sensor_schema(
            UNIT_EMPTY, ICON_EMPTY, 0
        ).extend(),
    }
).extend(cv.COMPONENT_SCHEMA)


def to_code(config):
    hub = yield cg.get_variable(config[CONF_AS5601_ID])

    if CONF_MGF_MAGNITUDE in config:
        sens = yield sensor.new_sensor(config[CONF_MGF_MAGNITUDE])
        cg.add(hub.init_magnitude_sensor_(sens))

    if CONF_MGF_ORIENTATION in config:
        sens = yield sensor.new_sensor(config[CONF_MGF_ORIENTATION])
        cg.add(hub.init_orientation_sensor_(sens))
