#include "layer.h"
#include "application.h"
#include <memory>
namespace Core {
void Layer::QueueTransition(std::unique_ptr<Layer> toLayer) {
  auto &layerStack = Application::Get().m_LayerStack;
  for (auto &layer : layerStack) {
    if (layer.get() == this) {
      layer = std::move(toLayer);
      return;
    }
  }
}
} // namespace Core
