#include <Strand.h>
#include <Strand/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace Strand {

	class StrandEditor : public Application
	{
	public:
		StrandEditor(ApplicationCommandLineArgs args)
			: Application("Strand Editor", args)
		{
			PushLayer(new EditorLayer());
		}

		~StrandEditor()
		{
		}
	};

	Application* CreateApplication(ApplicationCommandLineArgs args)
	{
		return new StrandEditor(args);
	}

}