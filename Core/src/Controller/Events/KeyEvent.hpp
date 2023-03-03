#pragma once

#include "Event.hpp"

#include <sstream>

namespace Vakol::Controller
{
	/**
	 * @class KeyEvent
	 * @brief a key event
	*/
	class KeyEvent : public Event
	{
	public:
		/**
		 * @brief get the keycode
		 * @return keycode
		*/
		inline int GetKeyCode() const { return m_KeyCode; }

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
	protected:
		/**
		 * @brief constructor with keycode
		*/
		KeyEvent(int keycode)
			: m_KeyCode(keycode) {}

		/**
		 * @brief keycode of the event
		*/
		int m_KeyCode;
	};

	/**
	 * @class KeyPressedEvent
	 * @brief key pressed event
	*/
	class KeyPressedEvent : public KeyEvent
	{
	public:
		/**
		 * @brief key pressed event constructor
		 * @param keycode that was pressed
		 * @param repeatCount how many times its pressed
		*/
		KeyPressedEvent(int keycode, int repeatCount)
			: KeyEvent(keycode), m_RepeatCount(repeatCount) {}

		/**
		 * @brief get the repeat count
		 * @return repeat count
		*/
		inline int GetRepeatCount() const { return m_RepeatCount; }

		/**
		 * @brief convert to string
		 * @return as string
		*/
		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_KeyCode << " (" << m_RepeatCount << " repeats)";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)
	private:
		/**
		 * @brief how many times it was repeated
		*/
		int m_RepeatCount;
	};

	/**
	 * @class KeyReleasedEvent
	 * @brief key released event
	*/
	class KeyReleasedEvent : public KeyEvent
	{
	public:
		/**
		 * @brief constructor with key
		 * @param keycode of released key
		*/
		KeyReleasedEvent(int keycode)
			: KeyEvent(keycode) {}

		/**
		 * @brief convert to string
		 * @return event as string
		*/
		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << m_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)
	};
}