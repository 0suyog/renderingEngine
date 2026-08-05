#pragma once
#include "cstdint"
namespace Core {
enum Movement : uint8_t {
  NONE = 0,
  LEFT = 1 << 1,
  RIGHT = 1 << 2,
  UP = 1 << 3,
  DOWN = 1 << 4,
  FORWARD = 1 << 5,
  BACKWARD = 1 << 6,
  FAST = 1 << 7
};
}
