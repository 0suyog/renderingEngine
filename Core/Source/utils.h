#pragma once
#include <cstdlib>
namespace Core {
inline bool nearZero(float x) { return std::abs(x) < 0.0001f; }

} // namespace Core
