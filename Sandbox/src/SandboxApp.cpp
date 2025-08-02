#include <GMCore.h>

class ExampleLayer : public GMCore::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{
	}

	void OnUpdate() override
	{
		//GM_INFO("ExampleLayer::Update");
		if (GMCore::Input::IsKeyPressed(GM_KEY_TAB))
			GM_TRACE("Tab key is pressed (poll)!");
	}

	void OnEvent(GMCore::Event& event) override
	{
		if (event.GetEventType() == GMCore::EventType::KeyPressed)
		{
			GMCore::KeyPressedEvent& e = (GMCore::KeyPressedEvent&)event;
			if (e.GetKeyCode() == GM_KEY_TAB)
				GM_TRACE("Tab key is pressed (event)!");
			GM_TRACE("{0}", (char)e.GetKeyCode());
		}
	}

};
class Sandbox : public GMCore::Application
{
public:
	Sandbox()
	{ 
		PushLayer(new ExampleLayer());
		PushOverlay(new GMCore::ImGuiLayer());
	}

	~Sandbox()
	{
	}


};

GMCore::Application* GMCore::CreateApplication()
{
	return new Sandbox();
}