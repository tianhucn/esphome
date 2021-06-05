#include "as560x.h"
#include "esphome/core/log.h"

namespace esphome {
namespace as560x {

static const char *TAG = "as560x";

/** AS5600 and AS5601 Register Map **/
// Common
static const uint8_t REGISTER_ZMCO = 0x00;
static const uint16_t REGISTER_ZPOS = 0x01;
static const uint16_t REGISTER_CONF = 0x07;
static const uint16_t REGISTER_RAWANGLE = 0x0C;
static const uint16_t REGISTER_ANGLE = 0x0E;
static const uint8_t REGISTER_STATUS = 0x0B;
static const uint8_t REGISTER_AGC = 0x1A;
static const uint16_t REGISTER_MAGNITUDE = 0x1B;
static const uint8_t REGISTER_BURN = 0xFF;

// AS5600
static const uint16_t REGISTER_MPOS = 0x03;
static const uint16_t REGISTER_MANG = 0x05;

// AS5601
static const uint8_t REGISTER_ABN = 0x09;
static const uint8_t REGISTER_PUSHTHR = 0x0A;
/** **/



void AS560XComponent::dump_common_sensor_config() {
    LOG_BINARY_SENSOR("  ", "Magnet Detection Sensor", this->presence_binarysensor);
    LOG_SENSOR("  ", "Magnet Angle Sensor", this->angle_sensor);
    LOG_SENSOR("  ", "Magnet Field Strength Sensor", this->magnitude_sensor);
};

void AS560XComponent::continue_common_setup() {

    // Common device config
    this->write_zero_position(this->_zero_position);

    // Initial sensor publishing
    if (this->presence_binarysensor != nullptr) this->presence_binarysensor->publish_state(presence());
    if (this->magnitude_sensor != nullptr)      this->magnitude_sensor->publish_state(magnitude());
    if (this->angle_sensor != nullptr)    this->angle_sensor->publish_state(angle());
}


void AS560XComponent::loop() {
    if (this->presence_binarysensor != nullptr) this->presence_binarysensor->publish_state(presence());
    
    if (this->magnitude_sensor != nullptr) {
        uint data = magnitude();
        if (data != this->magnitude_sensor->get_raw_state()) this->magnitude_sensor->publish_state(data);
    }
    
    if (this->angle_sensor != nullptr) {
        uint data = angle();
        if (data != this->angle_sensor->get_raw_state()) this->angle_sensor->publish_state(data);
    }
}

bool AS560XComponent::device_online() {
    auto ack = this->read_byte( REGISTER_STATUS );
    return ack ? true : false;
}


void AS560XComponent::write_ab_resolution(uint16_t positions) {
    // Taken from https://github.com/bitfasching/AS5601/blob/master/AS5601.h
    char power = -1;
    while ( ( 1 << ++power ) < this->_ab_resolution);
    if(!this->write_byte(REGISTER_ABN, power-3)) return;
    this->_ab_resolution = positions; // Do not set if write failed
    delayMicroseconds(1000); // Wait 1ms according to datasheet
    return;
};


void AS560XComponent::write_zero_position(uint16_t angle) {
    if (!this->write_byte_16(REGISTER_ZPOS, angle)) return;
    this->_zero_position = angle; // Do not set if write failed
    delayMicroseconds(1000); // Wait 1ms according to datasheet
    return;
};


int16_t AS560XComponent::read_status_byte() {
    auto status = this->read_byte( REGISTER_STATUS );
    return (int16_t)(status ? status.value() : -1);
}

bool AS560XComponent::presence() {
    auto data = this->read_byte( REGISTER_STATUS );
    if (!data) return false;
    return bitRead( data.value(), 5 ) == 1 ? true : false;
}

uint AS560XComponent::magnitude() {
    uint16_t data;
    if (!this->read_byte_16( REGISTER_MAGNITUDE, &data )) return this->magnitude_sensor->get_raw_state();
    return data;
}

uint AS560XComponent::raw_angle() {
    uint16_t data;
    if (!this->read_byte_16( REGISTER_RAWANGLE, &data )) return 0;
    return data;
}

uint AS560XComponent::angle() {
    uint16_t data;
    if (!this->read_byte_16( REGISTER_ANGLE, &data )) return this->angle_sensor->get_raw_state();
    return data;
}

uint8_t AS560XComponent::gain() {
    auto data = this->read_byte( REGISTER_AGC );
    if (!data) return 0;
    return data.value();
}


}  // namespace as560x
}  // namespace esphome