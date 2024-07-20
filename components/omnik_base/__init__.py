import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.const import (
    CONF_ID,
    CONF_UART_ID,
)
from esphome.components import (
    uart,
)

omnik_base = cg.esphome_ns.namespace("omnik_base")
OmnikBase = omnik_base.class_(
    "OmnikBase",
    cg.Component,
)

CONFIG_SCHEMA_BASE = (
    cv.COMPONENT_SCHEMA
    .extend(uart.UART_DEVICE_SCHEMA)
)

async def to_code_base(config):
    comp = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(comp, config)
    await uart.register_uart_device(comp, config)
    return comp
