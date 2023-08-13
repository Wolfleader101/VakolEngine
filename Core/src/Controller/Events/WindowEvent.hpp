#pragma once

#include <sstream>

#include "Event.hpp"

namespace Vakol::Controller
{
    /**
     * @class WindowResizeEvent
     * @brief the window resize event
     */
    class WindowResizeEvent : public Event
    {
      public:
        /**
         * @brief the consturctor for window resize
         * @param width resized too
         * @param height resized too
         */
        WindowResizeEvent(unsigned int width, unsigned int height) : m_Width(width), m_Height(height)
        {
        }

        /**
         * @brief get the width
         * @return width resized
         */
        inline unsigned int GetWidth() const
        {
            return m_Width;
        }

        /**
         * @brief get the height
         * @return height resized
         */
        inline unsigned int GetHeight() const
        {
            return m_Height;
        }

        /**
         * @brief convert to string
         * @return event as string
         */
        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "WindowResizeEvent: " << m_Width << ", " << m_Height;
            return ss.str();
        }

        EVENT_CLASS_TYPE(WindowResize)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)
      private:
        /**
         * @brief width and height of resize
         */
        unsigned int m_Width, m_Height;
    };

    /**
     * @class WindowCloseEvent
     * @brief window close event
     */
    class WindowCloseEvent : public Event
    {
      public:
        /**
         * @brief default constructor
         */
        WindowCloseEvent()
        {
        }

        EVENT_CLASS_TYPE(WindowClose)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    };
} // namespace Vakol::Controller