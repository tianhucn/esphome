#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/i2c/i2c.h"

namespace esphome {
namespace as560x {


#define LOG_AS560X(this) \
  ESP_LOGCONFIG(TAG, "  Zero position", this->zero_position_); \
  LOG_SENSOR(TAG, "  Angle sensor", this->angle_sensor_); \
  LOG_SENSOR(TAG, "  Magnitude sensor", this->magnitude_sensor_); \
  LOG_BINARY_SENSOR(TAG, "  Magnet presence binary sensor", this->presence_binary_sensor_);


class AS560XComponent : public Component, public i2c::I2CDevice {
 public:
  AS560XComponent() = default;

  float get_setup_priority() const override { return setup_priority::DATA; }
  void loop() override;
  void setup() override;

  // Configuration setters
  void set_zero_position( uint position ) { this->zero_position_ = position; };
  void set_presence_binary_sensor(binary_sensor::BinarySensor *binary_sensor) { this->presence_binary_sensor_ = binary_sensor; }
  void set_angle_sensor(sensor::Sensor *sensor) { this->angle_sensor_ = sensor; }
  void set_magnitude_sensor(sensor::Sensor *sensor) { this->magnitude_sensor_ = sensor; }

  void update_device();

  // Device readings
  uint16_t magnitude();
  uint16_t angle();
  uint16_t raw_angle();
  uint8_t gain();
  bool presence();

 protected:
  void common_setup();

  int16_t read_status_byte();
  bool is_device_online();
  void write_zero_position(uint16_t position);

  binary_sensor::BinarySensor *presence_binary_sensor_{nullptr};
  sensor::Sensor *angle_sensor_{nullptr};
  sensor::Sensor *magnitude_sensor_{nullptr};

  uint16_t zero_position_{0};
};



}  // namespace as560x
}  // namespace esphome