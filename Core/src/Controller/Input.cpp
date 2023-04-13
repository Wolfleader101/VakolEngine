#include "Input.hpp"

namespace Vakol::Controller {
    bool Input::OnKeyPressed(KeyPressedEvent& kev) {
        KEY keycode = static_cast<KEY>(kev.GetKeyCode());
        if (m_keyStates[keycode] != KeyState::KEY_HELD) {
            m_keyStates[keycode] = KeyState::KEY_PRESSED;
        }
        return true;
    }

    bool Input::OnKeyReleased(KeyReleasedEvent& kev) {
        KEY keycode = static_cast<KEY>(kev.GetKeyCode());
        m_keyStates[keycode] = KeyState::KEY_RELEASED;
        return true;
    }

    bool Input::GetKey(KEY keycode) const {
        auto it = m_keyStates.find(keycode);
        return it != m_keyStates.end() && (it->second == KeyState::KEY_PRESSED || it->second == KeyState::KEY_HELD);
    }
    bool Input::GetKeyDown(KEY keycode) const {
        auto it = m_keyStates.find(keycode);
        return it != m_keyStates.end() && it->second == KeyState::KEY_PRESSED;
    }

    bool Input::GetKeyUp(KEY keycode) const {
        auto it = m_keyStates.find(keycode);
        return it != m_keyStates.end() && it->second == KeyState::KEY_RELEASED;
    }

    bool Input::OnMouseMoved(MouseMovedEvent& ev) {
        float xpos = static_cast<float>(ev.GetX());
        float ypos = static_cast<float>(ev.GetY());

        m_mousePos = glm::vec2(xpos, ypos);
        glm::vec2 m_deltaMousePos;

        m_deltaMousePos = m_mousePos - m_prevMousePos;

        // float xoffset = xpos - lastX; // delta x
        // float yoffset = lastY - ypos; // delta y

        m_prevMousePos = m_mousePos;

        return true;
    }

    void Input::Update() {
        for (auto& [key, state] : m_keyStates) {
            if (state == KeyState::KEY_PRESSED) {
                state = KeyState::KEY_HELD;
            } else if (state == KeyState::KEY_RELEASED) {
                state = KeyState::KEY_IDLE;
            }
        }
    }
}  // namespace Vakol::Controller