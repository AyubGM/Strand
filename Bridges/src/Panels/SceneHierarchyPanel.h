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

		Entity GetSelectedEntity() const { return m_SelectionContext; }
		void SetSelectedEntity(Entity entity);
	private:
		template<typename T>
		void DisplayAddComponentEntry(const std::string& entryName);

		//void DrawEntityNode(Entity entity);
		void DrawEntityNode(Entity entity, std::unordered_set<UUID>* visited = nullptr);

		void DrawComponents(Entity entity);
	private:
		Ref<Scene> m_Context;
		Entity m_SelectionContext;
	};

}