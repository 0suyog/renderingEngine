#pragma once
#include <concepts>
#include <memory>
#include <utility>
#include "event.h"
namespace Core {
class Layer {
public:
  virtual ~Layer() = default;

  virtual void OnUpdate(float ts) {}

  virtual void OnRender() {}
  virtual void OnEvent(Core::Event &e) {}
  template <std::derived_from<Layer> T, typename... Args>
  void TransitionTo(Args &&...args) {
    QueueTransition(
        std::move(std::make_unique<T>(std::forward<Args>(args)...)));
  }

private:
  void QueueTransition(std::unique_ptr<Layer> layer);
};
} // namespace Core
