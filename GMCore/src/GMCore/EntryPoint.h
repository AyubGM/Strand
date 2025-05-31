#pragma once

#ifdef GM_PLATFORM_WINDOWS

extern GMCore::Application* GMCore::CreateApplication();

int main(int argc, char* argv)
{
	auto app = GMCore::CreateApplication();
	app->Run();
	delete app;
}

#endif // GM_PLATFORM_WINDOWS
