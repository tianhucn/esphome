#pragma once

#include "esphome/core/component.h"
#include "esphome/components/as560x/as560x.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/i2c/i2c.h"


namespace esphome {
namespace as5600 {

class AS5600 : public as560x::AS560X {
 public:
  void setup() override;
  void loop() override;
  void dump_config() override;
  float get_setup_priority() const override { return setup_priority::DATA; }
};


}  // namespace as5600
}  // namespace esphome