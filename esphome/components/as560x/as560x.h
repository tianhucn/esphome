#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/i2c/i2c.h"


namespace esphome {
namespace as560x {



class AS560XComponent : public Component, public i2c::I2CDevice {
 public:
  AS560XComponent() = default;

  void continue_common_setup();
  void loop() override;
  void dump_common_sensor_config();
  float get_setup_priority() const override { return setup_priority::DATA; }

  // Sensors
  void set_presence_sensor_(binary_sensor::BinarySensor *sensor) { presence_binarysensor = sensor; }
  void set_angle_sensor_(sensor::Sensor *sensor) { angle_sensor = sensor; }
  void set_magnitude_sensor_(sensor::Sensor *sensor) { magnitude_sensor = sensor; }
  binary_sensor::BinarySensor presence_sensor_() { return *presence_binarysensor; } 
  sensor::Sensor angle_sensor_() { return *angle_sensor; } 
  sensor::Sensor magnitude_sensor_() { return *magnitude_sensor; }

  // Common device configuration
  void set_zero_position( uint value ) { this->_zero_position = value; };
  void set_end_position( uint value ) { this->_end_position = value; };
  void set_mang( uint value ) { this->_mang = value; };
  void set_ab_resolution(uint16_t value) { this->_ab_resolution = value; };
  void set_push_threshold( uint value ) { this->_push_threshold = value; }; 


  // Common device readings
  uint magnitude();
  uint angle();
  uint raw_angle();
  uint8_t gain();
  bool presence();


 protected:
  int16_t read_status_byte();
  bool device_online();
  void write_ab_resolution(uint16_t positions);
  void write_zero_position(uint16_t angle);

  binary_sensor::BinarySensor *presence_binarysensor{nullptr};
  sensor::Sensor *angle_sensor{nullptr};
  sensor::Sensor *magnitude_sensor{nullptr};

  uint16_t _ab_resolution;
  uint16_t _zero_position;
  uint16_t _push_threshold;
  uint16_t _end_position;
  uint16_t _mang;
};



}  // namespace as560x
}  // namespace esphome