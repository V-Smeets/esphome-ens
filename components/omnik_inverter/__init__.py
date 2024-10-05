import esphome.codegen as cg
import esphome.config_validation as cv
import esphome.components.sensor as s
import esphome.components.text_sensor as ts
from esphome.const import (
    CONF_ACCURACY_DECIMALS,
    CONF_DEVICE_CLASS,
    CONF_ENTITY_CATEGORY,
    CONF_ID,
    CONF_INTERNAL,
    CONF_NAME,
    CONF_STATE_CLASS,
    CONF_TEMPERATURE,
    CONF_UNIT_OF_MEASUREMENT,
    DEVICE_CLASS_CONDUCTIVITY,
    DEVICE_CLASS_CURRENT,
    DEVICE_CLASS_EMPTY,
    DEVICE_CLASS_ENERGY,
    DEVICE_CLASS_FREQUENCY,
    DEVICE_CLASS_POWER,
    DEVICE_CLASS_TEMPERATURE,
    DEVICE_CLASS_VOLTAGE,
    ENTITY_CATEGORY_DIAGNOSTIC,
    ENTITY_CATEGORY_NONE,
    STATE_CLASS_MEASUREMENT,
    STATE_CLASS_TOTAL,
    STATE_CLASS_TOTAL_INCREASING,
    UNIT_AMPERE,
    UNIT_CELSIUS,
    UNIT_DEGREES,
    UNIT_EMPTY,
    UNIT_HERTZ,
    UNIT_HOUR,
    UNIT_KILOWATT,
    UNIT_KILOWATT_HOURS,
    UNIT_VOLT,
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

omnik_inverter_ns = cg.esphome_ns.namespace("omnik_inverter")
OmnikInverter = omnik_inverter_ns.class_(
    "OmnikInverter",
    OmnikBase,
    cg.Component,
)

CONF_BRAND = "brand"
CONF_COUNTRY = "country"
CONF_ENERGY_TODAY = "energy_today"
CONF_ENERGY_TOTAL = "energy_total"
CONF_ERROR_MESSAGE_BINARY_INDEX = "error_message_binary_index"
CONF_FIRMWARE_VERSION_MAIN = "firmware_version_main"
CONF_FIRMWARE_VERSION_SLAVE = "firmware_version_slave"
CONF_GFCI_CURRENT_FAULT = "gfci_current_fault"
CONF_GRID_FREQUENCY_FAULT_VALUE = "grid_frequency_fault_value"
CONF_GRID_IMPEDANCE_FAULT_VALUE = "grid_impedance_fault_value"
CONF_GRID_VOLTAGE_FAULT_VALUE = "grid_voltage_fault_value"
CONF_HOURS_TOTAL = "hours_total"
CONF_INVERTER_MODEL = "inverter_model"
CONF_MESSAGE_11_83_BYTES_60_77 = "message_11_83_bytes_60_77"
CONF_NR_OF_ALARMS = "nr_of_alarms"
CONF_NR_OF_PHASES = "nr_of_phases"
CONF_PV1_CURRENT = "pv1_current"
CONF_PV1_VOLTAGE = "pv1_voltage"
CONF_PV2_CURRENT = "pv2_current"
CONF_PV2_VOLTAGE = "pv2_voltage"
CONF_PV3_CURRENT = "pv3_current"
CONF_PV3_VOLTAGE = "pv3_voltage"
CONF_PV_VOLTAGE_FAULT = "pv_voltage_fault"
CONF_RATED_POWER = "rated_power"
CONF_R_CURRENT = "r_current"
CONF_R_FREQUENCY = "r_frequency"
CONF_R_POWER = "r_power"
CONF_RUN_STATE = "run_state"
CONF_R_VOLTAGE = "r_voltage"
CONF_S_CURRENT = "s_current"
CONF_SERIAL_DEVICE_NUMBER = "serial_device_number"
CONF_S_FREQUENCY = "s_frequency"
CONF_S_POWER = "s_power"
CONF_STATUS_10_81 = "status_10_81"
CONF_STATUS_10_84 = "status_10_84"
CONF_STATUS_12_C0 = "status_12_c0"
CONF_STATUS_12_C1 = "status_12_c1"
CONF_S_VOLTAGE = "s_voltage"
CONF_T_CURRENT = "t_current"
CONF_TEMPERATURE_FAULT = "temperature_fault"
CONF_T_FREQUENCY = "t_frequency"
CONF_T_POWER = "t_power"
CONF_T_VOLTAGE = "t_voltage"

CONFIG_SCHEMA = CONFIG_SCHEMA_BASE.extend({
    cv.GenerateID(): cv.declare_id(OmnikInverter),
    # Omnik 0x10/0x80 message.
    cv.Optional(CONF_SERIAL_DEVICE_NUMBER,
                default={
                    CONF_NAME: "Inverter Serial device number",
                    CONF_ENTITY_CATEGORY: ENTITY_CATEGORY_DIAGNOSTIC,
                }): ts.text_sensor_schema(),
    # Omnik 0x10/0x81 message.
    cv.Optional(CONF_STATUS_10_81,
                default={
                    CONF_NAME: "Inverter Status 0x10/0x81",
                    CONF_ENTITY_CATEGORY: ENTITY_CATEGORY_DIAGNOSTIC,
                }): ts.text_sensor_schema(),
    # Omnik 0x10/0x84 message.
    cv.Optional(CONF_STATUS_10_84,
                default={
                    CONF_NAME: "Inverter Status 0x10/0x84",
                    CONF_ENTITY_CATEGORY: ENTITY_CATEGORY_DIAGNOSTIC,
                }): ts.text_sensor_schema(),
    # Omnik 0x11/0x83 message.
    cv.Optional(CONF_NR_OF_PHASES,
                default={
                    CONF_NAME: "Inverter Number of phases",
                    CONF_ENTITY_CATEGORY: ENTITY_CATEGORY_DIAGNOSTIC,
                }): ts.text_sensor_schema(),
    cv.Optional(CONF_RATED_POWER,
                default={
                    CONF_NAME: "Inverter Rated power",
                    CONF_ENTITY_CATEGORY: ENTITY_CATEGORY_DIAGNOSTIC,
                }): ts.text_sensor_schema(),
    cv.Optional(CONF_COUNTRY,
                default={
                    CONF_NAME: "Inverter Country",
                    CONF_ENTITY_CATEGORY: ENTITY_CATEGORY_DIAGNOSTIC,
                }): ts.text_sensor_schema(),
    cv.Optional(CONF_FIRMWARE_VERSION_MAIN,
                default={
                    CONF_NAME: "Inverter Firmware version (main)",
                    CONF_ENTITY_CATEGORY: ENTITY_CATEGORY_DIAGNOSTIC,
                }): ts.text_sensor_schema(),
    cv.Optional(CONF_FIRMWARE_VERSION_SLAVE,
                default={
                    CONF_NAME: "Inverter Firmware version (slave)",
                    CONF_ENTITY_CATEGORY: ENTITY_CATEGORY_DIAGNOSTIC,
                }): ts.text_sensor_schema(),
    cv.Optional(CONF_INVERTER_MODEL,
                default={
                    CONF_NAME: "Inverter Model",
                    CONF_ENTITY_CATEGORY: ENTITY_CATEGORY_DIAGNOSTIC,
                }): ts.text_sensor_schema(),
    cv.Optional(CONF_BRAND,
                default={
                    CONF_NAME: "Inverter Brand",
                    CONF_ENTITY_CATEGORY: ENTITY_CATEGORY_DIAGNOSTIC,
                }): ts.text_sensor_schema(),
    cv.Optional(CONF_MESSAGE_11_83_BYTES_60_77,
                default={
                    CONF_NAME: "Inverter Message 0x11/0x83 bytes 60-77",
                    CONF_ENTITY_CATEGORY: ENTITY_CATEGORY_DIAGNOSTIC,
                }): ts.text_sensor_schema(),
    # Omnik 0x11/0x90 message.
    cv.Optional(CONF_TEMPERATURE,
                default={
                    CONF_NAME: "Inverter Temperature",
                    CONF_UNIT_OF_MEASUREMENT: UNIT_CELSIUS,
                    CONF_DEVICE_CLASS: DEVICE_CLASS_TEMPERATURE,
                    CONF_STATE_CLASS: STATE_CLASS_MEASUREMENT,
                    CONF_ACCURACY_DECIMALS: 1,
                    CONF_ENTITY_CATEGORY: ENTITY_CATEGORY_NONE,
                }): s.sensor_schema(),
    cv.Optional(CONF_PV1_VOLTAGE,
                default={
                    CONF_NAME: "Inverter PV1 voltage",
                    CONF_UNIT_OF_MEASUREMENT: UNIT_VOLT,
                    CONF_DEVICE_CLASS: DEVICE_CLASS_VOLTAGE,
                    CONF_STATE_CLASS: STATE_CLASS_MEASUREMENT,
                    CONF_ACCURACY_DECIMALS: 1,
                    CONF_ENTITY_CATEGORY: ENTITY_CATEGORY_NONE,
                }): s.sensor_schema(),
    cv.Optional(CONF_PV2_VOLTAGE,
                default={
                    CONF_NAME: "Inverter PV2 voltage",
                    CONF_UNIT_OF_MEASUREMENT: UNIT_VOLT,
                    CONF_DEVICE_CLASS: DEVICE_CLASS_VOLTAGE,
                    CONF_STATE_CLASS: STATE_CLASS_MEASUREMENT,
                    CONF_ACCURACY_DECIMALS: 1,
                    CONF_INTERNAL: True,
                    CONF_ENTITY_CATEGORY: ENTITY_CATEGORY_NONE,
                }): s.sensor_schema(),
    cv.Optional(CONF_PV3_VOLTAGE,
                default={
                    CONF_NAME: "Inverter PV3 voltage",
                    CONF_UNIT_OF_MEASUREMENT: UNIT_VOLT,
                    CONF_DEVICE_CLASS: DEVICE_CLASS_VOLTAGE,
                    CONF_STATE_CLASS: STATE_CLASS_MEASUREMENT,
                    CONF_ACCURACY_DECIMALS: 1,
                    CONF_INTERNAL: True,
                    CONF_ENTITY_CATEGORY: ENTITY_CATEGORY_NONE,
                }): s.sensor_schema(),
    cv.Optional(CONF_PV1_CURRENT,
                default={
                    CONF_NAME: "Inverter PV1 current",
                    CONF_UNIT_OF_MEASUREMENT: UNIT_AMPERE,
                    CONF_DEVICE_CLASS: DEVICE_CLASS_CURRENT,
                    CONF_STATE_CLASS: STATE_CLASS_MEASUREMENT,
                    CONF_ACCURACY_DECIMALS: 1,
                    CONF_ENTITY_CATEGORY: ENTITY_CATEGORY_NONE,
                }): s.sensor_schema(),
    cv.Optional(CONF_PV2_CURRENT,
                default={
                    CONF_NAME: "Inverter PV2 current",
                    CONF_UNIT_OF_MEASUREMENT: UNIT_AMPERE,
                    CONF_DEVICE_CLASS: DEVICE_CLASS_CURRENT,
                    CONF_STATE_CLASS: STATE_CLASS_MEASUREMENT,
                    CONF_ACCURACY_DECIMALS: 1,
                    CONF_INTERNAL: True,
                    CONF_ENTITY_CATEGORY: ENTITY_CATEGORY_NONE,
                }): s.sensor_schema(),
    cv.Optional(CONF_PV3_CURRENT,
                default={
                    CONF_NAME: "Inverter PV3 current",
                    CONF_UNIT_OF_MEASUREMENT: UNIT_AMPERE,
                    CONF_DEVICE_CLASS: DEVICE_CLASS_CURRENT,
                    CONF_STATE_CLASS: STATE_CLASS_MEASUREMENT,
                    CONF_ACCURACY_DECIMALS: 1,
                    CONF_INTERNAL: True,
                    CONF_ENTITY_CATEGORY: ENTITY_CATEGORY_NONE,
                }): s.sensor_schema(),
    cv.Optional(CONF_R_CURRENT,
                default={
                    CONF_NAME: "Inverter R current",
                    CONF_UNIT_OF_MEASUREMENT: UNIT_AMPERE,
                    CONF_DEVICE_CLASS: DEVICE_CLASS_CURRENT,
                    CONF_STATE_CLASS: STATE_CLASS_MEASUREMENT,
                    CONF_ACCURACY_DECIMALS: 1,
                    CONF_ENTITY_CATEGORY: ENTITY_CATEGORY_NONE,
                }): s.sensor_schema(),
    cv.Optional(CONF_S_CURRENT,
                default={
                    CONF_NAME: "Inverter S current",
                    CONF_UNIT_OF_MEASUREMENT: UNIT_AMPERE,
                    CONF_DEVICE_CLASS: DEVICE_CLASS_CURRENT,
                    CONF_STATE_CLASS: STATE_CLASS_MEASUREMENT,
                    CONF_ACCURACY_DECIMALS: 1,
                    CONF_INTERNAL: True,
                    CONF_ENTITY_CATEGORY: ENTITY_CATEGORY_NONE,
                }): s.sensor_schema(),
    cv.Optional(CONF_T_CURRENT,
                default={
                    CONF_NAME: "Inverter T current",
                    CONF_UNIT_OF_MEASUREMENT: UNIT_AMPERE,
                    CONF_DEVICE_CLASS: DEVICE_CLASS_CURRENT,
                    CONF_STATE_CLASS: STATE_CLASS_MEASUREMENT,
                    CONF_ACCURACY_DECIMALS: 1,
                    CONF_INTERNAL: True,
                    CONF_ENTITY_CATEGORY: ENTITY_CATEGORY_NONE,
                }): s.sensor_schema(),
    cv.Optional(CONF_R_VOLTAGE,
                default={
                    CONF_NAME: "Inverter R voltage",
                    CONF_UNIT_OF_MEASUREMENT: UNIT_VOLT,
                    CONF_DEVICE_CLASS: DEVICE_CLASS_VOLTAGE,
                    CONF_STATE_CLASS: STATE_CLASS_MEASUREMENT,
                    CONF_ACCURACY_DECIMALS: 1,
                    CONF_ENTITY_CATEGORY: ENTITY_CATEGORY_NONE,
                }): s.sensor_schema(),
    cv.Optional(CONF_S_VOLTAGE,
                default={
                    CONF_NAME: "Inverter S voltage",
                    CONF_UNIT_OF_MEASUREMENT: UNIT_VOLT,
                    CONF_DEVICE_CLASS: DEVICE_CLASS_VOLTAGE,
                    CONF_STATE_CLASS: STATE_CLASS_MEASUREMENT,
                    CONF_ACCURACY_DECIMALS: 1,
                    CONF_INTERNAL: True,
                    CONF_ENTITY_CATEGORY: ENTITY_CATEGORY_NONE,
                }): s.sensor_schema(),
    cv.Optional(CONF_T_VOLTAGE,
                default={
                    CONF_NAME: "Inverter T voltage",
                    CONF_UNIT_OF_MEASUREMENT: UNIT_VOLT,
                    CONF_DEVICE_CLASS: DEVICE_CLASS_VOLTAGE,
                    CONF_STATE_CLASS: STATE_CLASS_MEASUREMENT,
                    CONF_ACCURACY_DECIMALS: 1,
                    CONF_INTERNAL: True,
                    CONF_ENTITY_CATEGORY: ENTITY_CATEGORY_NONE,
                }): s.sensor_schema(),
    cv.Optional(CONF_R_FREQUENCY,
                default={
                    CONF_NAME: "Inverter R frequency",
                    CONF_UNIT_OF_MEASUREMENT: UNIT_HERTZ,
                    CONF_DEVICE_CLASS: DEVICE_CLASS_FREQUENCY,
                    CONF_STATE_CLASS: STATE_CLASS_MEASUREMENT,
                    CONF_ACCURACY_DECIMALS: 2,
                    CONF_ENTITY_CATEGORY: ENTITY_CATEGORY_NONE,
                }): s.sensor_schema(),
    cv.Optional(CONF_R_POWER,
                default={
                    CONF_NAME: "Inverter R power",
                    CONF_UNIT_OF_MEASUREMENT: UNIT_KILOWATT,
                    CONF_DEVICE_CLASS: DEVICE_CLASS_POWER,
                    CONF_STATE_CLASS: STATE_CLASS_MEASUREMENT,
                    CONF_ACCURACY_DECIMALS: 3,
                    CONF_ENTITY_CATEGORY: ENTITY_CATEGORY_NONE,
                }): s.sensor_schema(),
    cv.Optional(CONF_S_FREQUENCY,
                default={
                    CONF_NAME: "Inverter S frequency",
                    CONF_UNIT_OF_MEASUREMENT: UNIT_HERTZ,
                    CONF_DEVICE_CLASS: DEVICE_CLASS_FREQUENCY,
                    CONF_STATE_CLASS: STATE_CLASS_MEASUREMENT,
                    CONF_ACCURACY_DECIMALS: 2,
                    CONF_INTERNAL: True,
                    CONF_ENTITY_CATEGORY: ENTITY_CATEGORY_NONE,
                }): s.sensor_schema(),
    cv.Optional(CONF_S_POWER,
                default={
                    CONF_NAME: "Inverter S power",
                    CONF_UNIT_OF_MEASUREMENT: UNIT_KILOWATT,
                    CONF_DEVICE_CLASS: DEVICE_CLASS_POWER,
                    CONF_STATE_CLASS: STATE_CLASS_MEASUREMENT,
                    CONF_ACCURACY_DECIMALS: 3,
                    CONF_INTERNAL: True,
                    CONF_ENTITY_CATEGORY: ENTITY_CATEGORY_NONE,
                }): s.sensor_schema(),
    cv.Optional(CONF_T_FREQUENCY,
                default={
                    CONF_NAME: "Inverter T frequency",
                    CONF_UNIT_OF_MEASUREMENT: UNIT_HERTZ,
                    CONF_DEVICE_CLASS: DEVICE_CLASS_FREQUENCY,
                    CONF_STATE_CLASS: STATE_CLASS_MEASUREMENT,
                    CONF_ACCURACY_DECIMALS: 2,
                    CONF_INTERNAL: True,
                    CONF_ENTITY_CATEGORY: ENTITY_CATEGORY_NONE,
                }): s.sensor_schema(),
    cv.Optional(CONF_T_POWER,
                default={
                    CONF_NAME: "Inverter T power",
                    CONF_UNIT_OF_MEASUREMENT: UNIT_KILOWATT,
                    CONF_DEVICE_CLASS: DEVICE_CLASS_POWER,
                    CONF_STATE_CLASS: STATE_CLASS_MEASUREMENT,
                    CONF_ACCURACY_DECIMALS: 3,
                    CONF_INTERNAL: True,
                    CONF_ENTITY_CATEGORY: ENTITY_CATEGORY_NONE,
                }): s.sensor_schema(),
    cv.Optional(CONF_ENERGY_TODAY,
                default={
                    CONF_NAME: "Inverter Energy today",
                    CONF_UNIT_OF_MEASUREMENT: UNIT_KILOWATT_HOURS,
                    CONF_DEVICE_CLASS: DEVICE_CLASS_ENERGY,
                    CONF_STATE_CLASS: STATE_CLASS_TOTAL_INCREASING,
                    CONF_ACCURACY_DECIMALS: 2,
                    CONF_ENTITY_CATEGORY: ENTITY_CATEGORY_NONE,
                }): s.sensor_schema(),
    cv.Optional(CONF_ENERGY_TOTAL,
                default={
                    CONF_NAME: "Inverter Energy total",
                    CONF_UNIT_OF_MEASUREMENT: UNIT_KILOWATT_HOURS,
                    CONF_DEVICE_CLASS: DEVICE_CLASS_ENERGY,
                    CONF_STATE_CLASS: STATE_CLASS_TOTAL,
                    CONF_ACCURACY_DECIMALS: 1,
                    CONF_ENTITY_CATEGORY: ENTITY_CATEGORY_NONE,
                }): s.sensor_schema(),
    cv.Optional(CONF_HOURS_TOTAL,
                default={
                    CONF_NAME: "Inverter Hours total",
                    CONF_UNIT_OF_MEASUREMENT: UNIT_HOUR,
                    CONF_DEVICE_CLASS: DEVICE_CLASS_EMPTY,
                    CONF_STATE_CLASS: STATE_CLASS_TOTAL,
                    CONF_ACCURACY_DECIMALS: 0,
                    CONF_ENTITY_CATEGORY: ENTITY_CATEGORY_NONE,
                }): s.sensor_schema(),
    cv.Optional(CONF_RUN_STATE,
                default={
                    CONF_NAME: "Inverter Run state",
                    CONF_ENTITY_CATEGORY: ENTITY_CATEGORY_DIAGNOSTIC,
                }): ts.text_sensor_schema(),
    cv.Optional(CONF_GRID_VOLTAGE_FAULT_VALUE,
                default={
                    CONF_NAME: "Inverter Grid voltage fault",
                    CONF_UNIT_OF_MEASUREMENT: UNIT_VOLT,
                    CONF_DEVICE_CLASS: DEVICE_CLASS_VOLTAGE,
                    CONF_STATE_CLASS: STATE_CLASS_MEASUREMENT,
                    CONF_ACCURACY_DECIMALS: 1,
                    CONF_ENTITY_CATEGORY: ENTITY_CATEGORY_NONE,
                }): s.sensor_schema(),
    cv.Optional(CONF_GRID_FREQUENCY_FAULT_VALUE,
                default={
                    CONF_NAME: "Inverter Grid frequence fault",
                    CONF_UNIT_OF_MEASUREMENT: UNIT_HERTZ,
                    CONF_DEVICE_CLASS: DEVICE_CLASS_FREQUENCY,
                    CONF_STATE_CLASS: STATE_CLASS_MEASUREMENT,
                    CONF_ACCURACY_DECIMALS: 2,
                    CONF_ENTITY_CATEGORY: ENTITY_CATEGORY_NONE,
                }): s.sensor_schema(),
    cv.Optional(CONF_GRID_IMPEDANCE_FAULT_VALUE,
                default={
                    CONF_NAME: "Inverter Grid impedance fault",
                    CONF_UNIT_OF_MEASUREMENT: UNIT_DEGREES,
                    CONF_STATE_CLASS: STATE_CLASS_MEASUREMENT,
                    CONF_ACCURACY_DECIMALS: 3,
                    CONF_ENTITY_CATEGORY: ENTITY_CATEGORY_NONE,
                }): s.sensor_schema(),
    cv.Optional(CONF_TEMPERATURE_FAULT,
                default={
                    CONF_NAME: "Inverter Temperature fault",
                    CONF_UNIT_OF_MEASUREMENT: UNIT_CELSIUS,
                    CONF_DEVICE_CLASS: DEVICE_CLASS_TEMPERATURE,
                    CONF_STATE_CLASS: STATE_CLASS_MEASUREMENT,
                    CONF_ACCURACY_DECIMALS: 1,
                    CONF_ENTITY_CATEGORY: ENTITY_CATEGORY_NONE,
                }): s.sensor_schema(),
    cv.Optional(CONF_PV_VOLTAGE_FAULT,
                default={
                    CONF_NAME: "Inverter PV voltage fault",
                    CONF_UNIT_OF_MEASUREMENT: UNIT_VOLT,
                    CONF_DEVICE_CLASS: DEVICE_CLASS_VOLTAGE,
                    CONF_STATE_CLASS: STATE_CLASS_MEASUREMENT,
                    CONF_ACCURACY_DECIMALS: 1,
                    CONF_ENTITY_CATEGORY: ENTITY_CATEGORY_NONE,
                }): s.sensor_schema(),
    cv.Optional(CONF_GFCI_CURRENT_FAULT,
                default={
                    CONF_NAME: "Inverter GFCI current fault",
                    CONF_UNIT_OF_MEASUREMENT: UNIT_AMPERE,
                    CONF_DEVICE_CLASS: DEVICE_CLASS_CURRENT,
                    CONF_STATE_CLASS: STATE_CLASS_MEASUREMENT,
                    CONF_ACCURACY_DECIMALS: 3,
                    CONF_ENTITY_CATEGORY: ENTITY_CATEGORY_NONE,
                }): s.sensor_schema(),
    cv.Optional(CONF_ERROR_MESSAGE_BINARY_INDEX,
                default={
                    CONF_NAME: "Inverter Error index",
                    CONF_ENTITY_CATEGORY: ENTITY_CATEGORY_DIAGNOSTIC,
                }): ts.text_sensor_schema(),
    # Omnik 0x11/0xC3 message.
    cv.Optional(CONF_NR_OF_ALARMS,
                default={
                    CONF_NAME: "Inverter Number of alarms",
                    CONF_UNIT_OF_MEASUREMENT: UNIT_EMPTY,
                    CONF_DEVICE_CLASS: DEVICE_CLASS_EMPTY,
                    CONF_STATE_CLASS: STATE_CLASS_MEASUREMENT,
                    CONF_ACCURACY_DECIMALS: 0,
                    CONF_ENTITY_CATEGORY: ENTITY_CATEGORY_DIAGNOSTIC,
                }): s.sensor_schema(),
    # Omnik 0x12/0xC0 message.
    cv.Optional(CONF_STATUS_12_C0,
                default={
                    CONF_NAME: "Inverter Status 0x12/0xC0",
                    CONF_ENTITY_CATEGORY: ENTITY_CATEGORY_DIAGNOSTIC,
                }): ts.text_sensor_schema(),
    # Omnik 0x12/0xC1 message.
    cv.Optional(CONF_STATUS_12_C1,
                default={
                    CONF_NAME: "Inverter Status 0x12/0xC1",
                    CONF_ENTITY_CATEGORY: ENTITY_CATEGORY_DIAGNOSTIC,
                }): ts.text_sensor_schema(),
})

async def to_code(config):
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
