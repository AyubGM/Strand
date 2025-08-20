#include "sdpch.h"
#include "Platform/OpenGL/OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <GL/GL.h>


namespace Strand {

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		SD_CORE_ASSERT(windowHandle, "Window handle is null!");
	}
	void OpenGLContext::Init()
	{
		SD_PROFILE_FUNCTION();

		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		SD_CORE_ASSERT(status, "Failed to initialize Glad!");
		SD_CORE_INFO("OpenGL Info:");
		SD_CORE_INFO("  Vendor: {0}", (const char*)glGetString(GL_VENDOR));
		SD_CORE_INFO("  Renderer: {0}", (const char*)glGetString(GL_RENDERER));
		SD_CORE_INFO("  Version: {0}", (const char*)glGetString(GL_VERSION));

#ifdef SD_ENABLE_ASSERTS
		int versionMajor;
		int versionMinor;
		glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
		glGetIntegerv(GL_MINOR_VERSION, &versionMinor);

		SD_CORE_ASSERT(versionMajor > 4 || (versionMajor == 4 && versionMinor >= 5), "Strand requires at least OpenGL version 4.5!");
#endif

	}
	void OpenGLContext::SwapBuffers()
	{
		SD_PROFILE_FUNCTION();

		glfwSwapBuffers(m_WindowHandle);
	}
}
