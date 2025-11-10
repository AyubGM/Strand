#pragma once

#include "Strand.h"
#include "EditorLayer.h"

namespace Strand {

	class ProjectHubLayer : public Layer
	{
	public:
		ProjectHubLayer(EditorLayer* editorLayer);
		virtual ~ProjectHubLayer() = default;

		virtual void OnImGuiRender() override;
	private:
		EditorLayer* m_EditorLayer;

	};
}