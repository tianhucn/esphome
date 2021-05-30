#include "as5600.h"
#include "esphome/core/log.h"

namespace esphome {
namespace as5601 {

static const char *TAG = "as5600";


void AS5600::dump_config() {
    ESP_LOGCONFIG(TAG, "AS5600:");
    ESP_LOGCONFIG(TAG, "  AB positions: %d", this->_ab_positions);
    ESP_LOGCONFIG(TAG, "  Zero position: %d", this->_zero_position);
    LOG_BINARY_SENSOR("  ", "Magnet Field Presence", this->presence_binarysensor);
    LOG_SENSOR("  ", "Magnet Field Orientation", this->orientation_sensor);
    LOG_SENSOR("  ", "Magnet Field Magnitude", this->magnitude_sensor);
}


void AS5600::setup() {
    ESP_LOGCONFIG(TAG, "Setting up AS5600...");

    int16_t status = this->read_status_byte();
    if (!status) {
        ESP_LOGE(TAG, "Unable to get status from AS5600");
        this->mark_failed();
    }
    
    ESP_LOGV(TAG, "Got AS5600 status %d", status);
    this->write_ab_resolution(this->_ab_positions);
    this->write_zero_position(this->_zero_position);

    // Initial sensor publishing
    if (this->magnitude_sensor != nullptr) this->magnitude_sensor->publish_state(magnitude());
    if (this->orientation_sensor != nullptr) this->orientation_sensor->publish_state(angle());
}


void AS5600::loop() {
    if (this->presence_binarysensor != nullptr) this->presence_binarysensor->publish_state(presence());

    // Publish any changes to the magnitude sensor if it is enabled
    if (this->magnitude_sensor != nullptr) {
        uint data = magnitude();
        if (data != this->magnitude_sensor->get_raw_state()) this->magnitude_sensor->publish_state(data);
    }

    // Publish any changes to the orientation sensor if it is enabled
    if (this->orientation_sensor != nullptr) {
        uint data = angle();
        if (data != this->orientation_sensor->get_raw_state()) this->orientation_sensor->publish_state(data);
    }
}


}  // namespace as5601
}  // namespace esphome