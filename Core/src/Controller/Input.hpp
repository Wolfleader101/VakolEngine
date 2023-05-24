#pragma once

#include <glm/glm.hpp>
#include <unordered_map>

#include "Events/KeyEvent.hpp"
#include "Events/MouseEvent.hpp"

namespace Vakol::Controller {

    class Input {
       public:
           enum class KEY {
               KEY_SPACE = 32,
               KEY_APOSTROPHE = 39,
               KEY_COMMA = 44,
               KEY_MINUS = 45,
               KEY_PERIOD = 46,
               KEY_SLASH = 47,
               KEY_0 = 48,
               KEY_1 = 49,
               KEY_2 = 50,
               KEY_3 = 51,
               KEY_4 = 52,
               KEY_5 = 53,
               KEY_6 = 54,
               KEY_7 = 55,
               KEY_8 = 56,
               KEY_9 = 57,
               KEY_SEMICOLON = 59,
               KEY_EQUAL = 61,
               KEY_A = 65,
               KEY_B = 66,
               KEY_C = 67,
               KEY_D = 68,
               KEY_E = 69,
               KEY_F = 70,
               KEY_G = 71,
               KEY_H = 72,
               KEY_I = 73,
               KEY_J = 74,
               KEY_K = 75,
               KEY_L = 76,
               KEY_M = 77,
               KEY_N = 78,
               KEY_O = 79,
               KEY_P = 80,
               KEY_Q = 81,
               KEY_R = 82,
               KEY_S = 83,
               KEY_T = 84,
               KEY_U = 85,
               KEY_V = 86,
               KEY_W = 87,
               KEY_X = 88,
               KEY_Y = 89,
               KEY_Z = 90,
               KEY_LEFT_SHIFT = 340,
               KEY_ESCAPE = 256
        };

        enum class KeyState { KEY_IDLE, KEY_PRESSED, KEY_HELD, KEY_RELEASED };
        bool OnKeyPressed(KeyPressedEvent& kev);
        bool OnKeyReleased(KeyReleasedEvent& kev);
        bool OnMouseMoved(MouseMovedEvent& ev);

        const glm::vec2& GetMousePos() const { return m_mousePos; }
        glm::vec2 GetDeltaMousePos() {
            auto pos = m_deltaMousePos;
            m_deltaMousePos = glm::vec2(0.0f, 0.0f);
            return pos;
        }

        bool GetKey(KEY keycode) const;
        bool GetKeyDown(KEY keycode) const;
        bool GetKeyUp(KEY keycode) const;

        void Update();

       private:
        glm::vec2 m_prevMousePos;
        glm::vec2 m_mousePos;
        glm::vec2 m_deltaMousePos;

        std::unordered_map<KEY, KeyState> m_keyStates;
    };
}  // namespace Vakol::Controller