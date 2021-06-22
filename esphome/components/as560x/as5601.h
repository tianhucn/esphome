#pragma once

#include "esphome/core/component.h"
#include "esphome/core/automation.h"
#include "as560x.h"

namespace esphome {
namespace as560x {


class AS5601Component : public AS560XComponent {
 public:
  AS5601Component() = default;
  void dump_config() override;
  void setup() override;

  // Configuration setters and writers
  void set_ab_resolution(uint16_t value) { this->ab_resolution_ = value; };
  void set_push_threshold(uint value ) { this->push_threshold_ = value; };
  void write_ab_resolution(uint16_t resolution);
  void write_push_threshold(uint16_t threshold);

  uint16_t ab_resolution() { return this->ab_resolution_; }

 protected: 
  uint16_t ab_resolution_{8};
  uint16_t push_threshold_{0};
};


template<typename... Ts> class AS5601ConfigAction : public Action<Ts...>, public Parented<AS5601Component> {
 public:
  // explicit AS5601ConfigAction(AS5601Component *parent) : parent_(parent) {}

  TEMPLATABLE_VALUE(uint16_t, zero_position)
  TEMPLATABLE_VALUE(uint16_t, ab_resolution)
  TEMPLATABLE_VALUE(uint16_t, push_threshold)

  void play(Ts... x) override {
    if (this->zero_position_.has_value()) 
      this->parent_->write_zero_position(this->zero_position_.value(x...));

    if (this->ab_resolution_.has_value()) {
      ESP_LOGI("as5601", "Received new AB value %d", this->ab_resolution_.value(x...));
      this->parent_->write_ab_resolution(this->ab_resolution_.value(x...));
    }

    if (this->push_threshold_.has_value())
      this->parent_->write_push_threshold(this->push_threshold_.value(x...));
  }

 protected:
  AS5601Component *parent_;
};


} // as560x
} // as5600