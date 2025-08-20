#pragma once

namespace Strand
{
	typedef enum class MouseCode : uint16_t
	{
		// From glfw3.h
		Button0 = 0,
		Button1 = 1,
		Button2 = 2,
		Button3 = 3,
		Button4 = 4,
		Button5 = 5,
		Button6 = 6,
		Button7 = 7,

		ButtonLast = Button7,
		ButtonLeft = Button0,
		ButtonRight = Button1,
		ButtonMiddle = Button2
	} Mouse;

	inline std::ostream& operator<<(std::ostream& os, MouseCode mouseCode)
	{
		os << static_cast<int32_t>(mouseCode);
		return os;
	}
}

#define SD_MOUSE_BUTTON_0      ::Strand::Mouse::Button0
#define SD_MOUSE_BUTTON_1      ::Strand::Mouse::Button1
#define SD_MOUSE_BUTTON_2      ::Strand::Mouse::Button2
#define SD_MOUSE_BUTTON_3      ::Strand::Mouse::Button3
#define SD_MOUSE_BUTTON_4      ::Strand::Mouse::Button4
#define SD_MOUSE_BUTTON_5      ::Strand::Mouse::Button5
#define SD_MOUSE_BUTTON_6      ::Strand::Mouse::Button6
#define SD_MOUSE_BUTTON_7      ::Strand::Mouse::Button7
#define SD_MOUSE_BUTTON_LAST   ::Strand::Mouse::ButtonLast
#define SD_MOUSE_BUTTON_LEFT   ::Strand::Mouse::ButtonLeft
#define SD_MOUSE_BUTTON_RIGHT  ::Strand::Mouse::ButtonRight
#define SD_MOUSE_BUTTON_MIDDLE ::Strand::Mouse::ButtonMiddle