#pragma once

#include "Core.h"


#include "Window.h"
#include "Strand/LayerStack.h"
#include "Strand/Events/Event.h"
#include "Strand/Events/ApplicationEvent.h"
#include "Strand/ImGui/ImGuiLayer.h"

#include "Strand/Core/Timestep.h"

#include "Strand/Renderer/OrthographicCamera.h"


namespace Strand {

	class STRAND_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline Window& GetWindow() { return *m_Window; }

		inline static Application& Get() { return *s_Instance; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);
	private:

		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;

		
	private:
		static Application* s_Instance;
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}



