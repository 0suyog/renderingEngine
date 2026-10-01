#pragma once
#include "entity.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
namespace Core {
class EntityManager {
public:
  std::shared_ptr<Entity> AddEntity(const std::string &tag);
  std::vector<std::shared_ptr<Entity>> GetEntities();
  std::vector<std::shared_ptr<Entity>> GetEntities(const std::string &tag);
  unsigned int getTotalEntitiesCreated() { return m_totalEntities; }
  void update();

private:
  std::vector<std::shared_ptr<Entity>> m_entities;
  std::unordered_map<std::string, std::vector<std::shared_ptr<Entity>>>
      m_entitiesMap;
  std::vector<std::shared_ptr<Entity>> m_toAdd;
  unsigned int m_totalEntities;
};

} // namespace Core
