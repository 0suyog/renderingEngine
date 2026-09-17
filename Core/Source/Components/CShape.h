#pragma once
#include "Renderer/regularPolygon.h"
namespace Component {
class CShape {
public:
  CShape(const Renderer::RegularPolygon &shape) : shape(shape) {}
  Renderer::RegularPolygon shape;
};
} // namespace Component
