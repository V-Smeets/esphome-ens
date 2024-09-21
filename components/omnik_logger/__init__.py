import esphome.codegen as cg
import esphome.config_validation as cv
import esphome.components.sensor as s
import esphome.components.text_sensor as ts
from esphome.const import (
    CONF_ENTITY_CATEGORY,
    CONF_ID,
    CONF_IP_ADDRESS,
    CONF_NAME,
    CONF_STATE_CLASS,
    ENTITY_CATEGORY_DIAGNOSTIC,
    STATE_CLASS_MEASUREMENT,
)
from ..omnik_base import (
    to_code_base,
    OmnikBase,
    CONFIG_SCHEMA_BASE,
)

AUTO_LOAD = [
    "omnik_base",
    "sensor",
    "text_sensor",
]

omnik_logger_ns = cg.esphome_ns.namespace("omnik_logger")
OmnikLogger = omnik_logger_ns.class_(
    "OmnikLogger",
    OmnikBase,
    cg.Component,
)

CONF_CONNECTION_NUMBER = "connection_number"
CONF_SERIAL_DEVICE_NUMBER = "serial_device_number"

CONFIG_SCHEMA = CONFIG_SCHEMA_BASE.extend({
    cv.GenerateID(): cv.declare_id(OmnikLogger),
    cv.Optional(CONF_CONNECTION_NUMBER,
                default={
                    CONF_NAME: "Inverter Connection number",
                    CONF_ENTITY_CATEGORY: ENTITY_CATEGORY_DIAGNOSTIC,
                }): ts.text_sensor_schema(),
    cv.Optional(CONF_IP_ADDRESS,
                default={
                    CONF_NAME: "Logger IP address",
                    CONF_ENTITY_CATEGORY: ENTITY_CATEGORY_DIAGNOSTIC,
                }): ts.text_sensor_schema(),
    cv.Optional(CONF_SERIAL_DEVICE_NUMBER,
                default={
                    CONF_NAME: "Logger Serial device number",
                    CONF_ENTITY_CATEGORY: ENTITY_CATEGORY_DIAGNOSTIC,
                }): ts.text_sensor_schema(),
})

async def to_code(config):
    # breakpoint()
    comp = await to_code_base(config)

    for sensor_key in config:
        sensor_config = config[sensor_key]
        if not isinstance(sensor_config, dict):
            continue
        sensor_id = sensor_config[CONF_ID]
        sensor_type = sensor_id.type
        match sensor_type.base:
            case s.Sensor.base:
                sensor = await s.new_sensor(sensor_config)
                cg.add(getattr(comp, f"set_{sensor_key}_sensor")(sensor))
            case ts.TextSensor.base:
                sensor = await ts.new_text_sensor(sensor_config)
                cg.add(getattr(comp, f"set_{sensor_key}_text_sensor")(sensor))

# vim:sw=4:
