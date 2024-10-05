#include "omnik_logger.h"

namespace esphome {
namespace omnik_logger {

// Tag that is used for log messages.
static const char *const TAG = "omnik_logger";

/**
 * @see the header file.
 */
void OmnikLogger::dump_config() {
  ESP_LOGCONFIG(TAG, "OmnikLogger:");
  omnik_base::dump_config(TAG, "  ", this);
  ESP_LOGCONFIG(TAG, "  connection_number:");
  omnik_base::dump_config(TAG, "    ", connection_number_text_sensor_);
  ESP_LOGCONFIG(TAG, "  ip_address:");
  omnik_base::dump_config(TAG, "    ", ip_address_text_sensor_);
  ESP_LOGCONFIG(TAG, "  serial_device_number:");
  omnik_base::dump_config(TAG, "    ", serial_device_number_text_sensor_);
}

/**
 * @see the header file.
 */
void OmnikLogger::process_omnik_message(uint8_t control_code,
                                        uint8_t function_code,
                                        ByteBuffer &buffer) {
  switch (OMNIK_MESSAGE_ID(control_code, function_code)) {
  case OMNIK_MESSAGE_ID(0x10, 0x01):
    omnik_message_10_01(buffer);
    break;

  case OMNIK_MESSAGE_ID(0x12, 0x40):
    omnik_message_12_40(buffer);
    break;

  case OMNIK_MESSAGE_ID(0x12, 0x41):
    omnik_message_12_41(buffer);
    break;

  case OMNIK_MESSAGE_ID(0x10, 0x00):
  case OMNIK_MESSAGE_ID(0x10, 0x04):
  case OMNIK_MESSAGE_ID(0x11, 0x03):
  case OMNIK_MESSAGE_ID(0x11, 0x10):
  case OMNIK_MESSAGE_ID(0x11, 0x43):
    omnik_message_no_data(buffer);
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
void OmnikLogger::omnik_message_10_01(ByteBuffer &buffer) {
  std::string serial_number = omnik_base::to_string(buffer.get_vector(16));
  ESP_LOGI(TAG, "Inverter serial number: %s", serial_number.c_str());

  uint8_t connection_number = buffer.get_uint8();
  connection_number_text_sensor_->publish_state(
      std::to_string(connection_number));
}

/**
 * @see the header file.
 */
void OmnikLogger::omnik_message_12_40(ByteBuffer &buffer) {
  std::string serial_number = omnik_base::to_string(buffer.get_vector(16));
  serial_device_number_text_sensor_->publish_state(serial_number);
}

/**
 * @see the header file.
 */
void OmnikLogger::omnik_message_12_41(ByteBuffer &buffer) {
  std::string ip_address = omnik_base::to_string(buffer.get_vector(16));
  ip_address_text_sensor_->publish_state(ip_address);
}

} // namespace omnik_logger
} // namespace esphome
