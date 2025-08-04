#include <Strand.h>
#include <ImGui/imgui.h>

class ExampleLayer : public Strand::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{
	}

	void OnUpdate() override
	{
		//GM_INFO("ExampleLayer::Update");
		if (Strand::Input::IsKeyPressed(SD_KEY_TAB))
			SD_TRACE("Tab key is pressed (poll)!");
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Example Layer");
		ImGui::Text("Hello from ExampleLayer!");
		if (ImGui::Button("Click Me"))
			SD_TRACE("Button clicked!");
		ImGui::End();
	}

	void OnEvent(Strand::Event& event) override
	{
		if (event.GetEventType() == Strand::EventType::KeyPressed)
		{
			Strand::KeyPressedEvent& e = (Strand::KeyPressedEvent&)event;
			if (e.GetKeyCode() == SD_KEY_TAB)
				SD_TRACE("Tab key is pressed (event)!");
			SD_TRACE("{0}", (char)e.GetKeyCode());
		}
	}

};
class Sandbox : public Strand::Application
{
public:
	Sandbox()
	{ 
		PushLayer(new ExampleLayer());
	}

	~Sandbox()
	{
	}


};

Strand::Application* Strand::CreateApplication()
{
	return new Sandbox();
}