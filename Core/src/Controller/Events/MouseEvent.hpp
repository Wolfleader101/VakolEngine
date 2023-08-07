#pragma once

#include <sstream>

#include "Event.hpp"

namespace Vakol::Controller {
    /**
     * @class MouseMovedEvent
     * @brief the mouse moved event
     */
    class MouseMovedEvent : public Event {
       public:
        /**
         * @brief the constructor to set x,y
         * @param x to set
         * @param y to set
         */
        MouseMovedEvent(float x, float y) : m_MouseX(x), m_MouseY(y) {}

        /**
         * @brief get the x pos
         * @return x pos
         */
        inline float GetX() const { return m_MouseX; }

        /**
         * @brief get the y pos
         * @return y pos
         */
        inline float GetY() const { return m_MouseY; }

        /**
         * @brief convert to string
         * @return event as string
         */
        std::string ToString() const override {
            std::stringstream ss;
            ss << "MouseMovedEvent: " << m_MouseX << ", " << m_MouseY;
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseMoved)
        EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
       private:
        /**
         * @brief mouse x and y
         */
        float m_MouseX, m_MouseY;
    };

    /**
     * @class MouseScrolledEvent
     * @brief mouse scrolled event
     */
    class MouseScrolledEvent : public Event {
       public:
        /**
         * @brief constructor for scrolled
         * @param xOffset of scroll
         * @param yOffset of scroll
         */
        MouseScrolledEvent(float xOffset, float yOffset) : m_XOffset(xOffset), m_YOffset(yOffset) {}

        /**
         * @brief get the x offset
         * @return x offset
         */
        inline float GetXOffset() const { return m_XOffset; }

        /**
         * @brief get the y offset
         * @return y offset
         */
        inline float GetYOffset() const { return m_YOffset; }

        /**
         * @brief convert to string
         * @return event as string
         */
        std::string ToString() const override {
            std::stringstream ss;
            ss << "MouseScrolledEvent: " << GetXOffset() << ", " << GetYOffset();
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseScrolled)
        EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
       private:
        /**
         * @brief x and y offset
         */
        float m_XOffset, m_YOffset;
    };

    /**
     * @class MouseButtonEvent
     * @brief mouse button event
     */
    class MouseButtonEvent : public Event {
       public:
        /**
         * @brief get the mouse button
         * @return mouse button
         */
        inline int GetMouseButton() const { return m_Button; }

        EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
       protected:
        /**
         * @brief constructor with button
         */
        MouseButtonEvent(int button) : m_Button(button) {}

        /**
         * @brief button of mouse
         */
        int m_Button;
    };

    /**
     * @class MouseButtonPressedEvent
     * @brief the mouse button pressed event
     */
    class MouseButtonPressedEvent : public MouseButtonEvent {
       public:
        /**
         * @brief mouse button pressed constructor
         * @param button pressed
         */
        MouseButtonPressedEvent(int button) : MouseButtonEvent(button) {}

        /**
         * @brief convert to string
         * @return event as string
         */
        std::string ToString() const override {
            std::stringstream ss;
            ss << "MouseButtonPressedEvent: " << m_Button;
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseButtonPressed)
    };

    /**
     * @class MouseButtonReleasedEvent
     * @brief the mouse button released
     */
    class MouseButtonReleasedEvent : public MouseButtonEvent {
       public:
        /**
         * @brief the mouse button released event
         * @param button released
         */
        MouseButtonReleasedEvent(int button) : MouseButtonEvent(button) {}

        /**
         * @brief convert to string
         * @return event as string
         */
        std::string ToString() const override {
            std::stringstream ss;
            ss << "MouseButtonReleasedEvent: " << m_Button;
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseButtonReleased)
    };
}  // namespace Vakol::Controller