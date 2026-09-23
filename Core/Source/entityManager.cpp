#include "entityManager.h"
#include "entity.h"
namespace Core {

void removeSafely(std::vector<std::shared_ptr<Entity>> &v, unsigned int i) {
  std::swap(v[i], v.back());
  v.pop_back();
}

[[nodiscard]] std::shared_ptr<Entity>
EntityManager::AddEntity(const std::string &tag) {
  std::shared_ptr<Entity> e(new Entity(++m_totalEntities, tag));
  m_toAdd.push_back(e);
  return e;
}

[[nodiscard]] std::vector<std::shared_ptr<Entity>>
EntityManager::GetEntities() {
  return m_entities;
}

[[nodiscard]] std::vector<std::shared_ptr<Entity>>
EntityManager::GetEntities(const std::string &tag) {
  if (m_entitiesMap.contains(tag)) {
    return m_entitiesMap[tag];
  }
  return {};
}

void EntityManager::update() {
  for (unsigned int i = 0; i < m_entities.size();) {
    if (!m_entities[i]->m_alive) {
      removeSafely(m_entities, i);
    } else {
      i++;
    }
  }
  m_entities.insert(m_entities.end(), m_toAdd.begin(), m_toAdd.end());
  for (const auto &e : m_toAdd) {
    m_entitiesMap[e->m_tag].push_back(e);
  }
  m_toAdd.clear();
}

} // namespace Core
