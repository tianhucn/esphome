import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import (
    CONF_MAGNITUDE,
    CONF_ANGLE,
    UNIT_EMPTY,
    ICON_EMPTY,
    ICON_AXIS_X_ROTATE_COUNTERCLOCKWISE,
)
from . import AS560XComponent, CONF_AS560X_ID


DEPENDENCIES = ["as560x"]

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_AS560X_ID): cv.use_id(AS560XComponent),
        cv.Optional(CONF_MAGNITUDE): sensor.sensor_schema(
            UNIT_EMPTY, ICON_EMPTY, 0
        ).extend(),
        cv.Optional(CONF_ANGLE): sensor.sensor_schema(
            UNIT_EMPTY, ICON_AXIS_X_ROTATE_COUNTERCLOCKWISE, 0
        ).extend(),
    }
).extend(cv.COMPONENT_SCHEMA)


async def to_code(config):
    hub = await cg.get_variable(config[CONF_AS560X_ID])

    if CONF_MAGNITUDE in config:
        sens = await sensor.new_sensor(config[CONF_MAGNITUDE])
        cg.add(hub.set_magnitude_sensor(sens))

    if CONF_ANGLE in config:
        sens = await sensor.new_sensor(config[CONF_ANGLE])
        cg.add(hub.set_angle_sensor(sens))
