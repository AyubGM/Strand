#include <Strand.h>
#include <Strand/Core/EntryPoint.h>

#include "EditorLayer.h"
#include "ProjectHubLayer.h"

namespace Strand {

	class Bridges : public Application
	{
	public:
		Bridges(const ApplicationSpecification& spec)
			: Application(spec)
		{
			m_EditorLayer = new EditorLayer();
			PushLayer(m_EditorLayer);

			PushOverlay(new ProjectHubLayer(m_EditorLayer));
		}
	private:
		EditorLayer* m_EditorLayer;
		
	};

	Application* CreateApplication(ApplicationCommandLineArgs args)
	{
		ApplicationSpecification spec;
		spec.Name = "Bridges";
		spec.CommandLineArgs = args;

		return new Bridges(spec);
	}

}