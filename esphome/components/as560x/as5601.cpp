#include "as5601.h"
#include "esphome/core/log.h"

namespace esphome {
namespace as560x {


/** AS5601 Register Map **/
static const uint8_t REGISTER_ABN = 0x09;
static const uint8_t REGISTER_PUSHTHR = 0x0A;
/** **/

static const char *TAG = "as5601";

void AS5601Component::dump_config() {
    ESP_LOGCONFIG(TAG, "AS5601:");
    ESP_LOGCONFIG(TAG, "  Zero position: %d", this->_zero_position);
    ESP_LOGCONFIG(TAG, "  AB resolution: %d", this->_ab_resolution);
    AS560XComponent::dump_common_sensor_config();
};


void AS5601Component::setup() {
    //if (this->device_online()) return this->mark_failed();
    this->write_ab_resolution(this->_ab_resolution);
    AS560XComponent::continue_common_setup();
};




}  // namespace as560x
}  // namespace esphome