#pragma once

#ifdef GM_PLATFORM_WINDOWS

extern GMCore::Application* GMCore::CreateApplication();

int main(int argc, char* argv)
{
	GMCore::Log::Init();
	GM_CORE_WARN("Initialized Log!"); 
	int a = 5;
	GM_INFO("Initialized Log! Var={0}", a);
	auto app = GMCore::CreateApplication();
	app->Run();
	delete app;
}

#endif // GM_PLATFORM_WINDOWS
