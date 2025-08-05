#pragma once

#include "Core.h"


#include "Window.h"
#include "Strand/LayerStack.h"
#include "Strand/Events/Event.h"
#include "Strand/Events/ApplicationEvent.h"
#include "Strand/ImGui/ImGuiLayer.h"

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

		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		LayerStack m_LayerStack;

		unsigned int m_VertextArray, m_VertextBuffer, m_IndexBuffer;

	private:
		static Application* s_Instance;
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}



