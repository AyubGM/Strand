#include "SceneHierarchyPanel.h"

#include <imgui/imgui.h>

#include "Strand/Scene/Components.h"

namespace Strand {

	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context)
	{
		SetContext(context);
	}

	void SceneHierarchyPanel::SetContext(const Ref<Scene>& context)
	{
		m_Context = context;
	}

    void SceneHierarchyPanel::OnImGuiRender()
    {
		ImGui::Begin("Scene Hierarchy");

		// Iterate all entities by looping over the registry's storage
		auto view = m_Context->m_Registry.view<TagComponent>();
		for (auto entityID : view)
		{
			Entity entity{ entityID , m_Context.get() };
			DrawEntityNode(entity);
		}

		ImGui::End();
    }

	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		auto& tag = entity.GetComponent<TagComponent>().Tag;

		ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
		if (ImGui::IsItemClicked())
		{
			m_SelectionContext = entity;
		}

		if (opened)
		{
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
			bool opened = ImGui::TreeNodeEx((void*)9817239, flags, tag.c_str());
			if (opened)
				ImGui::TreePop();
			ImGui::TreePop();
		}

	}

}