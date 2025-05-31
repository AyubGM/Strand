#include <GMCore.h>

class Sandbox : public GMCore::Application
{
public:
	Sandbox()
	{ 
	}

	~Sandbox()
	{
	}


};

GMCore::Application* GMCore::CreateApplication()
{
	return new Sandbox();
}