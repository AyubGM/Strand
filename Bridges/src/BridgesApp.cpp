#include <Strand.h>
#include <Strand/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace Strand {

	class StrandEditor : public Application
	{
	public:
		StrandEditor()
			: Application("Strand Editor")
		{
			PushLayer(new EditorLayer());
		}

		~StrandEditor()
		{
		}
	};

	Application* CreateApplication()
	{
		return new StrandEditor();
	}

}