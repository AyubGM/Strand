#pragma once

#include "Strand/Core/Base.h"
#include "Strand/Core/KeyCodes.h"
#include "Strand/Core/MouseCodes.h"

namespace Strand {

	class Input
	{
	public:
		static bool IsKeyPressed(KeyCode key);

		static bool IsMouseButtonPressed(MouseCode button);
		static std::pair<float, float> GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};
}