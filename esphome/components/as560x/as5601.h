#pragma once

#include "as560x.h"

namespace esphome {
namespace as560x {

class AS5601Component : public AS560XComponent {
 public:
  AS5601Component() = default;
  void dump_config() override;
  void setup() override;
};



}  // namespace as560x
}  // namespace esphome