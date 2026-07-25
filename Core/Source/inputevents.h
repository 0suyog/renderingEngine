#pragma once
#include "event.h"
#include <GLFW/glfw3.h>
#include <format>

namespace Core {
class KeyEvent : public Event {
public:
  inline int GetKeyCode() const { return m_KeyCode; }
  inline int GetScanCode() const { return m_ScanCode; }
  inline const char *GetPrintableChar() const {
    const char *c = glfwGetKeyName(m_KeyCode, m_ScanCode);
    return c ? c : "<nonprintable>";
  }

protected:
  KeyEvent(int scancode, int keycode)
      : m_ScanCode(scancode), m_KeyCode(keycode) {}
  int m_ScanCode, m_KeyCode;
};

class KeyPressedEvent : public KeyEvent {
public:
  KeyPressedEvent(int scancode, int keycode, bool isRepeat)
      : KeyEvent(scancode, keycode), m_IsRepeat(isRepeat) {}
  inline bool IsRepeat() const { return m_IsRepeat; }

  std::string ToString() const override {
    const char *printableChar = GetPrintableChar();
    return std::format("KeyPressedEvent: {}, (repeat={}), char: {}", m_ScanCode,
                       m_IsRepeat, printableChar);
  }
  EVENT_CLASS_TYPE(KeyPressed)
private:
  bool m_IsRepeat;
};

class KeyReleasedEvent : public KeyEvent {
public:
  KeyReleasedEvent(int scancode, int keycode) : KeyEvent(scancode, keycode) {}

  std::string ToString() const override {
    return std::format("KeyReleasedEvent: {}", m_ScanCode);
  }
  EVENT_CLASS_TYPE(KeyReleased)
};

// mousd Events

class MouseMovedEvent : public Event {
public:
  MouseMovedEvent(double x, double y) : m_MouseX(x), m_MouseY() {}

  inline double GetX() const { return m_MouseX; }
  inline double GetY() const { return m_MouseY; }

  std::string ToString() const override {
    return std::format("MousemovedEvent: {},{}", m_MouseX, m_MouseY);
  }

  EVENT_CLASS_TYPE(MouseMoved)

private:
  double m_MouseX, m_MouseY;
};

class MouseScrolledEvent : public Event {
public:
  MouseScrolledEvent(double xOffset, double yOffset)
      : m_XOffset(xOffset), m_YOffset(yOffset) {}

  inline double GetXOffset() const { return m_XOffset; }
  inline double GetYOffset() const { return m_YOffset; }

  std ::string ToString() const override {
    return std::format("MouseScrolledEvent: {}, {}", m_XOffset, m_YOffset);
  }

  EVENT_CLASS_TYPE(MouseScrolled)

private:
  double m_XOffset, m_YOffset;
};

class MouseButtonEvent : public Event {
public:
  inline int GetMouseButton() const { return m_Button; }

protected:
  MouseButtonEvent(int button) : m_Button(button) {}
  int m_Button;
};

class MouseButtonPressedEvent : public MouseButtonEvent {
public:
  MouseButtonPressedEvent(int button) : MouseButtonEvent(button) {}

  std::string ToString() const override {
    return std::format("MouseButtonPressedEvent: {}", m_Button);
  }

  EVENT_CLASS_TYPE(MouseButtonPressed);
};

class MouseButtonReleasedEvent : public MouseButtonEvent {
public:
  MouseButtonReleasedEvent(int button) : MouseButtonEvent(button) {}
  std::string ToString() const override {
    return std::format("MouseButtonReleaedEvent: {}", m_Button);
  }
  EVENT_CLASS_TYPE(MouseButtonReleased)
};

} // namespace Core
