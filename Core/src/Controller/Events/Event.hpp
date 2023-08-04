#pragma once

#include <string>
#include <functional>


namespace Vakol::Controller
{
#define BIT(x) (1 << x)

	/**
	 * @brief the type of event
	*/
	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		KeyPressed,	KeyReleased, 
		MouseButtonPressed, MouseButtonReleased,MouseMoved,	MouseScrolled
	};

	/**
	 * @brief the cateogry for the event
	*/
	enum EventCategory
	{
		None = 0,
		EventCategoryApplication = BIT(0),
		EventCategoryInput = BIT(1),
		EventCategoryKeyboard = BIT(2),
		EventCategoryMouse = BIT(3),
		EventCategoryMouseButton = BIT(4)
	};

#define EVENT_CLASS_TYPE(type)                                                  \
	static EventType GetStaticType() { return EventType::type; }              \
	virtual EventType GetEventType() const override { return GetStaticType(); } \
	virtual const char *GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) \
	virtual int GetCategoryFlags() const override { return category; }

	/**
	 * @class Event 
	 * @brief the base event class
	*/
	class Event
	{
	public:
		/**
		 * @brief if the event has been handled or not
		*/
		bool Handled = false;

		/**
		 * @brief get the event type
		 * @return type of event
		*/
		virtual EventType GetEventType() const = 0;

		/**
		 * @brief get the name of the event
		 * @return name of event
		*/
		virtual const char *GetName() const = 0;

		/**
		 * @brief get the categories of the event
		 * @return 
		*/
		virtual int GetCategoryFlags() const = 0;

		/**
		 * @brief convert the event to string
		 * @return the event as a string
		*/
		virtual std::string ToString() const { return GetName(); }

		/**
		 * @brief check if the event is in an category
		 * @param category to check
		 * @return if in category
		*/
		inline bool IsInCategory(EventCategory category)
		{
			return GetCategoryFlags() & category;
		}

	private:
	};

	/**
	 * @class EventDispatcher
	 * @brief the event dispatcher class to handle events
	*/
	class EventDispatcher
	{
		template <typename T>
		using EventFn = std::function<bool(T &)>;

	public:
		/**
		 * @brief default constructor
		 * @param event to dispatch
		*/
		EventDispatcher(Event &event)
			: m_Event(event) {}

		/**
		 * @brief distatch an event
		 * @tparam T type of event
		 * @param func callback function to handle event
		 * @return  true if handled, else false
		*/
		template <typename T>
		bool Dispatch(EventFn<T> func)
		{
			if (m_Event.GetEventType() == T::GetStaticType())
			{
				m_Event.Handled = func(*(T *)&m_Event);
				return true;
			}
			return false;
		}

	private:
		/**
		 * @brief the event to dispatch
		*/
		Event &m_Event;
	};

	inline std::ostream &operator<<(std::ostream &os, const Event &e)
	{
		return os << e.ToString();
	}
}