#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/i2c/i2c.h"


namespace esphome {
namespace as5601 {

class AS5601 : public Component, public i2c::I2CDevice {
 public:
  void setup() override;
  void loop() override;
  void dump_config() override;
  float get_setup_priority() const override { return setup_priority::DATA; }


  // Initial config
  void init_ab_positions(uint16_t positions) { this->_ab_positions = positions; };
  void init_zero_position( uint angle ) { this->_zero_position = angle; };

  // Sensors
  void init_presence_sensor_(binary_sensor::BinarySensor *sensor) { presence_binarysensor = sensor; }
  void init_orientation_sensor_(sensor::Sensor *sensor) { orientation_sensor = sensor; }
  void init_magnitude_sensor_(sensor::Sensor *sensor) { magnitude_sensor = sensor; }

  // binary_sensor::BinarySensor presence_sensor_() { return *presence_binarysensor; } 
  // sensor::Sensor orientation_sensor_() { return *orientation_sensor; } 
  // sensor::Sensor magnitude_sensor_() { return *magnitude_sensor; }
 


  // Hardware access
  void write_ab_resolution(uint16_t positions);
  void write_zero_position(uint16_t angle);

  // Sensor readings
  int16_t read_status_byte();
  bool presence();
  uint magnitude();
  uint angle();
  uint raw_angle();
  uint8_t gain();


  protected:
   uint16_t _ab_positions, _zero_position;

   binary_sensor::BinarySensor *presence_binarysensor;
   sensor::Sensor *orientation_sensor, *magnitude_sensor;
};


}  // namespace as5601
}  // namespace esphome