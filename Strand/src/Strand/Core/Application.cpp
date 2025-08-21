#include "sdpch.h"
#include "Strand/Core/Application.h"

#include "Strand/Core/Log.h"

#include "Strand/Renderer/Renderer.h"

#include "Strand/Core/Input.h"

#include <GLFW/glfw3.h>

namespace Strand {

	Application* Application::s_Instance = nullptr;

	Application::Application()
		
	{
		SD_PROFILE_FUNCTION();

		SD_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window> (Window::Create());
		m_Window->SetEventCallback(SD_BIND_EVENT_FN(Application::OnEvent));

		Renderer::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

	}


	Application::~Application()
	{
		SD_PROFILE_FUNCTION();

		Renderer::Shutdown();
	}


	void Application::PushLayer(Layer* layer)
	{
		SD_PROFILE_FUNCTION();

		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		SD_PROFILE_FUNCTION();

		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::Close()
	{
		m_Running = false;
	}

	void Application::OnEvent(Event& e)
	{
		SD_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(SD_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(SD_BIND_EVENT_FN(Application::OnWindowResize));

		//SD_CORE_TRACE("{0}", e.ToString());
		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
		{

			if (e.Handled)
				break;
			(*it)->OnEvent(e);
			
		}
	}

	void Application::Run()
	{
		SD_PROFILE_FUNCTION();


		while (m_Running)
		{
			SD_PROFILE_SCOPE("RunLoop");

			float time = (float)glfwGetTime();
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			if(!m_Minimized)
			{
				{
					SD_PROFILE_SCOPE("LayerStack OnUpdate");

					for (Layer* layer : m_LayerStack)
						layer->OnUpdate(timestep);
				}

				m_ImGuiLayer->Begin();
				{
					SD_PROFILE_SCOPE("LayerStack OnImGuiRender");

					for (Layer* layer : m_LayerStack)
						layer->OnImGuiRender();
				}
				m_ImGuiLayer->End();

			}

			//m_Window update() has poll events which will call Application onEvent and starts the chain of events for the layers and it has swap buffers which 
			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		SD_PROFILE_FUNCTION();

		if(e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimized = true; // If the window is minimized, we do not want to render anything
			return false;
		}

		m_Minimized = false;


		m_Minimized = false;
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

		return false;
	}



}