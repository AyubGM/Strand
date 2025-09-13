#include <Strand.h>
#include <Strand/Core/EntryPoint.h>

#include "Sandbox2D.h"
#include "Sandbox3D.h"
#include "ExampleLayer.h"
#include "Game1/GameLayer.h"


class Sandbox : public Strand::Application
{
public:
	Sandbox(const Strand::ApplicationSpecification& specification)
		: Strand::Application(specification)
	{ 
		//PushLayer(new ExampleLayer());
		//PushLayer(new Sandbox2D());
		PushLayer(new Sandbox3D());
		//PushLayer(new GameLayer());
	}

	~Sandbox()
	{
	}


};

Strand::Application* Strand::CreateApplication(ApplicationCommandLineArgs args)
{
	ApplicationSpecification spec;
	spec.Name = "Sandbox";
	spec.WorkingDirectory = "../Bridges";
	spec.CommandLineArgs = args;

	return new Sandbox(spec);
}