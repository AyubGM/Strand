#include "sdpch.h"
#include "ContentBrowserPanel.h"

#include "Strand/Project/Project.h"
#include "Strand/Asset/TextureImporter.h"
#include "Strand/EventBus/EditorEventBus.h"
#include "Strand/Events/EditorEvents.h"
#include "ScriptEditorPanel.h"

#include <imgui/imgui.h>

namespace Strand {

	ContentBrowserPanel::ContentBrowserPanel(Ref<Project> project)
		: m_Project(project), m_ThumbnailCache(CreateRef<ThumbnailCache>(project)),
		m_BaseDirectory(m_Project->GetAssetDirectory()), m_CurrentDirectory(m_BaseDirectory)
	{
		m_TreeNodes.push_back(TreeNode(".", 0));

		m_DirectoryIcon = TextureImporter::LoadTexture2D("Resources/Icons/ContentBrowser/DirectoryIcon.png");
		m_FileIcon = TextureImporter::LoadTexture2D("Resources/Icons/ContentBrowser/FileIcon.png");

		RefreshAssetTree();

		m_Mode = Mode::FileSystem;
	}



	void ContentBrowserPanel::OnImGuiRender()
	{
		ImGui::Begin("Content Browser");

		const char* label = m_Mode == Mode::Asset ? "Asset" : "File";
		if (ImGui::Button(label))
		{
			m_Mode = m_Mode == Mode::Asset ? Mode::FileSystem : Mode::Asset;
		}

		if (m_CurrentDirectory != std::filesystem::path(m_BaseDirectory))
		{
			ImGui::SameLine();
			if (ImGui::Button("<-"))
			{
				m_CurrentDirectory = m_CurrentDirectory.parent_path();
			}
		}

		bool isScriptsDir = (m_CurrentDirectory.stem() == "Scripts");
		bool isSceneDir = (m_CurrentDirectory.stem() == "Scenes");

		if (isScriptsDir)
		{
			ImGui::SameLine();
			if (ImGui::Button("New Script"))
			{
				CreateNewScriptFile(m_CurrentDirectory, "NewScript.cs");
			}
		}
		if (isSceneDir)
		{
			ImGui::SameLine();
			if (ImGui::Button("New Scene"))
			{
				CreateNewSceneFile(m_CurrentDirectory, "NewScene.strand");
			}
		}


		static float padding = 16.0f;
		static float thumbnailSize = 128.0f;
		float cellSize = thumbnailSize + padding;

		float panelWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = (int)(panelWidth / cellSize);
		if (columnCount < 1)
			columnCount = 1;

		ImGui::Columns(columnCount, 0, false);

		if (m_Mode == Mode::Asset)
		{
			TreeNode* node = &m_TreeNodes[0];

			auto currentDir = std::filesystem::relative(m_CurrentDirectory, Project::GetActiveAssetDirectory());
			for (const auto& p : currentDir)
			{
				// if only one level
				if (node->Path == currentDir)
					break;

				if (node->Children.find(p) != node->Children.end())
				{
					node = &m_TreeNodes[node->Children[p]];
					continue;
				}
				else
				{
					// can't find path
					SD_CORE_ASSERT(false);
				}

			}

			for (const auto& [item, treeNodeIndex] : node->Children)
			{
				bool isDirectory = std::filesystem::is_directory(Project::GetActiveAssetDirectory() / item);

				std::string itemStr = item.generic_string();

				ImGui::PushID(itemStr.c_str());
				Ref<Texture2D> icon = isDirectory ? m_DirectoryIcon : m_FileIcon;
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
				ImGui::ImageButton(itemStr.c_str(), (ImTextureID)icon->GetRendererID(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });

				if (ImGui::BeginPopupContextItem())
				{
					if (ImGui::MenuItem("Delete"))
					{
						SD_CORE_ASSERT(false, "Not implemented");
					}
					ImGui::EndPopup();
				}

				if (ImGui::BeginDragDropSource())
				{
					AssetHandle handle = m_TreeNodes[treeNodeIndex].Handle;
					ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", &handle, sizeof(AssetHandle));
					ImGui::EndDragDropSource();
				}


				ImGui::PopStyleColor();
				if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
				{
					if (isDirectory)
						m_CurrentDirectory /= item.filename();
					else
					{
						// Updated logic: Dispatch event
						if (item.extension() == ".cs")
						{
							ScriptFileOpenedEvent event(m_CurrentDirectory / item);
							EditorEventBus::Dispatch(event);
						}
					}
					
				}

				ImGui::TextWrapped(itemStr.c_str());

				ImGui::NextColumn();

				ImGui::PopID();
			}
		}
		else
		{
			for (auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory))
			{
				const auto& path = directoryEntry.path();
				std::string filenameString = path.filename().string();

				ImGui::PushID(filenameString.c_str());

				// THUMBNAIL
				auto relativePath = std::filesystem::relative(path, Project::GetActiveAssetDirectory());
				Ref<Texture2D> thumbnail = m_DirectoryIcon;
				if (!directoryEntry.is_directory())
				{
					thumbnail = m_ThumbnailCache->GetOrCreateThumbnail(relativePath);
					if (!thumbnail)
						thumbnail = m_FileIcon;
				}

				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
				ImGui::ImageButton(filenameString.c_str(), (ImTextureID)thumbnail->GetRendererID(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });

				if (ImGui::BeginPopupContextItem())
				{
					if (ImGui::MenuItem("Import"))
					{
						Project::GetActive()->GetEditorAssetManager()->ImportAsset(relativePath);
						RefreshAssetTree();
					}

					ImGui::EndPopup();
				}

				ImGui::PopStyleColor();
				if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
				{
					if (directoryEntry.is_directory())
						m_CurrentDirectory /= path.filename();
					else
					{
						// Updated logic: Dispatch event
						if (path.extension() == ".cs")
						{
							ScriptFileOpenedEvent event(path);
							EditorEventBus::Dispatch(event);
						}
					}
					
				}


			

			/*	if (ImGui::BeginPopupContextWindow("ContentBrowserPopup", ImGuiPopupFlags_NoOpenOverItems))
				{
					if (ImGui::MenuItem("New Script"))
					{
						CreateNewScriptFile(m_CurrentDirectory, "NewScript.cs");
					}
					ImGui::EndPopup();
				}*/



				ImGui::TextWrapped(filenameString.c_str());

				ImGui::NextColumn();

				ImGui::PopID();


		
	

			}
		}

		// Right-click on empty space: open our popup explicitly (prevents accidental left-click opens)
		if (ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup) && ImGui::IsMouseClicked(ImGuiMouseButton_Right))
			ImGui::OpenPopup("ContentBrowserPopup");

		if (ImGui::BeginPopup("ContentBrowserPopup"))
		{
			if (isScriptsDir && ImGui::MenuItem("New Script"))
			{
				CreateNewScriptFile(m_CurrentDirectory, "NewScript.cs");
			}
			if (isSceneDir && ImGui::MenuItem("New Scene"))
			{
				CreateNewSceneFile(m_CurrentDirectory, "NewScene.scene");
			}
			ImGui::EndPopup();
		}

		ImGui::Columns(1);

		ImGui::SliderFloat("Thumbnail Size", &thumbnailSize, 16, 512);
		ImGui::SliderFloat("Padding", &padding, 0, 32);

		// TODO: status bar
		ImGui::End();
	}

	void ContentBrowserPanel::RefreshAssetTree()
	{
		const auto& assetRegistry = Project::GetActive()->GetEditorAssetManager()->GetAssetRegistry();
		for (const auto& [handle, metadata] : assetRegistry)
		{
			uint32_t currentNodeIndex = 0;

			for (const auto& p : metadata.FilePath)
			{
				auto it = m_TreeNodes[currentNodeIndex].Children.find(p.generic_string());
				if (it != m_TreeNodes[currentNodeIndex].Children.end())
				{
					currentNodeIndex = it->second;
				}
				else
				{
					// add node
					TreeNode newNode(p, handle);
					newNode.Parent = currentNodeIndex;
					m_TreeNodes.push_back(newNode);

					m_TreeNodes[currentNodeIndex].Children[p] = m_TreeNodes.size() - 1;
					currentNodeIndex = m_TreeNodes.size() - 1;
				}

			}
		}
	}


	void ContentBrowserPanel::CreateNewScriptFile(const std::filesystem::path& directory, const std::string& filename)
	{
		Project::CreateScriptFile(directory, filename);

		// Refresh so the new file shows up
		RefreshAssetTree();
	}


	void ContentBrowserPanel::CreateNewSceneFile(const std::filesystem::path& directory, const std::string& filename)
	{
		Project::CreateSceneFile(directory, filename);

		// Refresh so the new file shows up
		RefreshAssetTree();
	}
}