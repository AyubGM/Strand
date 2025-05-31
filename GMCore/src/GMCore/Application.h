#pragma once

#include "Core.h"

namespace GMCore {

	class GMCORE_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}



