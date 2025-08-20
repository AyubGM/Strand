#pragma once

#include "Strand/Renderer/GraphicsContext.h"

struct GLFWwindow; // Forward declaration of GLFWwindow

namespace Strand {

	class  OpenGLContext : public GraphicsContext
	{
	public:
		OpenGLContext(GLFWwindow* windowHandle);
		virtual ~OpenGLContext() = default;
		void Init() override;
		void SwapBuffers() override;
	private:
		GLFWwindow* m_WindowHandle; // Handle to the GLFW window
	};

}