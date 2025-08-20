#include <Strand.h>
#include <Strand/Core/EntryPoint.h>

#include "Sandbox2D.h"
#include "ExampleLayer.h"


class Sandbox : public Strand::Application
{
public:
	Sandbox()
	{ 
		//PushLayer(new ExampleLayer());
		PushLayer(new Sandbox2D());
		//PushLayer(new GameLayer());
	}

	~Sandbox()
	{
	}


};

Strand::Application* Strand::CreateApplication()
{
	return new Sandbox();
}