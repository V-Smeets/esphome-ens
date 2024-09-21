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
                                        std::vector<uint8_t> const &data) {
  switch (OMNIK_MESSAGE_ID(control_code, function_code)) {
  case OMNIK_MESSAGE_ID(0x10, 0x01):
    omnik_message_10_01(data);
    break;

  case OMNIK_MESSAGE_ID(0x12, 0x40):
    omnik_message_12_40(data);
    break;

  case OMNIK_MESSAGE_ID(0x12, 0x41):
    omnik_message_12_41(data);
    break;

  case OMNIK_MESSAGE_ID(0x10, 0x00):
  case OMNIK_MESSAGE_ID(0x10, 0x04):
  case OMNIK_MESSAGE_ID(0x11, 0x03):
  case OMNIK_MESSAGE_ID(0x11, 0x10):
  case OMNIK_MESSAGE_ID(0x11, 0x43):
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
 * The data structure of an Omnik 0x10/0x01 message.
 */
typedef struct {
  char serial_number[16];
  uint8_t connection_number;
} omnik_message_10_01_t;

/**
 * @see the header file.
 */
void OmnikLogger::omnik_message_10_01(std::vector<uint8_t> const &data) {
  const std::size_t expected_size = sizeof(omnik_message_10_01_t);
  const std::size_t actual_size = data.size();
  if (actual_size != expected_size) {
    ESP_LOGE(TAG,
             "Received not the correct number of bytes: excpected=%d actual=%d",
             expected_size, actual_size);
    return;
  }

  const omnik_message_10_01_t *message =
      reinterpret_cast<const omnik_message_10_01_t *>(&data[0]);

  ESP_LOGI(TAG, "Inverter serial number: %s",
           std::string(message->serial_number, sizeof(message->serial_number))
               .c_str());
  connection_number_text_sensor_->publish_state(
      std::to_string(message->connection_number));
}

/*
 * The data structure of an Omnik 0x12/0x40 message.
 */
typedef struct {
  char serial_number[16];
} omnik_message_12_40_t;

/**
 * @see the header file.
 */
void OmnikLogger::omnik_message_12_40(std::vector<uint8_t> const &data) {
  const std::size_t expected_size = sizeof(omnik_message_12_40_t);
  const std::size_t actual_size = data.size();
  if (actual_size != expected_size) {
    ESP_LOGE(TAG,
             "Received not the correct number of bytes: excpected=%d actual=%d",
             expected_size, actual_size);
    return;
  }

  const omnik_message_12_40_t *message =
      reinterpret_cast<const omnik_message_12_40_t *>(&data[0]);

  serial_device_number_text_sensor_->publish_state(
      std::string(message->serial_number, sizeof(message->serial_number)));
}

/**
 * The data structure of an Omnik 0x12/0x41 message.
 */
typedef struct {
  char ip_address[16];
} omnik_message_12_41_t;

/**
 * @see the header file.
 */
void OmnikLogger::omnik_message_12_41(std::vector<uint8_t> const &data) {
  const std::size_t expected_size = sizeof(omnik_message_12_41_t);
  const std::size_t actual_size = data.size();
  if (actual_size != expected_size) {
    ESP_LOGE(TAG,
             "Received not the correct number of bytes: excpected=%d actual=%d",
             expected_size, actual_size);
    return;
  }

  const omnik_message_12_41_t *message =
      reinterpret_cast<const omnik_message_12_41_t *>(&data[0]);

  ip_address_text_sensor_->publish_state(
      std::string(message->ip_address, sizeof(message->ip_address)));
}

} // namespace omnik_logger
} // namespace esphome
