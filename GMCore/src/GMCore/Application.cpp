#include "gmcpch.h"
#include "Application.h"
#include "Events/ApplicationEvent.h"
#include "Log.h"
namespace GMCore {
	Application::Application()
	{
	}
	Application::~Application()
	{
	}
	void Application::Run()
	{
		WindowResizeEvent resizeEvent(1280, 720);
		GM_TRACE(resizeEvent.ToString());
		while (true);
	}
}