#pragma once

#include <glm/glm.hpp>
#include <unordered_map>

#include "Events/KeyEvent.hpp"
#include "Events/MouseEvent.hpp"

namespace Vakol::Controller {

    /**
     * @class Input
     *
     * @brief Class representing the input manager.
     */
    class Input {
       public:
        /**
         * @enum KEY
         *
         * @brief Enum representing key codes.
         */
        enum class KEY {
            KEY_MOUSE_0 = 0,
            KEY_MOUSE_1 = 1,
            KEY_MOUSE_MIDDLE = 2,
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

        /**
         * @enum KeyState
         *
         * @brief Enum representing the state of a key.
         */
        enum class KeyState { KEY_IDLE, KEY_PRESSED, KEY_HELD, KEY_RELEASED };

        /**
         * @brief Handle the event when a key is pressed.
         *
         * @param kev The KeyPressedEvent.
         * @return bool True if the event is handled, false otherwise.
         */
        bool OnKeyPressed(KeyPressedEvent& kev);

        /**
         * @brief Handle the event when a key is released.
         *
         * @param kev The KeyReleasedEvent.
         * @return bool True if the event is handled, false otherwise.
         */
        bool OnKeyReleased(KeyReleasedEvent& kev);

        /**
         * @brief Handle the event when the mouse is moved.
         *
         * @param ev The MouseMovedEvent.
         * @return bool True if the event is handled, false otherwise.
         */
        bool OnMouseMoved(MouseMovedEvent& ev);

        /**
         * @brief Handle the event when a mouse button is pressed.
         *
         * @param mev The MouseButtonPressedEvent.
         * @return bool True if the event is handled, false otherwise.
         */
        bool OnMouseButtonPressed(MouseButtonPressedEvent& mev);

        /**
         * @brief Handle the event when a mouse button is released.
         *
         * @param mev The MouseButtonReleasedEvent.
         * @return bool True if the event is handled, false otherwise.
         */
        bool OnMouseButtonReleased(MouseButtonReleasedEvent& mev);

        /**
         * @brief Get the current mouse position.
         *
         * @return const glm::vec2& The current mouse position.
         */
        const glm::vec2& GetMousePos() const { return m_mousePos; }

        /**
         * @brief Get the delta mouse position since the last update.
         *
         * @return glm::vec2 The delta mouse position.
         */
        glm::vec2 GetDeltaMousePos() {
            auto pos = m_deltaMousePos;
            m_deltaMousePos = glm::vec2(0.0f, 0.0f);
            return pos;
        }

        /**
         * @brief Check if a key is currently pressed.
         *
         * @param keycode The key code to check.
         * @return bool True if the key is pressed, false otherwise.
         */
        bool GetKey(KEY keycode) const;

        /**
         * @brief Check if a key was just pressed.
         *
         * @param keycode The key code to check.
         * @return bool True if the key was just pressed, false otherwise.
         */
        bool GetKeyDown(KEY keycode) const;

        /**
         * @brief Check if a key was just released.
         *
         * @param keycode The key code to check.
         * @return bool True if the key was just released, false otherwise.
         */
        bool GetKeyUp(KEY keycode) const;

        /**
         * @brief Check if a mouse button is currently pressed.
         *
         * @param keycode The key code to check.
         * @return bool True if the mouse button is pressed, false otherwise.
         */
        bool GetMouseButton(KEY keycode) const;

        /**
         * @brief Check if a mouse button was just pressed.
         *
         * @param keycode The key code to check.
         * @return bool True if the mouse button was just pressed, false otherwise.
         */
        bool GetMouseButtonDown(KEY keycode) const;

        /**
         * @brief Check if a mouse button was just released.
         *
         * @param keycode The key code to check.
         * @return bool True if the mouse button was just released, false otherwise.
         */
        bool GetMouseButtonUp(KEY keycode) const;

        /**
         * @brief Update the input state.
         */
        void Update();

       private:
        /**
         * @brief The previous mouse position.
         */
        glm::vec2 m_prevMousePos;

        /**
         * @brief The current mouse position.
         */
        glm::vec2 m_mousePos;

        /**
         * @brief The delta mouse position since the last update.
         */
        glm::vec2 m_deltaMousePos;

        /**
         * @brief Map of key states.
         */
        std::unordered_map<KEY, KeyState> m_keyStates;
    };
}  // namespace Vakol::Controller