#pragma once

#include "Event.h"
#include <sstream>


// This file defines mouse events for the GMCore framework.

namespace GMCore {

	class GMCORE_API MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(float x, float y)
			: m_MouseX(x), m_MouseY(y) {
		}
		inline float GetX() const { return m_MouseX; }
		inline float GetY() const { return m_MouseY; }

		std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseMovedEvent: " << m_MouseX << ", " << m_MouseY;
			return ss.str();
		}
		EVENT_CLASS_TYPE(MouseMoved)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	private:
		float m_MouseX, m_MouseY; // The X and Y coordinates of the mouse event
	};

	class GMCORE_API MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(float xOffset, float yOffset)
			: m_XOffset(xOffset), m_YOffset(yOffset) {
		}
		inline float GetXOffset() const { return m_XOffset; }
		inline float GetYOffset() const { return m_YOffset; }
		std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseScrolledEvent: " << GetXOffset() << ", " << GetYOffset();
			return ss.str();
		}
		EVENT_CLASS_TYPE(MouseScrolled)
			EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	private:
		float m_XOffset, m_YOffset; // The X and Y offsets of the scroll event
	};

	class GMCORE_API MouseButtonEvent : public Event
	{
	public:
		inline int GetMouseButton() const { return m_Button; }
		EVENT_CLASS_CATEGORY(EventCategoryMouseButton | EventCategoryInput)
			
	protected:
		MouseButtonEvent(int button)
			: m_Button(button) {
		}
		int m_Button; // The mouse button that was pressed or released

	};

	class GMCORE_API MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(int button)
			: MouseButtonEvent(button) {
		}
		std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseButtonPressedEvent: " << m_Button;
			return ss.str();
		}
		EVENT_CLASS_TYPE(MouseButtonPressed)


	};
	class GMCORE_API MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(int button)
			: MouseButtonEvent(button) {
		}
		std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent: " << m_Button;
			return ss.str();
		}
		EVENT_CLASS_TYPE(MouseButtonReleased)
	};
	
} // namespace GMCore




// Uncomment the following code to define mouse events

//class GMCORE_API MouseEvent : public Event
//{
//public:
//	inline float GetX() const { return m_X; }
//	inline float GetY() const { return m_Y; }
//	EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
//protected:
//	MouseEvent(float x, float y)
//		: m_X(x), m_Y(y) {
//	}
//	float m_X, m_Y; // The X and Y coordinates of the mouse event
//};
//class GMCORE_API MouseMovedEvent : public MouseEvent
//{
//public:
//	MouseMovedEvent(float x, float y)
//		: MouseEvent(x, y) {
//	}
//	std::string ToString() const override {
//		std::stringstream ss;
//		ss << "MouseMovedEvent: " << m_X << ", " << m_Y;
//		return ss.str();
//	}
//	EVENT_CLASS_TYPE(MouseMoved)
//};
//class GMCORE_API MouseScrolledEvent : public MouseEvent
//{
//public:
//	MouseScrolledEvent(float xOffset, float yOffset)
//		: MouseEvent(xOffset, yOffset), m_XOffset(xOffset), m_YOffset(yOffset) {
//	}
//
//	inline float GetXOffset() const { return m_XOffset; }
//	inline float GetYOffset() const { return m_YOffset; }
//	std::string ToString() const override {
//		std::stringstream ss;
//		ss << "MouseScrolledEvent: " << m_XOffset << ", " << m_YOffset;
//		return ss.str();
//	}
//
//	EVENT_CLASS_TYPE(MouseScrolled)
//private:
//	float m_XOffset, m_YOffset; // The X and Y offsets of the scroll event
//};