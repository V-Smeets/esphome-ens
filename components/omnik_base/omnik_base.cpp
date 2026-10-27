#include "esphome/components/omnik_base/omnik_base.h"

namespace esphome {
namespace omnik_base {

// Tag that is used for log messages.
static const char *const LOG_TAG = "omnik_base";
// Timeout for receiving the bytes of the same message from the uart
// (in milliseconds).
static const uint32_t RECEIVE_TIMEOUT = 50;

/**
 * Trim spaces from both sides of the string.
 *
 * @param source The string to trim.
 * @result The trimmed string.
 */
static std::string trim(std::string source) {
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

/**
 * @see the header file.
 */
void OmnikBase::loop() {
  const uint32_t now = millis();

  // Discard all received data in case the next byte isn't received within a
  // predefined timeout period.
  if (now - this->last_received_time_ > RECEIVE_TIMEOUT) {
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
    return true;

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

  bytebuffer::ByteBuffer byte_buffer = bytebuffer::ByteBuffer::wrap(
      {buffer.begin() + 9, buffer.begin() + 9 + data_size}, bytebuffer::BIG);
  process_omnik_message(control_code, function_code, byte_buffer);

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
std::string to_hex(uint8_t byte) {
  char buffer[3];

  snprintf(buffer, sizeof(buffer), "%02X", byte);
  return buffer;
}

/**
 * @see the header file.
 */
std::string to_hex(std::vector<uint8_t> const &buffer, char separator) {
  std::string hex_representation;

  for (auto element_iterator = buffer.cbegin();
       element_iterator != buffer.cend(); element_iterator++) {
    if (element_iterator != buffer.cbegin()) {
      hex_representation += separator;
    }
    hex_representation += to_hex(*element_iterator);
  }
  return hex_representation;
}

/**
 * @see the header file.
 */
std::string to_string(std::vector<uint8_t> const &buffer) {
  std::string result;
  result.reserve(buffer.size());

  std::copy_if(buffer.begin(), buffer.end(), std::back_inserter(result),
               [](uint8_t byte) { return byte != 0x00; });
  return trim(result);
}

} // namespace omnik_base
} // namespace esphome
