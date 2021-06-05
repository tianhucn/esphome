#include "as5600.h"
#include "esphome/core/log.h"

namespace esphome {
namespace as560x {

/** AS5600 Register Map **/
static const uint16_t REGISTER_MPOS = 0x03;
static const uint16_t REGISTER_MANG = 0x05;
/** **/

static const char *TAG = "as5600";

void AS5600Component::dump_config() {
    ESP_LOGCONFIG(TAG, "AS5600:");
    AS560XComponent::dump_common_sensor_config();
};


void AS5600Component::setup() {
    //if (this->device_online()) return this->mark_failed();
    AS560XComponent::continue_common_setup();
};


}  // namespace as560x
}  // namespace esphome