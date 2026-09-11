#include <vector>
#include "physicsObject.h"
namespace Physics {
class PhysicsEngine {
public:
  PhysicsEngine() {};

  void AddObject(const PhysicsObject &obj);
  void Simulate(float timeStep);

private:
  std::vector<PhysicsObject> m_objects;
};
} // namespace Physics
