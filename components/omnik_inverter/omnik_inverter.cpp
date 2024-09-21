#include "omnik_inverter.h"
#include <bitset>

namespace esphome {
namespace omnik_inverter {

// Tag that is used for log messages.
static const char *const TAG = "omnik_inverter";

/**
 * Convert a 16 bit integer from network representation to host
 * representation.
 */
static int16_t ntoh16(int16_t value) {
#if BYTE_ORDER == BIG_ENDIAN
  return value;
#else
  return (value << 8 & 0xFF00) | (value >> 8 & 0x00FF);
#endif
}

/**
 * Convert a 16 bit integer from network representation to host
 * representation.
 */
[[maybe_unused]] static uint16_t ntoh16(uint16_t value) {
#if BYTE_ORDER == BIG_ENDIAN
  return value;
#else
  return (value << 8 & 0xFF00) | (value >> 8 & 0x00FF);
#endif
}

/**
 * Convert a 24 bit integer from network representation to host
 * representation.
 */
[[maybe_unused]] static int32_t ntoh24(int32_t value) {
#if BYTE_ORDER == BIG_ENDIAN
  return value;
#else
  return (value << 16 & 0xFF0000) | (value & 0x00FF00) |
         (value >> 16 & 0x0000FF);
#endif
}

/**
 * Convert a 24 bit integer from network representation to host
 * representation.
 */
static uint32_t ntoh24(uint32_t value) {
#if BYTE_ORDER == BIG_ENDIAN
  return value;
#else
  return (value << 16 & 0xFF0000) | (value & 0x00FF00) |
         (value >> 16 & 0x0000FF);
#endif
}

/**
 * Convert a 32 bit integer from network representation to host
 * representation.
 */
[[maybe_unused]] static int32_t ntoh32(int32_t value) {
#if BYTE_ORDER == BIG_ENDIAN
  return value;
#else
  return (value << 24 & 0xFF000000) | (value << 8 & 0x00FF0000) |
         (value >> 8 & 0x0000FF00) | (value >> 24 & 0x000000FF);
#endif
}

/**
 * Convert a 32 bit integer from network representation to host
 * representation.
 */
static uint32_t ntoh32(uint32_t value) {
#if BYTE_ORDER == BIG_ENDIAN
  return value;
#else
  return (value << 24 & 0xFF000000) | (value << 8 & 0x00FF0000) |
         (value >> 8 & 0x0000FF00) | (value >> 24 & 0x000000FF);
#endif
}

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
                                          std::vector<uint8_t> const &data) {
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
 * The data structure of an Omnik 0x10/0x80 message.
 */
#pragma pack(push, 1)
typedef struct {
  char serial_number[16];
} omnik_message_10_80_t;
#pragma pack(pop)

/**
 * @see the header file.
 */
void OmnikInverter::omnik_message_10_80(std::vector<uint8_t> const &data) {
  const std::size_t expected_size = sizeof(omnik_message_10_80_t);
  const std::size_t actual_size = data.size();
  if (actual_size != expected_size) {
    ESP_LOGE(TAG,
             "Received not the correct number of bytes: excpected=%d actual=%d",
             expected_size, actual_size);
    return;
  }

  const omnik_message_10_80_t *message =
      reinterpret_cast<const omnik_message_10_80_t *>(&data[0]);

  serial_device_number_text_sensor_->publish_state(esphome::omnik_base::trim(
      std::string(message->serial_number, sizeof(message->serial_number))));
}

/**
 * The data structure of an Omnik 0x10/0x81 message.
 */
#pragma pack(push, 1)
typedef struct {
  uint8_t status;
} omnik_message_10_81_t;
#pragma pack(pop)

/**
 * @see the header file.
 */
void OmnikInverter::omnik_message_10_81(std::vector<uint8_t> const &data) {
  const std::size_t expected_size = sizeof(omnik_message_10_81_t);
  const std::size_t actual_size = data.size();
  if (actual_size != expected_size) {
    ESP_LOGE(TAG,
             "Received not the correct number of bytes: excpected=%d actual=%d",
             expected_size, actual_size);
    return;
  }

  const omnik_message_10_81_t *message =
      reinterpret_cast<const omnik_message_10_81_t *>(&data[0]);

  status_10_81_text_sensor_->publish_state(omnik_base::to_hex(message->status));
}

/**
 * The data structure of an Omnik 0x10/0x84 message.
 */
#pragma pack(push, 1)
typedef struct {
  uint8_t status;
} omnik_message_10_84_t;
#pragma pack(pop)

/**
 * @see the header file.
 */
void OmnikInverter::omnik_message_10_84(std::vector<uint8_t> const &data) {
  const std::size_t expected_size = sizeof(omnik_message_10_84_t);
  const std::size_t actual_size = data.size();
  if (actual_size != expected_size) {
    ESP_LOGE(TAG,
             "Received not the correct number of bytes: excpected=%d actual=%d",
             expected_size, actual_size);
    return;
  }

  const omnik_message_10_84_t *message =
      reinterpret_cast<const omnik_message_10_84_t *>(&data[0]);

  status_10_84_text_sensor_->publish_state(omnik_base::to_hex(message->status));
}

/**
 * The data structure of an Omnik 0x11/0x83 message.
 */
#pragma pack(push, 1)
typedef struct {
  uint8_t nr_of_phases;
  char rated_power[6];
  char country[2];
  uint32_t firmware_version_main : 24;
  uint32_t firmware_version_slave;
  char inverter_model[12];
  char brand[16];
  char serial_number[16];
  char message_11_83_bytes_60_77[17];
} omnik_message_11_83_t;
#pragma pack(pop)

/**
 * @see the header file.
 */
void OmnikInverter::omnik_message_11_83(std::vector<uint8_t> const &data) {
  const std::size_t expected_size = sizeof(omnik_message_11_83_t);
  const std::size_t actual_size = data.size();
  if (actual_size != expected_size) {
    ESP_LOGE(TAG,
             "Received not the correct number of bytes: excpected=%d actual=%d",
             expected_size, actual_size);
    return;
  }

  const omnik_message_11_83_t *message =
      reinterpret_cast<const omnik_message_11_83_t *>(&data[0]);

  nr_of_phases_text_sensor_->publish_state(
      esphome::omnik_base::trim(std::to_string(message->nr_of_phases)));
  rated_power_text_sensor_->publish_state(esphome::omnik_base::trim(
      std::string(message->rated_power, sizeof(message->rated_power))));
  country_text_sensor_->publish_state(esphome::omnik_base::trim(
      std::string(message->country, sizeof(message->country))));
  firmware_version_main_text_sensor_->publish_state(
      to_version(ntoh24(message->firmware_version_main)));
  firmware_version_slave_text_sensor_->publish_state(
      to_version(ntoh32(message->firmware_version_slave)));
  inverter_model_text_sensor_->publish_state(esphome::omnik_base::trim(
      std::string(message->inverter_model, sizeof(message->inverter_model))));
  brand_text_sensor_->publish_state(esphome::omnik_base::trim(
      std::string(message->brand, sizeof(message->brand))));
  serial_device_number_text_sensor_->publish_state(esphome::omnik_base::trim(
      std::string(message->serial_number, sizeof(message->serial_number))));
  message_11_83_bytes_60_77_text_sensor_->publish_state(
      esphome::omnik_base::trim(
          std::string(message->message_11_83_bytes_60_77,
                      sizeof(message->message_11_83_bytes_60_77))));
}

/**
 * The data structure of an Omnik 0x11/0x90 message.
 */
#pragma pack(push, 1)
typedef struct {
  int16_t temperature;
  uint16_t pv1_voltage;
  uint16_t pv2_voltage;
  uint16_t pv3_voltage;
  uint16_t pv1_current;
  uint16_t pv2_current;
  uint16_t pv3_current;
  uint16_t r_current;
  uint16_t s_current;
  uint16_t t_current;
  uint16_t r_voltage;
  uint16_t s_voltage;
  uint16_t t_voltage;
  uint16_t r_frequency;
  uint16_t r_power;
  uint16_t s_frequency;
  uint16_t s_power;
  uint16_t t_frequency;
  uint16_t t_power;
  uint16_t energy_today;
  uint32_t energy_total;
  uint32_t hours_total;
  uint16_t run_state;
  uint16_t grid_voltage_fault_value;
  uint16_t grid_frequency_fault_value;
  uint16_t grid_impedance_fault_value;
  uint16_t temperature_fault;
  uint16_t pv_voltage_fault;
  uint16_t gfci_current_fault;
  uint32_t error_message_binary_index;
  char main_firmware_version[20];
  char slave_firmware_version[20];
} omnik_message_11_90_t;
#pragma pack(pop)

/**
 * @see the header file.
 */
void OmnikInverter::omnik_message_11_90(std::vector<uint8_t> const &data) {
  const std::size_t expected_size = sizeof(omnik_message_11_90_t);
  const std::size_t actual_size = data.size();
  if (actual_size != expected_size) {
    ESP_LOGE(TAG,
             "Received not the correct number of bytes: excpected=%d actual=%d",
             expected_size, actual_size);
    return;
  }

  const omnik_message_11_90_t *message =
      reinterpret_cast<const omnik_message_11_90_t *>(&data[0]);

  temperature_sensor_->publish_state(ntoh16(message->temperature) / 10.0);
  pv1_voltage_sensor_->publish_state(ntoh16(message->pv1_voltage) / 10.0);
  pv2_voltage_sensor_->publish_state(ntoh16(message->pv2_voltage) / 10.0);
  pv3_voltage_sensor_->publish_state(ntoh16(message->pv3_voltage) / 10.0);
  pv1_current_sensor_->publish_state(ntoh16(message->pv1_current) / 10.0);
  pv2_current_sensor_->publish_state(ntoh16(message->pv2_current) / 10.0);
  pv3_current_sensor_->publish_state(ntoh16(message->pv3_current) / 10.0);
  r_current_sensor_->publish_state(ntoh16(message->r_current) / 10.0);
  s_current_sensor_->publish_state(ntoh16(message->s_current) / 10.0);
  t_current_sensor_->publish_state(ntoh16(message->t_current) / 10.0);
  r_voltage_sensor_->publish_state(ntoh16(message->r_voltage) / 10.0);
  s_voltage_sensor_->publish_state(ntoh16(message->s_voltage) / 10.0);
  t_voltage_sensor_->publish_state(ntoh16(message->t_voltage) / 10.0);
  r_frequency_sensor_->publish_state(ntoh16(message->r_frequency) / 100.0);
  r_power_sensor_->publish_state(ntoh16(message->r_power) / 1000.0);
  s_frequency_sensor_->publish_state(ntoh16(message->s_frequency) / 100.0);
  s_power_sensor_->publish_state(ntoh16(message->s_power) / 1000.0);
  t_frequency_sensor_->publish_state(ntoh16(message->t_frequency) / 100.0);
  t_power_sensor_->publish_state(ntoh16(message->t_power) / 1000.0);
  energy_today_sensor_->publish_state(ntoh16(message->energy_today) / 100.0);
  energy_total_sensor_->publish_state(ntoh32(message->energy_total) / 10.0);
  hours_total_sensor_->publish_state(ntoh32(message->hours_total));
  run_state_text_sensor_->publish_state(
      to_run_state(ntoh16(message->run_state)));
  grid_voltage_fault_value_sensor_->publish_state(
      ntoh16(message->grid_voltage_fault_value) / 10.0);
  grid_frequency_fault_value_sensor_->publish_state(
      ntoh16(message->grid_frequency_fault_value) / 100.0);
  grid_impedance_fault_value_sensor_->publish_state(
      ntoh16(message->grid_impedance_fault_value) / 1000.0);
  temperature_fault_sensor_->publish_state(ntoh16(message->temperature_fault) /
                                           10.0);
  pv_voltage_fault_sensor_->publish_state(ntoh16(message->pv_voltage_fault) /
                                          10.0);
  gfci_current_fault_sensor_->publish_state(
      ntoh16(message->gfci_current_fault) / 1000.0);
  error_message_binary_index_text_sensor_->publish_state(
      std::bitset<32>(ntoh32(message->error_message_binary_index)).to_string());
  std::string main_firmware_version = esphome::omnik_base::trim(std::string(
      message->main_firmware_version, sizeof(message->main_firmware_version)));
  if (!main_firmware_version.empty() && main_firmware_version[0] != '\0') {
    firmware_version_main_text_sensor_->publish_state(main_firmware_version);
  }
  std::string slave_firmware_version = esphome::omnik_base::trim(
      std::string(message->slave_firmware_version,
                  sizeof(message->slave_firmware_version)));
  if (!slave_firmware_version.empty() && slave_firmware_version[0] != '\0') {
    firmware_version_slave_text_sensor_->publish_state(slave_firmware_version);
  }
}

/**
 * The data structure of an Omnik 0x11/0xC3 message.
 */
#pragma pack(push, 1)
typedef struct {
  uint8_t nr_of_alarms;
} omnik_message_11_c3_t;
#pragma pack(pop)

/**
 * @see the header file.
 */
void OmnikInverter::omnik_message_11_c3(std::vector<uint8_t> const &data) {
  const std::size_t expected_size = sizeof(omnik_message_11_c3_t);
  const std::size_t actual_size = data.size();
  if (actual_size != expected_size) {
    ESP_LOGE(TAG,
             "Received not the correct number of bytes: excpected=%d actual=%d",
             expected_size, actual_size);
    nr_of_alarms_sensor_->publish_state(actual_size); // Fix this!
    return;
  }

  const omnik_message_11_c3_t *message =
      reinterpret_cast<const omnik_message_11_c3_t *>(&data[0]);

  nr_of_alarms_sensor_->publish_state(message->nr_of_alarms);
}

/**
 * The data structure of an Omnik 0x12/0xC0 message.
 */
#pragma pack(push, 1)
typedef struct {
  uint8_t status;
} omnik_message_12_c0_t;
#pragma pack(pop)

/**
 * @see the header file.
 */
void OmnikInverter::omnik_message_12_c0(std::vector<uint8_t> const &data) {
  const std::size_t expected_size = sizeof(omnik_message_12_c0_t);
  const std::size_t actual_size = data.size();
  if (actual_size != expected_size) {
    ESP_LOGE(TAG,
             "Received not the correct number of bytes: excpected=%d actual=%d",
             expected_size, actual_size);
    return;
  }

  const omnik_message_12_c0_t *message =
      reinterpret_cast<const omnik_message_12_c0_t *>(&data[0]);

  status_12_c0_text_sensor_->publish_state(omnik_base::to_hex(message->status));
}

/**
 * The data structure of an Omnik 0x12/0xC1 message.
 */
#pragma pack(push, 1)
typedef struct {
  uint8_t status;
} omnik_message_12_c1_t;
#pragma pack(pop)

/**
 * @see the header file.
 */
void OmnikInverter::omnik_message_12_c1(std::vector<uint8_t> const &data) {
  const std::size_t expected_size = sizeof(omnik_message_12_c1_t);
  const std::size_t actual_size = data.size();
  if (actual_size != expected_size) {
    ESP_LOGE(TAG,
             "Received not the correct number of bytes: excpected=%d actual=%d",
             expected_size, actual_size);
    return;
  }

  const omnik_message_12_c1_t *message =
      reinterpret_cast<const omnik_message_12_c1_t *>(&data[0]);

  status_12_c1_text_sensor_->publish_state(omnik_base::to_hex(message->status));
}

} // namespace omnik_inverter
} // namespace esphome
