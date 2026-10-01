#pragma once
namespace Core {
struct Action {
  enum : unsigned int {
    LEFT,
    RIGHT,
    UP,
    DOWN,
    FORWARD,
    BACKWARD,
    FAST,
    QUIT,
    JUMP,
  };
  unsigned int type;
  bool isStart;
};
} // namespace Core
