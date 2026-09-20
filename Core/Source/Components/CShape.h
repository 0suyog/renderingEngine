#pragma once
#include "Renderer/shapes3d.h"
namespace Component {
class CShape {
public:
  CShape(const Renderer::Shapes3D &shape) : shape(shape) {}
  Renderer::Shapes3D shape;
};
} // namespace Component
