#pragma once

#include "esphome/components/sensor/sensor.h"
#include "esphome/components/text_sensor/text_sensor.h"
#include "esphome/components/uart/uart.h"
#include "esphome/core/bytebuffer.h"

#define OMNIK_MESSAGE_ID(control_code, function_code)                          \
  ((control_code << 8) + function_code)

namespace esphome {
namespace omnik_base {

/**
 * The base class for the Omnik components. This class is responsible for
 * reciving the bytes from the UART and checking the checksum. the processing of
 * the specific messages is then delegated to the child class(es).
 */
class OmnikBase : public uart::UARTDevice, public Component {

  /**
   * Check and do what has to be done.
   */
  void loop() override;

protected:
  /**
   * process an Omnik message.
   *
   * @param control_code The control code.
   * @param function_code The function code.
   * @param buffer The data of the message.
   */
  virtual void process_omnik_message(uint8_t control_code,
                                     uint8_t function_code, ByteBuffer &buffer);

private:
  // The time (in milliseconds) at which the last byte has been received.
  uint32_t last_received_time_{0};
  // The buffer with the bytes that already have been reiceived.
  std::vector<uint8_t> rx_buffer_;

  /**
   * Process the Omnik message in the buffer.
   *
   * Try to process the Omnik message in the buffer. The logic is the same as in
   * the function is_buffer_processed() except then for Omnik messages.  An
   * Omnik message has the following format:
   * * buffer[0 .. 1] Start bytes (value 0x3A).
   * * buffer[2 .. 3] Sender address.
   * * buffer[4 .. 5] Receiver address.
   * * buffer[6] Control code.
   * * buffer[7] Function code.
   * * buffer[8] Data size.
   * * buffer[9 .. 9 + Data size - 1] Data.
   * * buffer[9 + Data size .. 9 + Data size + 1] Check sum.
   *
   * @param buffer The buffer with the bytes of the message.
   * @return True in case the buffer (message) was processed, False otherwise.
   */
  bool is_omnik_message_processed(std::vector<uint8_t> const &buffer);

  /**
   * Process the Modbus message in the buffer.
   *
   * Try to process the Modbus message in the buffer. The logic is the same as
   * in the function is_buffer_processed() except then for Modbus messages.
   *
   * @param buffer The buffer with the bytes of the message.
   * @return True in case the buffer (message) was processed, False otherwise.
   */
  bool is_modbus_message_processed(std::vector<uint8_t> const &buffer);

  /**
   * Process the message in the buffer.
   *
   * Try to process the message in the buffer. The buffer can contain garbage, a
   * partly recognized messages or a complete message. In case the buffer
   * contains a complete recognized message, then it is processed and this
   * function will return True. Otherwise, in case the message can't be
   * processed, then it will return False.
   *
   * @param buffer The buffer with the bytes of the message.
   * @return True in case the buffer (message) was processed, False otherwise.
   */
  bool is_buffer_processed(std::vector<uint8_t> const &buffer);
};

/**
 * Log the base configuration of an OmnikBase.
 */
void dump_config(const char *const tag, std::string prefix,
                 OmnikBase *omnikBase);

/**
 * Log the configuration of a Sensor.
 */
void dump_config(const char *const tag, std::string prefix,
                 sensor::Sensor *sensor);

/**
 * Log the configuration of a TextSensor.
 */
void dump_config(const char *const tag, std::string prefix,
                 text_sensor::TextSensor *text_sensor);

/**
 * Convert a byte to a hexadecimal representation.
 *
 * @param byte The byte value.
 * @return A string with a hexadecimal representation of the byte.
 */
std::string to_hex(uint8_t byte);

/**
 * Convert the byte buffer to a hexadecimal representation.
 *
 * @param buffer The buffer with the bytes.
 * @param length The length of the buffer.
 * @param separator The separator to use between the bytes.
 * @return A string with a hexadecimal representation of the buffer.
 */
std::string to_hex(const uint8_t buffer[], size_t length, char separator);

/**
 * Convert the byte buffer to a hexadecimal representation.
 *
 * @param buffer The buffer with the bytes.
 * @param separator The separator to use between the bytes.
 * @return A string with a hexadecimal representation of the buffer.
 */
std::string to_hex(std::vector<uint8_t> const &buffer, char separator);

/**
 * Convert the data bytes to an ASCII string.
 */
std::string to_string(std::vector<uint8_t> const &buffer);

} // namespace omnik_base
} // namespace esphome
