#pragma once

#ifdef SD_PLATFORM_WINDOWS

extern Strand::Application* Strand::CreateApplication();

int main(int argc, char* argv)
{
	Strand::Log::Init();

	SD_PROFILE_BEGIN_SESSION("Startup", "StrandProfile-Startup.json");
	auto app = Strand::CreateApplication();
	SD_PROFILE_END_SESSION();

	SD_PROFILE_BEGIN_SESSION("Runtime", "StrandProfile-Runtime.json");
	app->Run();
	SD_PROFILE_END_SESSION();

	SD_PROFILE_BEGIN_SESSION("Shutdown", "StrandProfile-Shutdown.json");
	delete app;
	SD_PROFILE_END_SESSION();
}

#endif // SD_PLATFORM_WINDOWS
