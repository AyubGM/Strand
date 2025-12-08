#include "EditorLayer.h"

#include "Utils/EditorSettings.h"

#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Strand/Scene/SceneSerializer.h"

#include "Strand/Utils/PlatformUtils.h"
#include "Strand/Scripting/ScriptEngine.h"

#include "Strand/Asset/AssetManager.h"
#include "Strand/Asset/TextureImporter.h"
#include "Strand/Asset/SceneImporter.h"

#include "Strand/Math/Math.h"
#include "Strand/EventBus/EditorEventBus.h"

#include "ImGuizmo.h"

namespace Strand {


	EditorLayer::EditorLayer()
		: Layer("EditorLayer"), m_CameraController(1280.0f / 720.0f), m_SquareColor({ 0.2f, 0.3f, 0.8f, 1.0f })
	{
		m_ScriptEditorPanel = CreateScope<ScriptEditorPanel>();
		//m_ScriptEditorPanel->OpenFile(Project::GetActiveAssetDirectory());
	}

	void EditorLayer::OnAttach()
	{
		SD_PROFILE_FUNCTION();

		EditorEventBus::Subscribe(EventType::ProjectOpened, SD_BIND_EVENT_FN(EditorLayer::OnProjectOpened));
		EditorEventBus::Subscribe(EventType::NewProjectRequest, SD_BIND_EVENT_FN(EditorLayer::OnNewProjectRequest));

		// Subscribe to the script saved event
		EditorEventBus::Subscribe(EventType::ScriptFileSaved, SD_BIND_EVENT_FN(EditorLayer::OnScriptFileSaved));

		m_ScriptEditorPanel->OnAttach();

		m_CheckerboardTexture = TextureImporter::LoadTexture2D("assets/textures/Checkerboard.png");
		m_IconPlay = TextureImporter::LoadTexture2D("Resources/Icons/PlayButton.png");
		m_IconPause = TextureImporter::LoadTexture2D("Resources/Icons/PauseButton.png");
		m_IconSimulate = TextureImporter::LoadTexture2D("Resources/Icons/SimulateButton.png");
		m_IconStep = TextureImporter::LoadTexture2D("Resources/Icons/StepButton.png");
		m_IconStop = TextureImporter::LoadTexture2D("Resources/Icons/StopButton.png");

		FramebufferSpecification fbSpec;
		fbSpec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		m_Framebuffer = Framebuffer::Create(fbSpec);

		// SUS
		m_EditorScene = CreateRef<Scene>();
		m_ActiveScene = m_EditorScene;

		//m_ActiveScene = CreateRef<Scene>();

		m_EditorCamera = EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);

		//auto commandLineArgs = Application::Get().GetSpecification().CommandLineArgs;
		//if (commandLineArgs.Count > 1)
		//{
		//	auto projectFilePath = commandLineArgs[1];
		//	OpenProject(projectFilePath);
		//}
		//else
		//{
		//	// TODO: prompt the user to select a directory
		//	// NewProject();

		//	// If no project is opened, close Bridges
		//	// NOTE: this is while we don't have a new project path
		//	if (!OpenProject())
		//		Application::Get().Close();
		//}


		Renderer2D::SetLineWidth(4.0f);

	}

	void EditorLayer::OnDetach()
	{
		SD_PROFILE_FUNCTION();
	}

	void EditorLayer::OnUpdate(Timestep ts)
	{
		SD_PROFILE_FUNCTION();

		// Don't update editor if no project is open
		if (!m_ProjectOpen)
			return;

		m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);

		// Resize
		if (FramebufferSpecification spec = m_Framebuffer->GetSpecification();
			m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && // zero sized framebuffer is invalid
			(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
		{
			m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_CameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);

			m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
			
		}

		// Render
		Renderer2D::ResetStats();
		{
			SD_PROFILE_SCOPE("Renderer Prep");
			m_Framebuffer->Bind();
			RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
			RenderCommand::Clear();

			// Clear our entity ID attachment to -1
			m_Framebuffer->ClearAttachment(1, -1);
		}

		// Update scene
		switch (m_SceneState)
		{
		case SceneState::Edit:
		{
			//SUS
			if (m_ViewportFocused)
				m_CameraController.OnUpdate(ts);

			m_EditorCamera.OnUpdate(ts);

			//Just Render
			m_ActiveScene->OnUpdateEditor(ts, m_EditorCamera);
			break;
		}
		case SceneState::Simulate:
		{
			m_EditorCamera.OnUpdate(ts);

			//Set up physics and then render
			m_ActiveScene->OnUpdateSimulation(ts, m_EditorCamera);
			break;
		}
		case SceneState::Play:
		{
			//Set up physics, find main camera component and then render
			m_ActiveScene->OnUpdateRuntime(ts);
			break;
		}
		}

		auto [mx, my] = ImGui::GetMousePos();
		mx -= m_ViewportBounds[0].x;
		my -= m_ViewportBounds[0].y;
		glm::vec2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];
		my = viewportSize.y - my;
		int mouseX = (int)mx;
		int mouseY = (int)my;

		if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y)
		{
			int pixelData = m_Framebuffer->ReadPixel(1, mouseX, mouseY);
			m_HoveredEntity = pixelData == -1 ? Entity() : Entity((entt::entity)pixelData, m_ActiveScene.get());
		}

		OnOverlayRender();

		m_Framebuffer->Unbind();
	}

	void EditorLayer::OnImGuiRender()
	{
		SD_PROFILE_FUNCTION();

		// Don't render editor UI if no project is open
		if (!m_ProjectOpen)
			return;


		// Note: Switch this to true to enable dockspace
		static bool dockspaceOpen = true;
		static bool opt_fullscreen_persistant = true;
		bool opt_fullscreen = opt_fullscreen_persistant;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive, 
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise 
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
		ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				// Disabling fullscreen would allow the window to be moved to the front of other windows, 
				// which we can't undo at the moment without finer window depth/z control.
				//ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

				if (ImGui::MenuItem("new Project", "not Implemented"))
					NewProject();

				if (ImGui::MenuItem("Open Project...", "Ctrl+O"))
					OpenProject();

				if (ImGui::MenuItem("Save Project", "not Implemented"))
					SaveProject();

				ImGui::Separator();

				if (ImGui::MenuItem("New Scene", "Ctrl+N"))
					NewScene();

				if (ImGui::MenuItem("Save Scene", "Ctrl+S"))
					SaveScene();

				ImGui::Separator();

				if (ImGui::MenuItem("Save Scene As...", "Ctrl+Shift+S"))
					SaveSceneAs();

				ImGui::Separator();

				if (ImGui::MenuItem("Exit")) 
					Application::Get().Close();

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Script"))
			{
				if (ImGui::MenuItem("Reload assembly", "Ctrl+R"))
					ScriptEngine::ReloadAssembly();

				ImGui::EndMenu();
			}


			ImGui::EndMenuBar();
		}

		m_SceneHierarchyPanel.OnImGuiRender();
		m_ContentBrowserPanel->OnImGuiRender();
		m_ScriptEditorPanel->OnImGuiRender();
		ImGui::Begin("Stats");


#if 0
		std::string name = "None";
		if (m_HoveredEntity)
			name = m_HoveredEntity.GetComponent<TagComponent>().Tag;
		ImGui::Text("Hovered Entity: %s", name.c_str());
#endif


		auto stats = Renderer2D::GetStats();
		ImGui::Text("Renderer2D Stats:");
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("Quads: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

		ImGui::End();

		ImGui::Begin("Settings");
		ImGui::Checkbox("Show physics colliders", &m_ShowPhysicsColliders);
		ImGui::End();


		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport");
		auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
		auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
		auto viewportOffset = ImGui::GetWindowPos();
		m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
		m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();
		Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportHovered);

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

		uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
		ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		//Drag and Drop
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
			{

				AssetHandle handle = *(AssetHandle*)payload->Data;
				const AssetType assetType = AssetManager::GetAssetType(handle);

				if (assetType == AssetType::Scene)
				{
					OpenScene(handle);
				}
				else
				{
					SD_CORE_WARN("Dropped asset is not a Scene ({}). Ignoring drop.", AssetTypeToString(assetType));
				}

			}
			ImGui::EndDragDropTarget();
		}


		// Gizmos
		Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
		if (selectedEntity && m_GizmoType != -1)
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();

			ImGuizmo::SetRect(m_ViewportBounds[0].x, m_ViewportBounds[0].y, m_ViewportBounds[1].x - m_ViewportBounds[0].x, m_ViewportBounds[1].y - m_ViewportBounds[0].y);

			// Camera
			// Runtime camera from entity
			// auto cameraEntity = m_ActiveScene->GetPrimaryCameraEntity();
			// const auto& camera = cameraEntity.GetComponent<CameraComponent>().Camera;
			// const glm::mat4& cameraProjection = camera.GetProjection();
			// glm::mat4 cameraView = glm::inverse(cameraEntity.GetComponent<TransformComponent>().GetTransform());

			// Editor camera
			const glm::mat4& cameraProjection = m_EditorCamera.GetProjection();
			glm::mat4 cameraView = m_EditorCamera.GetViewMatrix();

			// Entity transform
			auto& tc = selectedEntity.GetComponent<TransformComponent>();
			glm::mat4 transform = tc.GetTransform();

			// Snapping
			bool snap = Input::IsKeyPressed(Key::LeftControl);
			float snapValue = 0.5f; // Snap to 0.5m for translation/scale
			// Snap to 45 degrees for rotation
			if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
				snapValue = 45.0f;

			float snapValues[3] = { snapValue, snapValue, snapValue };

			ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
				(ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform),
				nullptr, snap ? snapValues : nullptr);

			if (ImGuizmo::IsUsing())
			{
				glm::vec3 translation, rotation, scale;
				Math::DecomposeTransform(transform, translation, rotation, scale);

				glm::vec3 deltaRotation = rotation - tc.Rotation;
				tc.Translation = translation;
				tc.Rotation += deltaRotation;
				tc.Scale = scale;
			}
		}

		ImGui::End();
		ImGui::PopStyleVar();

		UI_Toolbar();

		ImGui::End();
	}

	void EditorLayer::ReloadProjectScripts()
	{
		Ref<Project> activeProject = Project::GetActive();
		if (!activeProject)
		{
			SD_CORE_ERROR("Cannot reload scripts: No active project is loaded.");
			return;
		}

		const auto& config = activeProject->GetConfig();
		// Construct the full path to the Assets directory
		std::filesystem::path assetsDir = Project::GetActiveProjectDirectory() / config.AssetDirectory;
		const std::string& projectName = config.Name;

		// 2. Call the private compilation function
		if (CompileScripts(assetsDir, projectName))
		{
			SD_CORE_INFO("C# scripts compiled successfully. Triggering assembly reload...");

			// 3. If compilation succeeded, trigger the hot reload in the ScriptEngine
			ScriptEngine::ReloadAssembly();
		}
		else
		{
			SD_CORE_ERROR("Script compilation failed. Check compile_log.txt for details.");
			// TODO, you would pop up an error notification here.
		}
	}

	void EditorLayer::UI_Toolbar()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		auto& colors = ImGui::GetStyle().Colors;
		const auto& buttonHovered = colors[ImGuiCol_ButtonHovered];
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(buttonHovered.x, buttonHovered.y, buttonHovered.z, 0.5f));
		const auto& buttonActive = colors[ImGuiCol_ButtonActive];
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(buttonActive.x, buttonActive.y, buttonActive.z, 0.5f));

		ImGui::Begin("##toolbar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

		bool toolbarEnabled = (bool)m_ActiveScene;

		ImVec4 tintColor = ImVec4(1, 1, 1, 1);
		if (!toolbarEnabled)
			tintColor.w = 0.5f;

		float size = ImGui::GetWindowHeight() - 4.0f;
		ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));

		bool hasPlayButton = m_SceneState == SceneState::Edit || m_SceneState == SceneState::Play;
		bool hasSimulateButton = m_SceneState == SceneState::Edit || m_SceneState == SceneState::Simulate;
		bool hasPauseButton = m_SceneState != SceneState::Edit;

		if (hasPlayButton)
		{
			Ref<Texture2D> icon = (m_SceneState == SceneState::Edit || m_SceneState == SceneState::Simulate) ? m_IconPlay : m_IconStop;
			if (ImGui::ImageButton("##PlayStop", (ImTextureID)(uint64_t)icon->GetRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), ImVec4(0, 0, 0, 0), tintColor) && toolbarEnabled)
			{
				if (m_SceneState == SceneState::Edit || m_SceneState == SceneState::Simulate)
					OnScenePlay();
				else if (m_SceneState == SceneState::Play)
					OnSceneStop();
			}
		}

		if (hasSimulateButton)
		{
			if (hasPlayButton)
				ImGui::SameLine();
			{
				Ref<Texture2D> icon = (m_SceneState == SceneState::Edit || m_SceneState == SceneState::Play) ? m_IconSimulate : m_IconStop;		//ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));
				if (ImGui::ImageButton("##SimulateStop", (ImTextureID)(uint64_t)icon->GetRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), ImVec4(0.0f, 0.0f, 0.0f, 0.0f), tintColor) && toolbarEnabled)
				{
					if (m_SceneState == SceneState::Edit || m_SceneState == SceneState::Play)
						OnSceneSimulate();
					else if (m_SceneState == SceneState::Simulate)
						OnSceneStop();
				}
			}
		}

		if (hasPauseButton)
		{
			bool isPaused = m_ActiveScene->IsPaused();
			ImGui::SameLine();
			{
				Ref<Texture2D> icon = m_IconPause;
				if (ImGui::ImageButton("##Debug", (ImTextureID)(uint64_t)icon->GetRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), ImVec4(0.0f, 0.0f, 0.0f, 0.0f), tintColor) && toolbarEnabled)
				{
					m_ActiveScene->SetPaused(!isPaused);
				}
			}

			// Step button
			if (isPaused)
			{
				ImGui::SameLine();
				{
					Ref<Texture2D> icon = m_IconStep;
					bool isPaused = m_ActiveScene->IsPaused();
					if (ImGui::ImageButton("##Step", (ImTextureID)(uint64_t)icon->GetRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), ImVec4(0.0f, 0.0f, 0.0f, 0.0f), tintColor) && toolbarEnabled)
					{
						m_ActiveScene->Step();
					}
				}
			}
		}
		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor(3);
		ImGui::End();
	}

	void EditorLayer::OnEvent(Event& e)
	{
		m_CameraController.OnEvent(e);
		if (m_SceneState == SceneState::Edit)
		{
			m_EditorCamera.OnEvent(e);
		}

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(SD_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
		dispatcher.Dispatch<MouseButtonPressedEvent>(SD_BIND_EVENT_FN(EditorLayer::OnMouseButtonPressed));
		dispatcher.Dispatch<WindowDropEvent>(SD_BIND_EVENT_FN(EditorLayer::OnWindowDrop));
	}

	void EditorLayer::OnProjectOpened(Event& e)
	{
		//TODO
	}

	void EditorLayer::OnNewProjectRequest(Event& e)
	{
		//TODO
		//NewProject();
	}

	void EditorLayer::OnScriptFileSaved(Event& e)
	{
		// This is now called when the ScriptEditorPanel dispatches the event
		SD_CORE_TRACE("EditorLayer received ScriptFileSavedEvent, reloading scripts...");
		ReloadProjectScripts();
	}

	bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
	{
		// Shortcuts
		if (e.IsRepeat())
			return false;

		bool control = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl);
		bool shift = Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift);
		switch (e.GetKeyCode())
		{
			case Key::N:
			{
				if (control)
					NewScene();

				break;
			}
			case Key::O:
			{
				if (control)
					OpenProject();

				break;
			}
			case Key::S:
			{
				if (control)
				{
					if (shift)
						SaveSceneAs();
					else
						SaveScene();
				}

				break;
			}

			// Scene Commands
			case Key::D:
			{
				if (control)
					OnDuplicateEntity();

				break;
			}

			// Gizmos
			case Key::Q:
			{
				if (!ImGuizmo::IsUsing())
					m_GizmoType = -1;
				break;
			}
			case Key::W:
			{
				if (!ImGuizmo::IsUsing())
					m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
				break;
			}
			case Key::E:
			{
				if (!ImGuizmo::IsUsing())
					m_GizmoType = ImGuizmo::OPERATION::ROTATE;
				break;
			}
			case Key::R:
			{
				if (!ImGuizmo::IsUsing())
					m_GizmoType = ImGuizmo::OPERATION::SCALE;
				if (control)
				{
					ScriptEngine::ReloadAssembly();
				}
				else
				{
					if (!ImGuizmo::IsUsing())
						m_GizmoType = ImGuizmo::OPERATION::SCALE;
				}
				break;
			}
			case Key::Delete:
			{
				if (Application::Get().GetImGuiLayer()->GetActiveWidgetID() == 0)
				{
					Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
					if (selectedEntity)
					{
						m_SceneHierarchyPanel.SetSelectedEntity({});
						m_ActiveScene->DestroyEntity(selectedEntity);
					}
				}
				break;
			}

		}

		return false;
	}

	bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		if (e.GetMouseButton() == Mouse::ButtonLeft)
		{
			if (m_ViewportHovered && !ImGuizmo::IsOver() && !Input::IsKeyPressed(Key::LeftAlt))
				m_SceneHierarchyPanel.SetSelectedEntity(m_HoveredEntity);
		}
		return false;
	}

	bool EditorLayer::OnWindowDrop(WindowDropEvent& e)
	{
		// TODO: if a project is dropped in, probably open it

		//AssetManager::ImportAsset();

		return true;
	}

	void EditorLayer::OnOverlayRender()
	{
		if (m_SceneState == SceneState::Play)
		{
			Entity camera = m_ActiveScene->GetPrimaryCameraEntity();
			if (!camera)
				return;
			Renderer2D::BeginScene(camera.GetComponent<CameraComponent>().Camera, camera.GetComponent<TransformComponent>().GetTransform());
		}
		else
		{
			Renderer2D::BeginScene(m_EditorCamera);
		}

		if (m_ShowPhysicsColliders)
		{
			// Box Colliders
			{
				auto view = m_ActiveScene->GetAllEntitiesWith<TransformComponent, BoxCollider2DComponent>();
				for (auto entity : view)
				{
					auto [tc, bc2d] = view.get<TransformComponent, BoxCollider2DComponent>(entity);

					// World transform for this entity (includes parent hierarchy)
					glm::mat4 worldTransform = m_ActiveScene->GetWorldTransform({ entity, m_ActiveScene.get() });

					glm::vec3 worldTranslation, worldRotation, worldScale;
					if (!Math::DecomposeTransform(worldTransform, worldTranslation, worldRotation, worldScale))
					{
						// Fallback to local component if decomposition fails
						worldTranslation = tc.Translation;
						worldRotation = tc.Rotation;
						worldScale = tc.Scale;
					}

					glm::vec4 worldOffset4 = worldTransform * glm::vec4(bc2d.Offset, 0.0f, 1.0f);
					glm::vec3 worldOffset = glm::vec3(worldOffset4);

					glm::vec3 colliderScale;
					colliderScale.x = worldScale.x * (bc2d.Size.x * 2.0f);
					colliderScale.y = worldScale.y * (bc2d.Size.y * 2.0f);
					colliderScale.z = 1.0f;

					// Rotation around Z (2D)
					float rotationZ = worldRotation.z;

					glm::mat4 transform = glm::translate(glm::mat4(1.0f), worldOffset)
						* glm::rotate(glm::mat4(1.0f), rotationZ, glm::vec3(0.0f, 0.0f, 1.0f))
						* glm::scale(glm::mat4(1.0f), colliderScale);

					Renderer2D::DrawRect(transform, glm::vec4(0, 1, 0, 1));
				}
			}
			// Circle Colliders
			{
				auto view = m_ActiveScene->GetAllEntitiesWith<TransformComponent, CircleCollider2DComponent>();
				for (auto entity : view)
				{
					auto& tc = view.get<TransformComponent>(entity);
					auto& cc2d = view.get<CircleCollider2DComponent>(entity);

					// World transform (includes parents)
					glm::mat4 worldTransform = m_ActiveScene->GetWorldTransform({ entity, m_ActiveScene.get() });

					glm::vec3 worldTranslation, worldRotation, worldScale;
					if (!Math::DecomposeTransform(worldTransform, worldTranslation, worldRotation, worldScale))
					{
						// fallback to local if decomposition fails
						worldTranslation = tc.Translation;
						worldRotation = tc.Rotation;
						worldScale = tc.Scale;
					}

					// Transform local collider offset to world space
					glm::vec4 worldCenter4 = worldTransform * glm::vec4(cc2d.Offset, 0.0f, 1.0f);
					glm::vec3 worldCenter = glm::vec3(worldCenter4);

					// Compute world diameter from the maximum XY world scale to preserve circular shape
					float worldDiameter = glm::max(worldScale.x, worldScale.y) * cc2d.Radius * 2.0f;

					glm::mat4 transform = glm::translate(glm::mat4(1.0f), worldCenter)
						* glm::scale(glm::mat4(1.0f), glm::vec3(worldDiameter, worldDiameter, 1.0f));

					Renderer2D::DrawCircle(transform, glm::vec4(0, 1, 0, 1), 0.01f);
				}
			}
		}

		// Draw selected entity outline 
		if (Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity())
		{
			glm::mat4 worldTransform = m_ActiveScene->GetWorldTransform(selectedEntity);
			Renderer2D::DrawRect(worldTransform, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
			//const TransformComponent& transform = selectedEntity.GetComponent<TransformComponent>();
			//Renderer2D::DrawRect(transform.GetTransform(), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
			
		}

		Renderer2D::EndScene();
	}

	void EditorLayer::NewProject()
	{
		//Project::New();
		std::string filepath = FileDialogs::SaveFile("Strand Project (*.sproj)\0*.sproj\0", "Project.sproj", "sproj");
		if (filepath.empty())
			return;
		std::filesystem::path projectFilePath = filepath;
		std::filesystem::path parentDirectory = projectFilePath.parent_path();
		std::string projectName = projectFilePath.stem().string();
		std::filesystem::path projectDirectory = parentDirectory / projectName;

		projectFilePath = projectDirectory / (projectName + ".sproj");

		Ref<Project> proj = Project::New(projectDirectory, projectName);
		if (!proj)
		{
			SD_CORE_ERROR("Failed to create new project at '{}'", projectDirectory.string());
			return;
		}

		

		if (!Project::SaveActive(projectFilePath))
		{
			SD_CORE_ERROR("Failed to save new project file '{}'", projectFilePath.string());
			return;
		}

		// Compile starter scripts
		auto prjoCompleteAssetDir = projectDirectory / proj->GetConfig().AssetDirectory;
		if(!CompileScripts(prjoCompleteAssetDir, projectName))
			SD_CORE_ERROR("Scripts compilation failed for '{}'", projectName);
		else
			SD_CORE_INFO("Compiled scripts: {}/Scripts/{}.dll", projectDirectory.string(), projectName);


		// Initialize scripting and UI similar to OpenProject
		ScriptEngine::Init();

		AssetHandle startScene = Project::GetActive()->GetConfig().StartScene;
		if (startScene)
			OpenScene(startScene);

		m_ContentBrowserPanel = CreateScope<ContentBrowserPanel>(Project::GetActive());
		

		m_ProjectOpen = true;

		// Add to recent projects
		EditorSettings::Get().AddRecentProject(projectFilePath);
	}

	void EditorLayer::OpenProject(const std::filesystem::path& path)
	{
		if (Project::Load(path))
		{
			ScriptEngine::Init();

			AssetHandle startScene = Project::GetActive()->GetConfig().StartScene;
			if (startScene)
				OpenScene(startScene);
			m_ContentBrowserPanel = CreateScope<ContentBrowserPanel>(Project::GetActive());
			


			m_ProjectOpen = true;

			// Add to recent projects
			EditorSettings::Get().AddRecentProject(path);
		}
	}

	bool EditorLayer::OpenProject()
	{
		std::string filepath = FileDialogs::OpenFile("Strand Project (*.sproj)\0*.sproj\0");
		if (filepath.empty())
			return false;

		OpenProject(filepath);
		return true;
	}

	void EditorLayer::SaveProject()
	{
		const auto& path = Project::GetActiveProjectDirectory() / (Project::GetActive()->GetConfig().Name + ".sproj");

		if (Project::SaveActive(path))
		{
			SD_CORE_INFO("Project saved successfully to: {}", path.string());
		}
		else
		{
			SD_CORE_ERROR("Failed to save active project.");
		}

		//if (auto editorAssetManager = Project::GetActive()->GetEditorAssetManager())
		//{
		//	editorAssetManager->SerializeAssetRegistry();
		//}
	}

	void EditorLayer::NewScene()
	{
		m_ActiveScene = CreateRef<Scene>();
		//m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		m_SceneHierarchyPanel.SetContext(m_ActiveScene);

		m_EditorScenePath = std::filesystem::path();
	}

	void EditorLayer::OpenScene()
	{
	/*	std::string filepath = FileDialogs::OpenFile("Strand Scene (*.strand)\0*.strand\0");
		if (!filepath.empty())
		{
			OpenScene(filepath);
		}*/
	}

	void EditorLayer::OpenScene(AssetHandle handle)
	{
		SD_CORE_ASSERT(handle);

		if (m_SceneState != SceneState::Edit)
			OnSceneStop();

		Ref<Scene> readOnlyScene = AssetManager::GetAsset<Scene>(handle);
		Ref<Scene> newScene = Scene::Copy(readOnlyScene);

		m_EditorScene = newScene;
		m_SceneHierarchyPanel.SetContext(m_EditorScene);

		m_ActiveScene = m_EditorScene;
		m_EditorScenePath = Project::GetActive()->GetEditorAssetManager()->GetFilePath(handle);
	}

	void EditorLayer::SaveScene()
	{
		if (!m_EditorScenePath.empty())
			SerializeScene(m_ActiveScene, m_EditorScenePath);
		else
			SaveSceneAs();
	}

	void EditorLayer::SaveSceneAs()
	{
		std::string filepath = FileDialogs::SaveFile("Strand Scene (*.strand)\0*.strand\0", "Scene.strand", "strand");
		if (!filepath.empty())
		{
			SerializeScene(m_ActiveScene, filepath);
			m_EditorScenePath = filepath;
		}
	}

	void EditorLayer::SerializeScene(Ref<Scene> scene, const std::filesystem::path& path)
	{
		SceneImporter::SaveScene(scene, path);
	}

	void EditorLayer::OnScenePlay()
	{
		if (m_SceneState == SceneState::Simulate)
			OnSceneStop();

		m_SceneState = SceneState::Play;

		m_ActiveScene = Scene::Copy(m_EditorScene);
		m_ActiveScene->OnRuntimeStart();

		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
	}

	void EditorLayer::OnSceneSimulate()
	{
		if (m_SceneState == SceneState::Play)
			OnSceneStop();

		m_SceneState = SceneState::Simulate;

		m_ActiveScene = Scene::Copy(m_EditorScene);
		m_ActiveScene->OnSimulationStart();

		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
	}

	void EditorLayer::OnSceneStop()
	{
		SD_CORE_ASSERT(m_SceneState == SceneState::Play || m_SceneState == SceneState::Simulate);

		if (m_SceneState == SceneState::Play)
			m_ActiveScene->OnRuntimeStop();
		else if (m_SceneState == SceneState::Simulate)
			m_ActiveScene->OnSimulationStop();

		m_SceneState = SceneState::Edit;
		m_ActiveScene = m_EditorScene;

		m_SceneHierarchyPanel.SetContext(m_ActiveScene);

	}

	void EditorLayer::OnScenePause()
	{
		if (m_SceneState == SceneState::Edit)
			return;

		m_ActiveScene->SetPaused(true);
	}

	void EditorLayer::OnDuplicateEntity()
	{
		if (m_SceneState != SceneState::Edit)
			return;

		Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
		if (selectedEntity)
		{
			Entity newEntity = m_EditorScene->DuplicateEntity(selectedEntity);
			m_SceneHierarchyPanel.SetSelectedEntity(newEntity);
		}
	}

	bool EditorLayer::CompileScripts(const std::filesystem::path& assetsDir, const std::string& projectName)
	{
		auto scriptsDir = assetsDir / "Scripts";
		auto outputDll = scriptsDir / "Binaries" / (projectName + ".dll");

		// Ensure output directory exists
		// TODO REMOVE THIS FORM HERE
		std::error_code ec;
		std::filesystem::create_directories(outputDll.parent_path(), ec);
		if (ec)
		{
			SD_CORE_ERROR("Failed to create script output directory '{}': {}", outputDll.parent_path().string(), ec.message());
			return false;
		}

		std::string inputPattern = (scriptsDir / "*.cs").string();
		std::string logPath = (scriptsDir / "compile_log.txt").string();
		auto referenceDll = scriptsDir / "Binaries" / "Strand-ScriptCore.dll";


		std::vector<std::string> compilers = {
			// Prefer Microsoft csc for .NET Framework projects, then Mono, then PATH lookups
			"C:\\Windows\\Microsoft.NET\\Framework64\\v4.0.30319\\csc.exe",
			"C:\\Windows\\Microsoft.NET\\Framework\\v4.0.30319\\csc.exe",
			"C:\\Program Files\\Mono\\bin\\mcs.bat",
			"csc",
			"mcs"
		};

		for (const auto& compiler : compilers)
		{
			// If compiler is an absolute path and missing, skip it
			if ((compiler.find(':') != std::string::npos) && !std::filesystem::exists(compiler))
				continue;

			// Quote executable if it contains spaces
			std::string exe = compiler;
			if (exe.find(' ') != std::string::npos && exe.front() != '"')
				exe = "\"" + exe + "\"";

			// Build command and redirect stdout/stderr to a log file
			std::string command =
				exe + " -target:library -out:\"" + outputDll.string() + "\" "
				"-r:\"" + referenceDll.string() + "\" "
				"\"" + inputPattern + "\" > \"" + logPath + "\" 2>&1";


			SD_CORE_INFO("Running script compile command: {}", command);

			int result = std::system(command.c_str());

			if (result == 0 && std::filesystem::exists(outputDll))
			{
				SD_CORE_INFO("Scripts compiled successfully to '{}'", outputDll.string());
				return true;
			}
			else
			{
				SD_CORE_WARN("Compiler '{}' returned {}. See '{}'", compiler, result, logPath);
			}
		}

		SD_CORE_ERROR("All script compilers failed. Inspect '{}'", logPath);
		return false;
	}


}