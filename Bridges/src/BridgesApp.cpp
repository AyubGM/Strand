#include <Strand.h>
#include <Strand/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace Strand {

	class Bridges : public Application
	{
	public:
		Bridges(const ApplicationSpecification& spec)
			: Application(spec)
		{
			PushLayer(new EditorLayer());
		}

		
	};

	Application* CreateApplication(ApplicationCommandLineArgs args)
	{
		ApplicationSpecification spec;
		spec.Name = "Bridges";
		spec.CommandLineArgs = args;

		return new Bridges(spec);
	}

}