#include "omnik_inverter.h"
#include <bitset>

namespace esphome {
namespace omnik_inverter {

// Tag that is used for log messages.
static const char *const TAG = "omnik_inverter";

/**
 * Convert an integer value to a run state string.
 */
static std::string to_run_state(uint16_t run_state) {
  switch (run_state) {
  case 0:
    return "Startup";
  case 1:
    return "Online";
  case 2:
    return "Waiting";
  default:
    return std::to_string(run_state);
  }
}

/**
 * Convert an integer value to a version string.
 */
static std::string to_version(uint32_t version) {
  unsigned int build = version % 10000;
  version /= 10000;
  unsigned int minor = version % 100;
  version /= 100;
  unsigned int major = version;

  char buffer[32];
  snprintf(buffer, sizeof(buffer), "V%d.%02dBuild%d", major, minor, build);

  return std::string(buffer);
}

/**
 * @see the header file.
 */
void OmnikInverter::dump_config() {
  ESP_LOGCONFIG(TAG, "OmnikInverter:");
  omnik_base::dump_config(TAG, "  ", this);
  // Dump sensors of Omnik 0x10/0x80 message.
  ESP_LOGCONFIG(TAG, "  serial_device_number:");
  omnik_base::dump_config(TAG, "    ", serial_device_number_text_sensor_);
  // Dump sensors of Omnik 0x10/0x81 message.
  ESP_LOGCONFIG(TAG, "  status_10_81:");
  omnik_base::dump_config(TAG, "    ", status_10_81_text_sensor_);
  // Dump sensors of Omnik 0x10/0x84 message.
  ESP_LOGCONFIG(TAG, "  status_10_84:");
  omnik_base::dump_config(TAG, "    ", status_10_84_text_sensor_);
  // Dump sensors of Omnik 0x11/0x83 message.
  ESP_LOGCONFIG(TAG, "  nr_of_phases:");
  omnik_base::dump_config(TAG, "    ", nr_of_phases_text_sensor_);
  ESP_LOGCONFIG(TAG, "  rated_power:");
  omnik_base::dump_config(TAG, "    ", rated_power_text_sensor_);
  ESP_LOGCONFIG(TAG, "  country:");
  omnik_base::dump_config(TAG, "    ", country_text_sensor_);
  ESP_LOGCONFIG(TAG, "  firmware_version_main:");
  omnik_base::dump_config(TAG, "    ", firmware_version_main_text_sensor_);
  ESP_LOGCONFIG(TAG, "  firmware_version_slave:");
  omnik_base::dump_config(TAG, "    ", firmware_version_slave_text_sensor_);
  ESP_LOGCONFIG(TAG, "  inverter_model:");
  omnik_base::dump_config(TAG, "    ", inverter_model_text_sensor_);
  ESP_LOGCONFIG(TAG, "  brand:");
  omnik_base::dump_config(TAG, "    ", brand_text_sensor_);
  ESP_LOGCONFIG(TAG, "  message_11_83_bytes_60_77:");
  omnik_base::dump_config(TAG, "    ", message_11_83_bytes_60_77_text_sensor_);
  // Dump sensors of Omnik 0x11/0x90 message.
  ESP_LOGCONFIG(TAG, "  temperature:");
  omnik_base::dump_config(TAG, "    ", temperature_sensor_);
  ESP_LOGCONFIG(TAG, "  pv1_voltage:");
  omnik_base::dump_config(TAG, "    ", pv1_voltage_sensor_);
  ESP_LOGCONFIG(TAG, "  pv2_voltage:");
  omnik_base::dump_config(TAG, "    ", pv2_voltage_sensor_);
  ESP_LOGCONFIG(TAG, "  pv3_voltage:");
  omnik_base::dump_config(TAG, "    ", pv3_voltage_sensor_);
  ESP_LOGCONFIG(TAG, "  pv1_current:");
  omnik_base::dump_config(TAG, "    ", pv1_current_sensor_);
  ESP_LOGCONFIG(TAG, "  pv2_current:");
  omnik_base::dump_config(TAG, "    ", pv2_current_sensor_);
  ESP_LOGCONFIG(TAG, "  pv3_current:");
  omnik_base::dump_config(TAG, "    ", pv3_current_sensor_);
  ESP_LOGCONFIG(TAG, "  r_current:");
  omnik_base::dump_config(TAG, "    ", r_current_sensor_);
  ESP_LOGCONFIG(TAG, "  s_current:");
  omnik_base::dump_config(TAG, "    ", s_current_sensor_);
  ESP_LOGCONFIG(TAG, "  t_current:");
  omnik_base::dump_config(TAG, "    ", t_current_sensor_);
  ESP_LOGCONFIG(TAG, "  r_voltage:");
  omnik_base::dump_config(TAG, "    ", r_voltage_sensor_);
  ESP_LOGCONFIG(TAG, "  s_voltage:");
  omnik_base::dump_config(TAG, "    ", s_voltage_sensor_);
  ESP_LOGCONFIG(TAG, "  t_voltage:");
  omnik_base::dump_config(TAG, "    ", t_voltage_sensor_);
  ESP_LOGCONFIG(TAG, "  r_frequency:");
  omnik_base::dump_config(TAG, "    ", r_frequency_sensor_);
  ESP_LOGCONFIG(TAG, "  r_power:");
  omnik_base::dump_config(TAG, "    ", r_power_sensor_);
  ESP_LOGCONFIG(TAG, "  s_frequency:");
  omnik_base::dump_config(TAG, "    ", s_frequency_sensor_);
  ESP_LOGCONFIG(TAG, "  s_power:");
  omnik_base::dump_config(TAG, "    ", s_power_sensor_);
  ESP_LOGCONFIG(TAG, "  t_frequency:");
  omnik_base::dump_config(TAG, "    ", t_frequency_sensor_);
  ESP_LOGCONFIG(TAG, "  t_power:");
  omnik_base::dump_config(TAG, "    ", t_power_sensor_);
  ESP_LOGCONFIG(TAG, "  energy_today:");
  omnik_base::dump_config(TAG, "    ", energy_today_sensor_);
  ESP_LOGCONFIG(TAG, "  energy_total:");
  omnik_base::dump_config(TAG, "    ", energy_total_sensor_);
  ESP_LOGCONFIG(TAG, "  hours_total:");
  omnik_base::dump_config(TAG, "    ", hours_total_sensor_);
  ESP_LOGCONFIG(TAG, "  run_state:");
  omnik_base::dump_config(TAG, "    ", run_state_text_sensor_);
  ESP_LOGCONFIG(TAG, "  grid_voltage_fault_value:");
  omnik_base::dump_config(TAG, "    ", grid_voltage_fault_value_sensor_);
  ESP_LOGCONFIG(TAG, "  grid_frequency_fault_value:");
  omnik_base::dump_config(TAG, "    ", grid_frequency_fault_value_sensor_);
  ESP_LOGCONFIG(TAG, "  grid_impedance_fault_value:");
  omnik_base::dump_config(TAG, "    ", grid_impedance_fault_value_sensor_);
  ESP_LOGCONFIG(TAG, "  temperature_fault:");
  omnik_base::dump_config(TAG, "    ", temperature_fault_sensor_);
  ESP_LOGCONFIG(TAG, "  pv_voltage_fault:");
  omnik_base::dump_config(TAG, "    ", pv_voltage_fault_sensor_);
  ESP_LOGCONFIG(TAG, "  gfci_current_fault:");
  omnik_base::dump_config(TAG, "    ", gfci_current_fault_sensor_);
  ESP_LOGCONFIG(TAG, "  error_message_binary_index:");
  omnik_base::dump_config(TAG, "    ", error_message_binary_index_text_sensor_);
  // Dump sensors of Omnik 0x11/0xC3 message.
  ESP_LOGCONFIG(TAG, "  nr_of_alarms:");
  omnik_base::dump_config(TAG, "    ", nr_of_alarms_sensor_);
  // Dump sensors of Omnik 0x12/0xC0 message.
  ESP_LOGCONFIG(TAG, "  status_12_C0:");
  omnik_base::dump_config(TAG, "    ", status_12_c0_text_sensor_);
  // Dump sensors of Omnik 0x12/0xC1 message.
  ESP_LOGCONFIG(TAG, "  status_12_C1:");
  omnik_base::dump_config(TAG, "    ", status_12_c1_text_sensor_);
}

/**
 * @see the header file.
 */
void OmnikInverter::process_omnik_message(uint8_t control_code,
                                          uint8_t function_code,
                                          ByteBuffer &data) {
  switch (OMNIK_MESSAGE_ID(control_code, function_code)) {
  case OMNIK_MESSAGE_ID(0x10, 0x80):
    omnik_message_10_80(data);
    break;

  case OMNIK_MESSAGE_ID(0x10, 0x81):
    omnik_message_10_81(data);
    break;

  case OMNIK_MESSAGE_ID(0x10, 0x84):
    omnik_message_10_84(data);
    break;

  case OMNIK_MESSAGE_ID(0x11, 0x83):
    omnik_message_11_83(data);
    break;

  case OMNIK_MESSAGE_ID(0x11, 0x90):
    omnik_message_11_90(data);
    break;

  case OMNIK_MESSAGE_ID(0x11, 0xC3):
    omnik_message_11_c3(data);
    break;

  case OMNIK_MESSAGE_ID(0x12, 0xC0):
    omnik_message_12_c0(data);
    break;

  case OMNIK_MESSAGE_ID(0x12, 0xC1):
    omnik_message_12_c1(data);
    break;

  case OMNIK_MESSAGE_ID(0xFF, 0xFF):
    omnik_message_no_data(data);
    break;

  default:
    ESP_LOGW(TAG,
             "Unknown combination: control_code=0x%02x, function_code=0x%02x",
             control_code, function_code);
    break;
  }
}

/**
 * @see the header file.
 */
void OmnikInverter::omnik_message_10_80(ByteBuffer &buffer) {
  std::string serial_number = omnik_base::to_string(buffer.get_vector(16));
  serial_device_number_text_sensor_->publish_state(serial_number);
}

/**
 * @see the header file.
 */
void OmnikInverter::omnik_message_10_81(ByteBuffer &buffer) {
  uint8_t status = buffer.get_uint8();
  status_10_81_text_sensor_->publish_state(omnik_base::to_hex(status));
}

/**
 * @see the header file.
 */
void OmnikInverter::omnik_message_10_84(ByteBuffer &buffer) {
  uint8_t status = buffer.get_uint8();
  status_10_84_text_sensor_->publish_state(omnik_base::to_hex(status));
}

/**
 * @see the header file.
 */
void OmnikInverter::omnik_message_11_83(ByteBuffer &buffer) {
  uint8_t nr_of_phases = buffer.get_uint8();
  nr_of_phases_text_sensor_->publish_state(std::to_string(nr_of_phases));

  std::string rated_power = omnik_base::to_string(buffer.get_vector(6));
  rated_power_text_sensor_->publish_state(rated_power);

  std::string country = omnik_base::to_string(buffer.get_vector(2));
  country_text_sensor_->publish_state(country);

  uint32_t firmware_version_main = buffer.get_uint24();
  firmware_version_main_text_sensor_->publish_state(
      to_version(firmware_version_main));

  uint32_t firmware_version_slave = buffer.get_uint32();
  firmware_version_slave_text_sensor_->publish_state(
      to_version(firmware_version_slave));

  std::string inverter_model = omnik_base::to_string(buffer.get_vector(12));
  inverter_model_text_sensor_->publish_state(inverter_model);

  std::string brand = omnik_base::to_string(buffer.get_vector(16));
  brand_text_sensor_->publish_state(brand);

  std::string serial_number = omnik_base::to_string(buffer.get_vector(16));
  serial_device_number_text_sensor_->publish_state(serial_number);

  std::string message_11_83_bytes_60_77 =
      omnik_base::to_string(buffer.get_vector(17));
  message_11_83_bytes_60_77_text_sensor_->publish_state(
      message_11_83_bytes_60_77);
}

/**
 * @see the header file.
 */
void OmnikInverter::omnik_message_11_90(ByteBuffer &buffer) {
  int16_t temperature = buffer.get_int16();
  temperature_sensor_->publish_state(temperature / 10.0);

  uint16_t pv1_voltage = buffer.get_uint16();
  pv1_voltage_sensor_->publish_state(pv1_voltage / 10.0);

  uint16_t pv2_voltage = buffer.get_uint16();
  pv2_voltage_sensor_->publish_state(pv2_voltage / 10.0);

  uint16_t pv3_voltage = buffer.get_uint16();
  pv3_voltage_sensor_->publish_state(pv3_voltage / 10.0);

  uint16_t pv1_current = buffer.get_uint16();
  pv1_current_sensor_->publish_state(pv1_current / 10.0);

  uint16_t pv2_current = buffer.get_uint16();
  pv2_current_sensor_->publish_state(pv2_current / 10.0);

  uint16_t pv3_current = buffer.get_uint16();
  pv3_current_sensor_->publish_state(pv3_current / 10.0);

  uint16_t r_current = buffer.get_uint16();
  r_current_sensor_->publish_state(r_current / 10.0);

  uint16_t s_current = buffer.get_uint16();
  s_current_sensor_->publish_state(s_current / 10.0);

  uint16_t t_current = buffer.get_uint16();
  t_current_sensor_->publish_state(t_current / 10.0);

  uint16_t r_voltage = buffer.get_uint16();
  r_voltage_sensor_->publish_state(r_voltage / 10.0);

  uint16_t s_voltage = buffer.get_uint16();
  s_voltage_sensor_->publish_state(s_voltage / 10.0);

  uint16_t t_voltage = buffer.get_uint16();
  t_voltage_sensor_->publish_state(t_voltage / 10.0);

  uint16_t r_frequency = buffer.get_uint16();
  r_frequency_sensor_->publish_state(r_frequency / 100.0);

  uint16_t r_power = buffer.get_uint16();
  r_power_sensor_->publish_state(r_power / 1000.0);

  uint16_t s_frequency = buffer.get_uint16();
  s_frequency_sensor_->publish_state(s_frequency / 100.0);

  uint16_t s_power = buffer.get_uint16();
  s_power_sensor_->publish_state(s_power / 1000.0);

  uint16_t t_frequency = buffer.get_uint16();
  t_frequency_sensor_->publish_state(t_frequency / 100.0);

  uint16_t t_power = buffer.get_uint16();
  t_power_sensor_->publish_state(t_power / 1000.0);

  uint16_t energy_today = buffer.get_uint16();
  energy_today_sensor_->publish_state(energy_today / 100.0);

  uint32_t energy_total = buffer.get_uint32();
  energy_total_sensor_->publish_state(energy_total / 10.0);

  uint32_t hours_total = buffer.get_uint32();
  hours_total_sensor_->publish_state(hours_total);

  uint16_t run_state = buffer.get_uint16();
  run_state_text_sensor_->publish_state(to_run_state(run_state));

  uint16_t grid_voltage_fault_value = buffer.get_uint16();
  grid_voltage_fault_value_sensor_->publish_state(grid_voltage_fault_value /
                                                  10.0);

  uint16_t grid_frequency_fault_value = buffer.get_uint16();
  grid_frequency_fault_value_sensor_->publish_state(grid_frequency_fault_value /
                                                    100.0);

  uint16_t grid_impedance_fault_value = buffer.get_uint16();
  grid_impedance_fault_value_sensor_->publish_state(grid_impedance_fault_value /
                                                    1000.0);

  uint16_t temperature_fault = buffer.get_uint16();
  temperature_fault_sensor_->publish_state(temperature_fault / 10.0);

  uint16_t pv_voltage_fault = buffer.get_uint16();
  pv_voltage_fault_sensor_->publish_state(pv_voltage_fault / 10.0);

  uint16_t gfci_current_fault = buffer.get_uint16();
  gfci_current_fault_sensor_->publish_state(gfci_current_fault / 1000.0);

  uint32_t error_message_binary_index = buffer.get_uint32();
  error_message_binary_index_text_sensor_->publish_state(
      std::bitset<32>(error_message_binary_index).to_string());

  std::string main_firmware_version =
      omnik_base::to_string(buffer.get_vector(20));
  if (!main_firmware_version.empty() && main_firmware_version[0] != '\0') {
    firmware_version_main_text_sensor_->publish_state(main_firmware_version);
  }

  std::string slave_firmware_version =
      omnik_base::to_string(buffer.get_vector(20));
  if (!slave_firmware_version.empty() && slave_firmware_version[0] != '\0') {
    firmware_version_slave_text_sensor_->publish_state(slave_firmware_version);
  }
}

/**
 * @see the header file.
 */
void OmnikInverter::omnik_message_11_c3(ByteBuffer &buffer) {
  uint8_t nr_of_alarms = buffer.get_uint8();
  nr_of_alarms_sensor_->publish_state(nr_of_alarms);
}

/**
 * @see the header file.
 */
void OmnikInverter::omnik_message_12_c0(ByteBuffer &buffer) {
  uint8_t status = buffer.get_uint8();
  status_12_c0_text_sensor_->publish_state(omnik_base::to_hex(status));
}

/**
 * @see the header file.
 */
void OmnikInverter::omnik_message_12_c1(ByteBuffer &buffer) {
  uint8_t status = buffer.get_uint8();
  status_12_c1_text_sensor_->publish_state(omnik_base::to_hex(status));
}

} // namespace omnik_inverter
} // namespace esphome
