#pragma once

#include "esphome/core/component.h"
#include "as560x.h"

namespace esphome {
namespace as560x {


class AS5600Component : public AS560XComponent {
 public:
  AS5600Component() = default;
  void dump_config() override;
  void setup() override;

  void set_stop_position( uint value ) { this->stop_position_ = value; };
  void set_maximum_angle( uint value ) { this->maximum_angle_ = value; };
  
  void write_stop_position(uint16_t position);
  void write_maximum_angle(uint16_t angle);

 protected:
  uint16_t stop_position_{4095};
  uint16_t maximum_angle_{359};
};

template<typename... Ts> class AS5600ConfigAction : public Action<Ts...>, public Parented<AS5600Component> {
 public:
  explicit AS5600ConfigAction(AS5600Component *parent) : parent_(parent) {}

  TEMPLATABLE_VALUE(uint16_t, zero_position)
  TEMPLATABLE_VALUE(uint16_t, stop_position)
  TEMPLATABLE_VALUE(uint16_t, maximum_angle)

  void play(Ts... x) override {
    if (this->zero_position_.has_value())
      this->parent_->write_zero_position(this->zero_position_.value(x...));

    if (this->stop_position.has_value())
      this->parent_->write_stop_position(this->stop_position.value(x...));
      
    if (this->maximum_angle_.has_value())
      this->parent_->write_maximum_angle(this->maximum_angle_.value(x...));
  }

 protected:
  AS5600Component *parent_;
};


} // as560x
} // esphome