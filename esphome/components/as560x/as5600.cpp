#include "esphome/core/log.h"
#include "as5600.h"

namespace esphome {
namespace as560x {

/** AS5600 Register Map **/
static const uint16_t REGISTER_MPOS = 0x03;
static const uint16_t REGISTER_MANG = 0x05;
/** **/

static const char *TAG = "AS5600";

void AS5600Component::dump_config() {
    ESP_LOGCONFIG(TAG, "Setting up device:");
    ESP_LOGCONFIG(TAG, "  Angle Stop Position: %d", this->stop_position_);
    ESP_LOGCONFIG(TAG, "  Maximum Angle: %d", this->maximum_angle_);
    LOG_AS560X(this);
};


void AS5600Component::setup() {
    //if (this->is_device_online()) return this->mark_failed();
    AS560XComponent::common_setup();
    this->write_stop_position(this->stop_position_);
    this->write_maximum_angle(this->maximum_angle_);
};


void AS5600Component::write_stop_position(uint16_t position) {
    /* write to device here */
    this->stop_position_ = position; // Do not set if write failed
    return;
};

void AS5600Component::write_maximum_angle(uint16_t angle) {
    /* write to device here */
    this->maximum_angle_ = angle; // Do not set if write failed
    return;
};


}  // namespace as560x
}  // namespace esphome