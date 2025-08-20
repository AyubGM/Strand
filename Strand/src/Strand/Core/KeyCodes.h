#pragma once

namespace Strand
{
	typedef enum class KeyCode : uint16_t
	{
		// From glfw3.h
		Space = 32,
		Apostrophe = 39, /* ' */
		Comma = 44, /* , */
		Minus = 45, /* - */
		Period = 46, /* . */
		Slash = 47, /* / */

		D0 = 48, /* 0 */
		D1 = 49, /* 1 */
		D2 = 50, /* 2 */
		D3 = 51, /* 3 */
		D4 = 52, /* 4 */
		D5 = 53, /* 5 */
		D6 = 54, /* 6 */
		D7 = 55, /* 7 */
		D8 = 56, /* 8 */
		D9 = 57, /* 9 */

		Semicolon = 59, /* ; */
		Equal = 61, /* = */

		A = 65,
		B = 66,
		C = 67,
		D = 68,
		E = 69,
		F = 70,
		G = 71,
		H = 72,
		I = 73,
		J = 74,
		K = 75,
		L = 76,
		M = 77,
		N = 78,
		O = 79,
		P = 80,
		Q = 81,
		R = 82,
		S = 83,
		T = 84,
		U = 85,
		V = 86,
		W = 87,
		X = 88,
		Y = 89,
		Z = 90,

		LeftBracket = 91,  /* [ */
		Backslash = 92,  /* \ */
		RightBracket = 93,  /* ] */
		GraveAccent = 96,  /* ` */

		World1 = 161, /* non-US #1 */
		World2 = 162, /* non-US #2 */

		/* Function keys */
		Escape = 256,
		Enter = 257,
		Tab = 258,
		Backspace = 259,
		Insert = 260,
		Delete = 261,
		Right = 262,
		Left = 263,
		Down = 264,
		Up = 265,
		PageUp = 266,
		PageDown = 267,
		Home = 268,
		End = 269,
		CapsLock = 280,
		ScrollLock = 281,
		NumLock = 282,
		PrintScreen = 283,
		Pause = 284,
		F1 = 290,
		F2 = 291,
		F3 = 292,
		F4 = 293,
		F5 = 294,
		F6 = 295,
		F7 = 296,
		F8 = 297,
		F9 = 298,
		F10 = 299,
		F11 = 300,
		F12 = 301,
		F13 = 302,
		F14 = 303,
		F15 = 304,
		F16 = 305,
		F17 = 306,
		F18 = 307,
		F19 = 308,
		F20 = 309,
		F21 = 310,
		F22 = 311,
		F23 = 312,
		F24 = 313,
		F25 = 314,

		/* Keypad */
		KP0 = 320,
		KP1 = 321,
		KP2 = 322,
		KP3 = 323,
		KP4 = 324,
		KP5 = 325,
		KP6 = 326,
		KP7 = 327,
		KP8 = 328,
		KP9 = 329,
		KPDecimal = 330,
		KPDivide = 331,
		KPMultiply = 332,
		KPSubtract = 333,
		KPAdd = 334,
		KPEnter = 335,
		KPEqual = 336,

		LeftShift = 340,
		LeftControl = 341,
		LeftAlt = 342,
		LeftSuper = 343,
		RightShift = 344,
		RightControl = 345,
		RightAlt = 346,
		RightSuper = 347,
		Menu = 348
	} Key;

	inline std::ostream& operator<<(std::ostream& os, KeyCode keyCode)
	{
		os << static_cast<int32_t>(keyCode);
		return os;
	}
}

// From glfw3.h
#define SD_KEY_SPACE           ::Strand::Key::Space
#define SD_KEY_APOSTROPHE      ::Strand::Key::Apostrophe    /* ' */
#define SD_KEY_COMMA           ::Strand::Key::Comma         /* , */
#define SD_KEY_MINUS           ::Strand::Key::Minus         /* - */
#define SD_KEY_PERIOD          ::Strand::Key::Period        /* . */
#define SD_KEY_SLASH           ::Strand::Key::Slash         /* / */
#define SD_KEY_0               ::Strand::Key::D0
#define SD_KEY_1               ::Strand::Key::D1
#define SD_KEY_2               ::Strand::Key::D2
#define SD_KEY_3               ::Strand::Key::D3
#define SD_KEY_4               ::Strand::Key::D4
#define SD_KEY_5               ::Strand::Key::D5
#define SD_KEY_6               ::Strand::Key::D6
#define SD_KEY_7               ::Strand::Key::D7
#define SD_KEY_8               ::Strand::Key::D8
#define SD_KEY_9               ::Strand::Key::D9
#define SD_KEY_SEMICOLON       ::Strand::Key::Semicolon     /* ; */
#define SD_KEY_EQUAL           ::Strand::Key::Equal         /* = */
#define SD_KEY_A               ::Strand::Key::A
#define SD_KEY_B               ::Strand::Key::B
#define SD_KEY_C               ::Strand::Key::C
#define SD_KEY_D               ::Strand::Key::D
#define SD_KEY_E               ::Strand::Key::E
#define SD_KEY_F               ::Strand::Key::F
#define SD_KEY_G               ::Strand::Key::G
#define SD_KEY_H               ::Strand::Key::H
#define SD_KEY_I               ::Strand::Key::I
#define SD_KEY_J               ::Strand::Key::J
#define SD_KEY_K               ::Strand::Key::K
#define SD_KEY_L               ::Strand::Key::L
#define SD_KEY_M               ::Strand::Key::M
#define SD_KEY_N               ::Strand::Key::N
#define SD_KEY_O               ::Strand::Key::O
#define SD_KEY_P               ::Strand::Key::P
#define SD_KEY_Q               ::Strand::Key::Q
#define SD_KEY_R               ::Strand::Key::R
#define SD_KEY_S               ::Strand::Key::S
#define SD_KEY_T               ::Strand::Key::T
#define SD_KEY_U               ::Strand::Key::U
#define SD_KEY_V               ::Strand::Key::V
#define SD_KEY_W               ::Strand::Key::W
#define SD_KEY_X               ::Strand::Key::X
#define SD_KEY_Y               ::Strand::Key::Y
#define SD_KEY_Z               ::Strand::Key::Z
#define SD_KEY_LEFT_BRACKET    ::Strand::Key::LeftBracket   /* [ */
#define SD_KEY_BACKSLASH       ::Strand::Key::Backslash     /* \ */
#define SD_KEY_RIGHT_BRACKET   ::Strand::Key::RightBracket  /* ] */
#define SD_KEY_GRAVE_ACCENT    ::Strand::Key::GraveAccent   /* ` */
#define SD_KEY_WORLD_1         ::Strand::Key::World1        /* non-US #1 */
#define SD_KEY_WORLD_2         ::Strand::Key::World2        /* non-US #2 */

/* Function keys */
#define SD_KEY_ESCAPE          ::Strand::Key::Escape
#define SD_KEY_ENTER           ::Strand::Key::Enter
#define SD_KEY_TAB             ::Strand::Key::Tab
#define SD_KEY_BACKSPACE       ::Strand::Key::Backspace
#define SD_KEY_INSERT          ::Strand::Key::Insert
#define SD_KEY_DELETE          ::Strand::Key::Delete
#define SD_KEY_RIGHT           ::Strand::Key::Right
#define SD_KEY_LEFT            ::Strand::Key::Left
#define SD_KEY_DOWN            ::Strand::Key::Down
#define SD_KEY_UP              ::Strand::Key::Up
#define SD_KEY_PAGE_UP         ::Strand::Key::PageUp
#define SD_KEY_PAGE_DOWN       ::Strand::Key::PageDown
#define SD_KEY_HOME            ::Strand::Key::Home
#define SD_KEY_END             ::Strand::Key::End
#define SD_KEY_CAPS_LOCK       ::Strand::Key::CapsLock
#define SD_KEY_SCROLL_LOCK     ::Strand::Key::ScrollLock
#define SD_KEY_NUM_LOCK        ::Strand::Key::NumLock
#define SD_KEY_PRINT_SCREEN    ::Strand::Key::PrintScreen
#define SD_KEY_PAUSE           ::Strand::Key::Pause
#define SD_KEY_F1              ::Strand::Key::F1
#define SD_KEY_F2              ::Strand::Key::F2
#define SD_KEY_F3              ::Strand::Key::F3
#define SD_KEY_F4              ::Strand::Key::F4
#define SD_KEY_F5              ::Strand::Key::F5
#define SD_KEY_F6              ::Strand::Key::F6
#define SD_KEY_F7              ::Strand::Key::F7
#define SD_KEY_F8              ::Strand::Key::F8
#define SD_KEY_F9              ::Strand::Key::F9
#define SD_KEY_F10             ::Strand::Key::F10
#define SD_KEY_F11             ::Strand::Key::F11
#define SD_KEY_F12             ::Strand::Key::F12
#define SD_KEY_F13             ::Strand::Key::F13
#define SD_KEY_F14             ::Strand::Key::F14
#define SD_KEY_F15             ::Strand::Key::F15
#define SD_KEY_F16             ::Strand::Key::F16
#define SD_KEY_F17             ::Strand::Key::F17
#define SD_KEY_F18             ::Strand::Key::F18
#define SD_KEY_F19             ::Strand::Key::F19
#define SD_KEY_F20             ::Strand::Key::F20
#define SD_KEY_F21             ::Strand::Key::F21
#define SD_KEY_F22             ::Strand::Key::F22
#define SD_KEY_F23             ::Strand::Key::F23
#define SD_KEY_F24             ::Strand::Key::F24
#define SD_KEY_F25             ::Strand::Key::F25

/* Keypad */
#define SD_KEY_KP_0            ::Strand::Key::KP0
#define SD_KEY_KP_1            ::Strand::Key::KP1
#define SD_KEY_KP_2            ::Strand::Key::KP2
#define SD_KEY_KP_3            ::Strand::Key::KP3
#define SD_KEY_KP_4            ::Strand::Key::KP4
#define SD_KEY_KP_5            ::Strand::Key::KP5
#define SD_KEY_KP_6            ::Strand::Key::KP6
#define SD_KEY_KP_7            ::Strand::Key::KP7
#define SD_KEY_KP_8            ::Strand::Key::KP8
#define SD_KEY_KP_9            ::Strand::Key::KP9
#define SD_KEY_KP_DECIMAL      ::Strand::Key::KPDecimal
#define SD_KEY_KP_DIVIDE       ::Strand::Key::KPDivide
#define SD_KEY_KP_MULTIPLY     ::Strand::Key::KPMultiply
#define SD_KEY_KP_SUBTRACT     ::Strand::Key::KPSubtract
#define SD_KEY_KP_ADD          ::Strand::Key::KPAdd
#define SD_KEY_KP_ENTER        ::Strand::Key::KPEnter
#define SD_KEY_KP_EQUAL        ::Strand::Key::KPEqual

#define SD_KEY_LEFT_SHIFT      ::Strand::Key::LeftShift
#define SD_KEY_LEFT_CONTROL    ::Strand::Key::LeftControl
#define SD_KEY_LEFT_ALT        ::Strand::Key::LeftAlt
#define SD_KEY_LEFT_SUPER      ::Strand::Key::LeftSuper
#define SD_KEY_RIGHT_SHIFT     ::Strand::Key::RightShift
#define SD_KEY_RIGHT_CONTROL   ::Strand::Key::RightControl
#define SD_KEY_RIGHT_ALT       ::Strand::Key::RightAlt
#define SD_KEY_RIGHT_SUPER     ::Strand::Key::RightSuper
#define SD_KEY_MENU            ::Strand::Key::Menu