#pragma once

#include "esphome/components/omnik_base/omnik_base.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/text_sensor/text_sensor.h"

namespace esphome {
namespace omnik_logger {

/**
 * This class is responsible for processing the messages received from the Omnik
 * logger.
 */
class OmnikLogger : public omnik_base::OmnikBase {
public:
  /**
   * Log the current configuration.
   */
  void dump_config() override;

  SUB_TEXT_SENSOR(connection_number)
  SUB_TEXT_SENSOR(ip_address)
  SUB_TEXT_SENSOR(serial_device_number)

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
   * Process an Omnik 0x10/0x01 message.
   *
   * @param data The data of the message.
   * 		 data[0-15]: Inverter serial number
   * 		 data[16]:   Connected inverter number
   */
  void omnik_message_10_01(std::vector<uint8_t> const &data);

  /**
   * Process an Omnik 0x12/0x40 message.
   *
   * @param data The data of the message.
   * 		 data[0-15]: Device serial number (\0 terminated)
   */
  void omnik_message_12_40(std::vector<uint8_t> const &data);

  /**
   * Process an Omnik 0x12/0x41 message.
   *
   * @param data The data of the message.
   * 		 data[0-15]: IP address (\0 terminated)
   */
  void omnik_message_12_41(std::vector<uint8_t> const &data);
};

} // namespace omnik_logger
} // namespace esphome
