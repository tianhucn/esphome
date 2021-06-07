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

  float get_setup_priority() const override { return setup_priority::DATA; }
  void loop() override;
  void continue_common_setup();
  void dump_common_sensor_config();

  // Configuration setters
  void set_zero_position( uint value ) { this->_zero_position = value; };
  void set_presence_sensor_(binary_sensor::BinarySensor *sensor) { presence_binarysensor = sensor; }
  void set_angle_sensor_(sensor::Sensor *sensor) { angle_sensor = sensor; }
  void set_magnitude_sensor_(sensor::Sensor *sensor) { magnitude_sensor = sensor; }

  binary_sensor::BinarySensor presence_sensor_() { return *presence_binarysensor; } 
  sensor::Sensor angle_sensor_() { return *angle_sensor; } 
  sensor::Sensor magnitude_sensor_() { return *magnitude_sensor; }

  // Device readings
  uint16_t magnitude();
  uint16_t angle();
  uint16_t raw_angle();
  uint8_t gain();
  bool presence();

 protected:
  int16_t read_status_byte();
  bool is_device_online();
  void write_zero_position(uint16_t position);

  binary_sensor::BinarySensor *presence_binarysensor{nullptr};
  sensor::Sensor *angle_sensor{nullptr};
  sensor::Sensor *magnitude_sensor{nullptr};

  uint16_t _zero_position;
};


class AS5600Component : public AS560XComponent {
 public:
  AS5600Component() = default;
  void dump_config() override;
  void setup() override;

  void set_stop_position( uint value ) { this->_stop_position = value; };
  void set_maximum_angle( uint value ) { this->_maximum_angle = value; };
  
  void write_stop_position(uint16_t position);
  void write_maximum_angle(uint16_t angle);

 protected:
  uint16_t _stop_position;
  uint16_t _maximum_angle;
};


class AS5601Component : public AS560XComponent {
 public:
  AS5601Component() = default;
  void dump_config() override;
  void setup() override;

  void set_ab_resolution(uint16_t value) { this->_ab_resolution = value; };
  void set_push_threshold( uint value ) { this->_push_threshold = value; };

  void write_ab_resolution(uint16_t resolution);
  void write_push_threshold(uint16_t threshold);

 protected: 
  uint16_t _ab_resolution;
  uint16_t _push_threshold;
};


}  // namespace as560x
}  // namespace esphome