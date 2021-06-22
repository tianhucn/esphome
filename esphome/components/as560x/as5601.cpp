#include "esphome/core/log.h"
#include "as5601.h"

namespace esphome {
namespace as560x {


/** AS5601 Register Map **/
static const uint8_t REGISTER_ABN = 0x09;
static const uint8_t REGISTER_PUSHTHR = 0x0A;
/** **/

static const char *TAG = "as5601";

void AS5601Component::dump_config() {
    ESP_LOGCONFIG(TAG, "Setting up device...");
    ESP_LOGCONFIG(TAG, "  AB Resolution: %d", this->ab_resolution_);
    ESP_LOGCONFIG(TAG, "  Push Threshold: %d", this->push_threshold_);
    LOG_AS560X(this);
};


void AS5601Component::setup() {
    //if (this->is_device_online()) return this->mark_failed();
    AS560XComponent::common_setup();
    this->write_ab_resolution(this->ab_resolution_);
    this->write_push_threshold(this->push_threshold_);
};


void AS5601Component::write_ab_resolution(uint16_t positions) {
    ESP_LOGI(TAG, "Configuring AB positions from %d to %d", this->ab_resolution_, positions);
    // Taken from https://github.com/bitfasching/AS5601/blob/master/AS5601.h
    char power = -1;
    while ( ( 1 << ++power ) < this->ab_resolution_);
    if(!this->write_byte(REGISTER_ABN, power-3)) return;
    this->ab_resolution_ = positions; // Do not set if write failed
    ESP_LOGI(TAG, "Configured AB positions %d", this->ab_resolution_);
    return;
};

void AS5601Component::write_push_threshold(uint16_t threshold) {
    return;
};


}  // namespace as560x
}  // namespace esphome