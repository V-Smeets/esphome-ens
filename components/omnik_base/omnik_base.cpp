#include "esphome/components/omnik_base/omnik_base.h"

namespace esphome {
namespace omnik_base {

// Tag that is used for log messages.
static const char *const LOG_TAG = "omnik_base";
// Timeout for receiving the bytes of the same message from the uart
// (in milliseconds).
static const uint32_t RECEIVE_TIMEOUT = 50;

/**
 * Convert an EntityCategory to a string.
 */
static std::string to_string(EntityCategory entity_category) {
  switch (entity_category) {
  case ENTITY_CATEGORY_NONE:
    return "None";
  case ENTITY_CATEGORY_CONFIG:
    return "Config";
  case ENTITY_CATEGORY_DIAGNOSTIC:
    return "Diagnostic";
  default:
    return "<Illegal Value>";
  }
}

/**
 * Log a name with a value, but only in case they are defined (not empty).
 */
static void dump_config(const char *const tag, std::string prefix,
                        std::string name, int value) {
  if (name.empty()) {
    return;
  }
  ESP_LOGCONFIG(tag, "%s%s: %d", prefix.c_str(), name.c_str(), value);
}

/**
 * Log a name with a value, but only in case they are defined (not empty).
 */
static void dump_config(const char *const tag, std::string prefix,
                        std::string name, std::string value) {
  if (name.empty() or value.empty()) {
    return;
  }
  ESP_LOGCONFIG(tag, "%s%s: %s", prefix.c_str(), name.c_str(), value.c_str());
}

/**
 * Log the configuration of an EntityBase.
 */
static void dump_config(const char *const tag, std::string prefix,
                        EntityBase *entity_base) {
  if (entity_base == nullptr) {
    return;
  }
  dump_config(tag, prefix, "Name", entity_base->get_name());
  dump_config(tag, prefix, "Entity Category",
              to_string(entity_base->get_entity_category()));
  dump_config(tag, prefix, "Icon", entity_base->get_icon());
}

/**
 * Log the configuration of an EntityBase_DeviceClass.
 */
static void dump_config(const char *const tag, std::string prefix,
                        EntityBase_DeviceClass *entity_base_device_class) {
  if (entity_base_device_class == nullptr) {
    return;
  }
  dump_config(tag, prefix, "Device Class",
              entity_base_device_class->get_device_class());
}

/**
 * Log the configuration of an EntityBase_UnitOfMeasurement.
 */
static void
dump_config(const char *const tag, std::string prefix,
            EntityBase_UnitOfMeasurement *entity_base_unit_of_measurement) {
  if (entity_base_unit_of_measurement == nullptr) {
    return;
  }
  dump_config(tag, prefix, "Unit of Measurement",
              entity_base_unit_of_measurement->get_unit_of_measurement());
}

/**
 * @see the header file.
 */
void OmnikBase::loop() {
  const uint32_t now = millis();

  // Discard all received data in case the next byte isn't received within a
  // predefined timeout period.
  if (this->last_received_time_ + RECEIVE_TIMEOUT < now) {
    this->rx_buffer_.clear();
    this->last_received_time_ = now;
  }

  // Process the next byte that is received. One byte at a time ensures a
  // minimal blocking time.
  if (this->available()) {
    uint8_t byte;

    this->last_received_time_ = now;
    this->read_byte(&byte);
    rx_buffer_.push_back(byte);

    if (is_buffer_processed(rx_buffer_)) {
      // In case this buffer has correctly been processed, then we can clear
      // the buffer so that we can start processing the next message.
      this->rx_buffer_.clear();
    }
  }
}

/**
 * @see the header file.
 */
bool OmnikBase::is_omnik_message_processed(std::vector<uint8_t> const &buffer) {
  // Check the start bytes.
  if (buffer.size() < 2)
    return false;
  if (buffer[0] != 0x3A || buffer[1] != 0x3A)
    return false;

  // Check the header bytes.
  if (buffer.size() < 11)
    return false;
  uint8_t control_code = buffer[6];
  uint8_t function_code = buffer[7];
  uint8_t data_size = buffer[8];

  // Get the expected check sum.
  if (buffer.size() < (9 + data_size + 2))
    return false;
  uint16_t expected_checksum =
      (buffer[9 + data_size] << 8) + buffer[9 + data_size + 1];

  // Check the checksum.
  uint16_t actual_checksum = 0;
  for (std::size_t index = 0; index < (9 + data_size); index++)
    actual_checksum += buffer[index];
  if (actual_checksum != expected_checksum) {
    ESP_LOGW(LOG_TAG, "Checksum mismatch: actual=0x%04X expected=0x%04X",
             actual_checksum, expected_checksum);
    ESP_LOGI(LOG_TAG, "Received bytes: %s", to_hex(buffer, ':'));
    return true;
  }

  std::vector<uint8_t> data(buffer.begin() + 9, buffer.begin() + 9 + data_size);
  process_omnik_message(control_code, function_code, data);

  return true;
}

/**
 * @see the header file.
 */
bool OmnikBase::is_modbus_message_processed(
    std::vector<uint8_t> const &buffer) {
  return false;
}

/**
 * @see the header file.
 */
bool OmnikBase::is_buffer_processed(std::vector<uint8_t> const &buffer) {
  return is_omnik_message_processed(buffer) ||
         is_modbus_message_processed(buffer);
}

/**
 * @see the header file.
 */
void dump_config(const char *const tag, std::string prefix,
                 OmnikBase *omnikBase) {}

/**
 * @see the header file.
 */
void dump_config(const char *const tag, std::string prefix,
                 sensor::Sensor *sensor) {
  dump_config(tag, prefix, (EntityBase *)sensor);
  dump_config(tag, prefix, (EntityBase_DeviceClass *)sensor);
  dump_config(tag, prefix, (EntityBase_UnitOfMeasurement *)sensor);
  dump_config(tag, prefix, "State Class",
              state_class_to_string(sensor->get_state_class()));
  dump_config(tag, prefix, "Accuracy Decimals",
              sensor->get_accuracy_decimals());
  dump_config(tag, prefix, "Unique ID", sensor->unique_id());
  dump_config(tag, prefix, "Force Update",
              sensor->get_force_update() ? "true" : "false");
}

/**
 * @see the header file.
 */
void dump_config(const char *const tag, std::string prefix,
                 text_sensor::TextSensor *text_sensor) {
  if (text_sensor == nullptr) {
    ESP_LOGCONFIG(tag, "%sNot Used", prefix.c_str());
    return;
  }
  dump_config(tag, prefix, (EntityBase *)text_sensor);
  dump_config(tag, prefix, (EntityBase_DeviceClass *)text_sensor);
  dump_config(tag, prefix, "Unique ID", text_sensor->unique_id());
}

/**
 * @see the header file.
 */
std::string to_hex(uint8_t byte) {
  char buffer[5];

  sprintf(buffer, "%02X", byte);
  return buffer;
}

/**
 * @see the header file.
 */
std::string to_hex(const uint8_t buffer[], size_t length, char separator) {
  std::string hex_representation;

  for (size_t i = 0; i < length; i++) {
    if (i > 0) {
      hex_representation += separator;
    }
    hex_representation += to_hex(buffer[i]);
  }
  return hex_representation;
}

/**
 * @see the header file.
 */
std::string to_hex(std::vector<uint8_t> const &buffer, char separator) {
  return to_hex(buffer.data(), buffer.size(), separator);
}

/**
 * @see the header file.
 */
std::string to_string(std::vector<uint8_t> const &data) {
  std::string result;
  result.reserve(data.size());

  std::copy_if(data.begin(), data.end(), std::back_inserter(result),
               [](uint8_t byte) { return byte != 0x00; });
  return result;
}

/**
 * @see the header file.
 */
std::string trim(std::string source) {
  bool last_character_is_space = false;

  std::string::iterator new_begin = source.begin();
  while (new_begin < source.end() && std::isspace(*new_begin)) {
    last_character_is_space = true;
    new_begin++;
  }

  std::string::iterator new_end = new_begin;
  for (std::string::iterator it = new_begin; it < source.end(); it++) {
    if (std::isspace(*it)) {
      if (!last_character_is_space) {
        new_end = it;
      }
      last_character_is_space = true;
    } else {
      last_character_is_space = false;
    }
  }
  if (!last_character_is_space) {
    new_end = source.end();
  }

  return {new_begin, new_end};
}

} // namespace omnik_base
} // namespace esphome
