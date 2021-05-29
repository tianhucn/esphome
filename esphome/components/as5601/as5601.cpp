#include "as5601.h"
#include "esphome/core/log.h"

namespace esphome {
namespace as5601 {

static const char *TAG = "as5601";

/** Register Map **/
// Configuration Registers
static const uint8_t REGISTER_ZMCO = 0x00;
static const uint16_t REGISTER_ZPOS = 0x01;
static const uint16_t REGISTER_CONF = 0x03;
static const uint8_t REGISTER_ABN = 0x09;
static const uint8_t REGISTER_PUSHTHR = 0x0A;

// Output Registers
static const uint16_t REGISTER_RAWANGLE = 0x0C;
static const uint16_t REGISTER_ANGLE = 0x0E;

// Status Registers
static const uint8_t REGISTER_STATUS = 0x0B;
static const uint8_t REGISTER_AGC = 0x1A;
static const uint16_t REGISTER_MAGNITUDE = 0x1B;

// Burn Command
static const uint8_t REGISTER_BURN = 0xFF;
/** **/


void AS5601::dump_config() {
    ESP_LOGCONFIG(TAG, "AS5601:");
    ESP_LOGCONFIG(TAG, "  AB positions: %d", this->_ab_positions);
    ESP_LOGCONFIG(TAG, "  Zero position: %d", this->_zero_position);
    LOG_BINARY_SENSOR("  ", "Magnet Field Presence", this->presence_binarysensor);
    LOG_SENSOR("  ", "Magnet Field Orientation", this->orientation_sensor);
    LOG_SENSOR("  ", "Magnet Field Magnitude", this->magnitude_sensor);
}


void AS5601::setup() {
    ESP_LOGCONFIG(TAG, "Setting up AS5601...");

    int16_t status = this->read_status_byte();
    if (!status) {
        ESP_LOGE(TAG, "Unable to get status from AS5601");
        this->mark_failed();
    }

    ESP_LOGV(TAG, "Got AS5601 status %d", status);
    this->write_ab_resolution(this->_ab_positions);
    this->write_zero_position(this->_zero_position);

    // Initial sensor publishing
    if (this->magnitude_sensor != nullptr) this->magnitude_sensor->publish_state(magnitude());
    if (this->orientation_sensor != nullptr) this->orientation_sensor->publish_state(angle());
}


void AS5601::loop() {
    if (this->presence_binarysensor != nullptr) this->presence_binarysensor->publish_state(presence());
    //delayMicroseconds(1000);

    // Publish any changes to the magnitude sensor if it is enabled
    if (this->magnitude_sensor != nullptr) {
        uint data = magnitude();
        if (data != this->magnitude_sensor->get_raw_state()) this->magnitude_sensor->publish_state(data);
    }
    //delayMicroseconds(1000);

    // Publish any changes to the orientation sensor if it is enabled
    if (this->orientation_sensor != nullptr) {
        uint data = angle();
        if (data != this->orientation_sensor->get_raw_state()) this->orientation_sensor->publish_state(data);
    }
    //delayMicroseconds(1000);
}


void AS5601::write_ab_resolution(uint16_t positions) {
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


void AS5601::write_zero_position(uint16_t angle) {
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


int16_t AS5601::read_status_byte() {
    auto status = this->read_byte( REGISTER_STATUS );
    return (int16_t)(status ? status.value() : -1);
}

bool AS5601::presence() {
    auto data = this->read_byte( REGISTER_STATUS );
    if (!data) return false; //this->mark_failed();
    return bitRead( data.value(), 5 ) == 1 ? true : false;
}

uint AS5601::magnitude() {
    uint16_t data;
    auto ack = this->read_byte_16( REGISTER_MAGNITUDE, &data );
    if (!ack) {
        // this->mark_failed();
        return this->magnitude_sensor->get_raw_state();
    }
    return data;
}

uint AS5601::raw_angle() {
    uint16_t data;
    if (!this->read_byte_16( REGISTER_RAWANGLE, &data )) return 0; //this->mark_failed();
    return data;
}

uint AS5601::angle() {
    uint16_t data;
    auto ack = this->read_byte_16( REGISTER_ANGLE, &data );
    if (!ack) {
        // this->mark_failed();
        return this->orientation_sensor->get_raw_state();
    }
    return data;
}

uint8_t AS5601::gain() {
    auto data = this->read_byte( REGISTER_AGC );
    if (!data) {
        //this->mark_failed();
        return 0;
    }
    return data.value();
}


}  // namespace as5601
}  // namespace esphome