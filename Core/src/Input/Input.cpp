#include "Input.hpp"

namespace Vakol
{
    bool Input::OnKeyPressed(KeyPressedEvent& kev)
    {
        KEY keycode = static_cast<KEY>(kev.GetKeyCode());

        if (m_keyStates[keycode] != KeyState::KEY_HELD)
            m_keyStates[keycode] = KeyState::KEY_PRESSED;

        return true;
    }

    bool Input::OnKeyReleased(KeyReleasedEvent& kev)
    {
        KEY keycode = static_cast<KEY>(kev.GetKeyCode());

        m_keyStates[keycode] = KeyState::KEY_RELEASED;

        return true;
    }

    bool Input::GetKey(KEY keycode) const
    {
        auto it = m_keyStates.find(keycode);

        return it != m_keyStates.end() && (it->second == KeyState::KEY_PRESSED || it->second == KeyState::KEY_HELD);
    }

    bool Input::GetKeyDown(KEY keycode) const
    {
        auto it = m_keyStates.find(keycode);

        return it != m_keyStates.end() && it->second == KeyState::KEY_PRESSED;
    }

    bool Input::GetKeyUp(KEY keycode) const
    {
        auto it = m_keyStates.find(keycode);

        return it != m_keyStates.end() && it->second == KeyState::KEY_RELEASED;
    }

    bool Input::GetMouseButton(KEY keycode) const
    {
        auto it = m_keyStates.find(keycode);

        return it != m_keyStates.end() && (it->second == KeyState::KEY_PRESSED || it->second == KeyState::KEY_HELD);
    }

    bool Input::GetMouseButtonDown(KEY keycode) const
    {
        auto it = m_keyStates.find(keycode);

        return it != m_keyStates.end() && it->second == KeyState::KEY_PRESSED;
    }

    bool Input::GetMouseButtonUp(KEY keycode) const
    {
        auto it = m_keyStates.find(keycode);

        return it != m_keyStates.end() && it->second == KeyState::KEY_RELEASED;
    }

    bool Input::OnMouseMoved(MouseMovedEvent& ev)
    {
        m_mousePos = Math::Vec2(ev.GetX(), ev.GetY());

        m_deltaMousePos.x = m_mousePos.x - m_prevMousePos.x;
        m_deltaMousePos.y = m_prevMousePos.y - m_mousePos.y;

        m_prevMousePos = m_mousePos;

        return true;
    }

    bool Input::OnMouseButtonPressed(MouseButtonPressedEvent& mev)
    {
        KEY keycode = static_cast<KEY>(mev.GetMouseButton());

        if (m_keyStates[keycode] != KeyState::KEY_HELD)
            m_keyStates[keycode] = KeyState::KEY_PRESSED;

        return true;
    }

    bool Input::OnMouseButtonReleased(MouseButtonReleasedEvent& mev)
    {
        KEY keycode = static_cast<KEY>(mev.GetMouseButton());

        m_keyStates[keycode] = KeyState::KEY_RELEASED;

        return true;
    }

    void Input::Update()
    {
        for (auto& [key, state] : m_keyStates)
        {
            if (state == KeyState::KEY_PRESSED)
            {
                state = KeyState::KEY_HELD;
            }
            else if (state == KeyState::KEY_RELEASED)
            {
                state = KeyState::KEY_IDLE;
            }
        }
    }
} // namespace Vakol