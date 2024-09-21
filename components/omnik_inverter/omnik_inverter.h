#pragma once

#include "esphome/components/omnik_base/omnik_base.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/text_sensor/text_sensor.h"

namespace esphome {
namespace omnik_inverter {

/**
 * This class is responsible for processing the messages received from the Omnik
 * inverter.
 */
class OmnikInverter : public omnik_base::OmnikBase {
public:
  /**
   * Log the current configuration.
   */
  void dump_config() override;

  // Omnik 0x10/0x80 message.
  SUB_TEXT_SENSOR(serial_device_number)
  // Omnik 0x10/0x81 message.
  SUB_TEXT_SENSOR(status_10_81)
  // Omnik 0x10/0x84 message.
  SUB_TEXT_SENSOR(status_10_84)
  // Omnik 0x11/0x83 message.
  SUB_TEXT_SENSOR(nr_of_phases)
  SUB_TEXT_SENSOR(rated_power)
  SUB_TEXT_SENSOR(country)
  SUB_TEXT_SENSOR(firmware_version_main)
  SUB_TEXT_SENSOR(firmware_version_slave)
  SUB_TEXT_SENSOR(inverter_model)
  SUB_TEXT_SENSOR(brand)
  SUB_TEXT_SENSOR(message_11_83_bytes_60_77)
  // Omnik 0x11/0x90 message.
  SUB_SENSOR(temperature)
  SUB_SENSOR(pv1_voltage)
  SUB_SENSOR(pv2_voltage)
  SUB_SENSOR(pv3_voltage)
  SUB_SENSOR(pv1_current)
  SUB_SENSOR(pv2_current)
  SUB_SENSOR(pv3_current)
  SUB_SENSOR(r_current)
  SUB_SENSOR(s_current)
  SUB_SENSOR(t_current)
  SUB_SENSOR(r_voltage)
  SUB_SENSOR(s_voltage)
  SUB_SENSOR(t_voltage)
  SUB_SENSOR(r_frequency)
  SUB_SENSOR(r_power)
  SUB_SENSOR(s_frequency)
  SUB_SENSOR(s_power)
  SUB_SENSOR(t_frequency)
  SUB_SENSOR(t_power)
  SUB_SENSOR(energy_today)
  SUB_SENSOR(energy_total)
  SUB_SENSOR(hours_total)
  SUB_TEXT_SENSOR(run_state)
  SUB_SENSOR(grid_voltage_fault_value)
  SUB_SENSOR(grid_frequency_fault_value)
  SUB_SENSOR(grid_impedance_fault_value)
  SUB_SENSOR(temperature_fault)
  SUB_SENSOR(pv_voltage_fault)
  SUB_SENSOR(gfci_current_fault)
  SUB_TEXT_SENSOR(error_message_binary_index)
  // SUB_TEXT_SENSOR(main_firmware_version)
  // SUB_TEXT_SENSOR(slave_firmware_version)
  // Omnik 0x11/0xC3 message.
  SUB_SENSOR(nr_of_alarms)
  // Omnik 0x12/0xC0 message.
  SUB_TEXT_SENSOR(status_12_c0)
  // Omnik 0x12/0xC1 message.
  SUB_TEXT_SENSOR(status_12_c1)

protected:
  /**
   * process an Omnik message.
   *
   * See omnik_base::OmnikBase for a full description.
   */
  void process_omnik_message(uint8_t control_code, uint8_t function_code,
                             std::vector<uint8_t> const &data) override;

private:
  /**
   * Process an Omnik message that contains no data.
   */
  void omnik_message_no_data(std::vector<uint8_t> const &data) {}

  /**
   * Process an Omnik 0x10/0x80 message.
   *
   * @param data The data of the message.
   * 		 data[0-15]: Inverter serial number
   */
  void omnik_message_10_80(std::vector<uint8_t> const &data);

  /**
   * Process an Omnik 0x10/0x81 message.
   *
   * @param data The data of the message.
   * 		 data[0]: Ok (0x06)
   */
  void omnik_message_10_81(std::vector<uint8_t> const &data);

  /**
   * Process an Omnik 0x10/0x84 message.
   *
   * @param data The data of the message.
   * 		 data[0]: Ok (0x06)
   */
  void omnik_message_10_84(std::vector<uint8_t> const &data);

  /**
   * Process an Omnik 0x11/0x83 message.
   *
   * @param data The data of the message.
   *             data[0]:     ??
   *             data[1-4]:   Rated power
   *             data[5-15]:  ??
   *             data[16-27]: Inverter model
   *             data[28-43]: Brand
   *             data[44-59]: Inverter serial number
   *             data[60-76]: ??
   */
  void omnik_message_11_83(std::vector<uint8_t> const &data);

  /**
   * Process an Omnik 0x11/0x90 message.
   *
   * @param data The data of the message.
   *             data[0-1]:   Temperature
   *             data[2-3]:   PV1 Voltage
   *             data[4-5]:   PV2 Voltage
   *             data[6-7]:   PV3 Voltage
   *             data[8-9]:   PV1 Current
   *             data[10-11]: PV2 Current
   *             data[12-13]: PV3 Current
   *             data[14-15]: R Current
   *             data[16-17]: S Current
   *             data[18-19]: T Current
   *             data[20-21]: R Voltage
   *             data[22-23]: S Voltage
   *             data[24-25]: T Voltage
   *             data[26-27]: R Frequency
   *             data[28-29]: R Power
   *             data[30-31]: S Frequency
   *             data[32-33]: S Power
   *             data[34-35]: T Frequency
   *             data[36-37]: T Power
   *             data[38-39]: Energy today
   *             data[40-43]: Energy total
   *             data[44-47]: Hour total
   *             data[48-49]: Run state
   *             data[50-51]: Grid voltage fault
   *             data[52-53]: Grid frequency fault
   *             data[54-55]: Grid impedance fault
   *             data[56-57]: Temperature fault
   *             data[58-59]: PV voltage fault
   *             data[60-61]: GFCI current fault
   *             data[62-65]: Error message binary index
   *             data[66-85]: Inverter main firmware version
   *             data[86-105]: Inverter slave firmware version
   */
  void omnik_message_11_90(std::vector<uint8_t> const &data);

  /**
   * Process an Omnik 0x11/0xC3 message.
   *
   * @param data The data of the message.
   *             data[0]: Number of alarms.
   */
  void omnik_message_11_c3(std::vector<uint8_t> const &data);

  /**
   * Process an Omnik 0x12/0xC0 message.
   *
   * @param data The data of the message.
   * 		 data[0]: Ok (0x06)
   */
  void omnik_message_12_c0(std::vector<uint8_t> const &data);

  /**
   * Process an Omnik 0x12/0xC1 message.
   *
   * @param data The data of the message.
   * 		 data[0]: Ok (0x06)
   */
  void omnik_message_12_c1(std::vector<uint8_t> const &data);
};

} // namespace omnik_inverter
} // namespace esphome
