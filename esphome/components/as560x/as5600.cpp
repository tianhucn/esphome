#include "as560x.h"
#include "esphome/core/log.h"

namespace esphome {
namespace as560x {

/** AS5600 Register Map **/
static const uint16_t REGISTER_MPOS = 0x03;
static const uint16_t REGISTER_MANG = 0x05;
/** **/

static const char *TAG = "as560x";

void AS5600Component::dump_config() {
    ESP_LOGCONFIG(TAG, "AS5600:");
    ESP_LOGCONFIG(TAG, "  Angle Stop Position: %d", this->_stop_position);
    ESP_LOGCONFIG(TAG, "  Maximum Angle: %d", this->_maximum_angle);
    AS560XComponent::dump_common_sensor_config();
};


void AS5600Component::setup() {
    //if (this->is_device_online()) return this->mark_failed();
    this->write_stop_position(this->_stop_position);
    this->write_maximum_angle(this->_maximum_angle);
    AS560XComponent::continue_common_setup();
};


void AS5600Component::write_stop_position(uint16_t position) {
    /* write to device here */
    this->_stop_position = position; // Do not set if write failed
    delayMicroseconds(1000); // Wait 1ms according to datasheet
    return;
};

void AS5600Component::write_maximum_angle(uint16_t angle) {
    /* write to device here */
    this->_maximum_angle = angle; // Do not set if write failed
    delayMicroseconds(1000); // Wait 1ms according to datasheet
    return;
};


}  // namespace as560x
}  // namespace esphome