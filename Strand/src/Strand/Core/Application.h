#pragma once

#include "Strand/Core/Base.h"

#include "Strand/Core/Window.h"
#include "Strand/Core/LayerStack.h"
#include "Strand/Events/Event.h"
#include "Strand/Events/ApplicationEvent.h"

#include "Strand/Core/Timestep.h"

#include "Strand/ImGui/ImGuiLayer.h"


int main(int argc, char** argv);

namespace Strand {

	class  Application
	{
	public:
		Application();
		virtual ~Application();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		Window& GetWindow() { return *m_Window; }

		void Close();
		static Application& Get() { return *s_Instance; }
	private:
		void Run();
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
	private:
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		bool m_Minimized = false;
		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;
	private:
		static Application* s_Instance;
		friend int ::main(int argc, char** argv);
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}



