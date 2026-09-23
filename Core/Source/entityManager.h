#include "entity.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
namespace Core {
class EntityManager {
public:
  [[nodiscard]] std::shared_ptr<Entity> AddEntity(const std::string &tag);
  [[nodiscard]] std::vector<std::shared_ptr<Entity>> GetEntities();
  [[nodiscard]] std::vector<std::shared_ptr<Entity>>
  GetEntities(const std::string &tag);
  void update();

private:
  std::vector<std::shared_ptr<Entity>> m_entities;
  std::unordered_map<std::string, std::vector<std::shared_ptr<Entity>>>
      m_entitiesMap;
  std::vector<std::shared_ptr<Entity>> m_toAdd;
  unsigned int m_totalEntities;
};

} // namespace Core
