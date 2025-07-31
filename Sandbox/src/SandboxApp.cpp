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
	}

	void OnEvent(GMCore::Event& event) override
	{
		GM_TRACE("{0}", event.ToString());
	}

};
class Sandbox : public GMCore::Application
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

GMCore::Application* GMCore::CreateApplication()
{
	return new Sandbox();
}