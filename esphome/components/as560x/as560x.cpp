#include "as560x.h"
#include "esphome/core/log.h"

namespace esphome {
namespace as560x {

static const char *TAG = "as560x";

/** AS5600 and AS560X Register Map **/
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



void AS560X::write_ab_resolution(uint16_t positions) {
    ESP_LOGV(TAG, "Setting AB positions to %d", positions);
    
    // Taken from https://github.com/bitfasching/AS5601/blob/master/AS5601.h
    char power = -1;
    while ( ( 1 << ++power ) < this->_ab_positions);
    auto ack = this->write_byte(REGISTER_ABN, power-3);
    delayMicroseconds(1000);

    if (!ack) {
        ESP_LOGE(TAG, "Failed to update AB positions to %d", positions);
        // this->mark_failed();
        return; 
    }

    // Do not set if write failed
    this->_ab_positions = positions;
    return;
};


void AS560X::write_zero_position(uint16_t angle) {
    ESP_LOGV(TAG, "Setting zero angle to %d", angle);
    auto ack = this->write_byte_16(REGISTER_ZPOS, angle);
    delayMicroseconds(1000);

    if (!ack) {
        ESP_LOGE(TAG, "Failed to update zero position (orientation offset) to %d", angle);
        // this->mark_failed();
        return;
    }

    // Do not set if write failed
    this->_zero_position = angle;
    return;
};


auto AS560X::read_status_byte() { return this->read_byte( REGISTER_STATUS ); }


bool AS560X::presence() {
    auto data = read_status_byte();
    if (!data) return false; //this->mark_failed();
    return bitRead( data.value(), 5 ) == 1 ? true : false;
}

uint AS560X::magnitude() {
    uint16_t data;
    auto ack = this->read_byte_16( REGISTER_MAGNITUDE, &data );
    if (!ack) {
        // this->mark_failed();
        return this->magnitude_sensor->get_raw_state();
    }
    return data;
}

uint AS560X::raw_angle() {
    uint16_t data;
    if (!this->read_byte_16( REGISTER_RAWANGLE, &data )) return 0; //this->mark_failed();
    return data;
}

uint AS560X::angle() {
    uint16_t data;
    auto ack = this->read_byte_16( REGISTER_ANGLE, &data );
    if (!ack) {
        // this->mark_failed();
        return this->orientation_sensor->get_raw_state();
    }
    return data;
}

uint8_t AS560X::gain() {
    auto data = this->read_byte( REGISTER_AGC );
    if (!data) {
        //this->mark_failed();
        return 0;
    }
    return data.value();
}


}  // namespace as560x
}  // namespace esphome