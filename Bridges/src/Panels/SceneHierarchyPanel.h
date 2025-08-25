#pragma once

#include "Strand/Core/Base.h"
#include "Strand/Core/Log.h"
#include "Strand/Scene/Scene.h"
#include "Strand/Scene/Entity.h"

namespace Strand {

	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& scene);

		void SetContext(const Ref<Scene>& scene);

		void OnImGuiRender();
	private:
		void DrawEntityNode(Entity entity);
	private:
		Ref<Scene> m_Context;
		Entity m_SelectionContext;
	};

}