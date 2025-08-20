#include "sdpch.h"
#include "Strand/Core/Window.h"

#ifdef SD_PLATFORM_WINDOWS
#include "Platform/Windows/WindowsWindow.h"
#endif

namespace Strand
{

	Scope<Window> Window::Create(const WindowProps& props)
	{
#ifdef SD_PLATFORM_WINDOWS
		return CreateScope<WindowsWindow>(props);
#else
		SD_CORE_ASSERT(false, "Unknown platform!");
		return nullptr;
#endif
	}

}