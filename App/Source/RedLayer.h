#include "event.h"
#include "layer.h"
#include "inputevents.h"
#include <iostream>
#include <string>
class RedLayer : public Core::Layer {

  bool handleMousePress(Core::MouseButtonPressedEvent &e) {
    std::cerr << "button: " << e.GetMouseButton() << "\n";
    return true;
  }

  bool handleKeyPress(Core::KeyPressedEvent &e) {
    std::cerr << "char: " << e.GetPrintableChar() << '\n';
    return true;
  }

  void OnEvent(Core::Event &e) override {
    // std::cerr << "[Event Occured]: " << e.ToString() << std::endl;
    Core::EventDispatcher dispatcher(e);
    dispatcher.Dispatch<Core::MouseButtonPressedEvent>(
        [this](Core::MouseButtonPressedEvent &e) {
          return handleMousePress(e);
        });
  }
};
