#pragma once

#ifdef SD_PLATFORM_WINDOWS

extern Strand::Application* Strand::CreateApplication();

int main(int argc, char* argv)
{
	Strand::Log::Init();
	SD_CORE_WARN("Initialized Log!"); 
	int a = 5;
	SD_INFO("Initialized Log! Var={0}", a);
	auto app = Strand::CreateApplication();
	app->Run();
	delete app;
}

#endif // SD_PLATFORM_WINDOWS
