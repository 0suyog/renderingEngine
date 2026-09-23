#pragma once
namespace Core {
struct Action {
  enum : unsigned int { NONE, LEFT, RIGHT, UP, DOWN, FORWARD, BACKWARD, FAST };
  unsigned int type;
  bool isStart;
};
} // namespace Core
