#include "sdpch.h"
#include "Strand/Renderer/GraphicsContext.h"

#include "Strand/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLContext.h"

namespace Strand {

	Scope<GraphicsContext> GraphicsContext::Create(void* window)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    SD_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return CreateScope<OpenGLContext>(static_cast<GLFWwindow*>(window));
		}

		SD_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}