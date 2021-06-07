#include "as560x.h"
#include "esphome/core/log.h"

namespace esphome {
namespace as560x {


/** AS5601 Register Map **/
static const uint8_t REGISTER_ABN = 0x09;
static const uint8_t REGISTER_PUSHTHR = 0x0A;
/** **/

static const char *TAG = "as560x";

void AS5601Component::dump_config() {
    ESP_LOGCONFIG(TAG, "AS5601:");
    ESP_LOGCONFIG(TAG, "  Zero position: %d", this->_zero_position);
    ESP_LOGCONFIG(TAG, "  AB resolution: %d", this->_ab_resolution);
    AS560XComponent::dump_common_sensor_config();
};


void AS5601Component::setup() {
    //if (this->is_device_online()) return this->mark_failed();
    this->write_ab_resolution(this->_ab_resolution);
    this->write_push_threshold(this->_push_threshold);
    AS560XComponent::continue_common_setup();
};


void AS5601Component::write_ab_resolution(uint16_t positions) {
    // Taken from https://github.com/bitfasching/AS5601/blob/master/AS5601.h
    char power = -1;
    while ( ( 1 << ++power ) < this->_ab_resolution);
    if(!this->write_byte(REGISTER_ABN, power-3)) return;
    this->_ab_resolution = positions; // Do not set if write failed
    delayMicroseconds(1000); // Wait 1ms according to datasheet
    return;
};


void AS5601Component::write_push_threshold(uint16_t threshold) {
    delayMicroseconds(1000); // Wait 1ms according to datasheet
    return;
};

}  // namespace as560x
}  // namespace esphome