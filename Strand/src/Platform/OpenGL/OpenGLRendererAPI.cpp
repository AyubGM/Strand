#include "sdpch.h"
#include "OpenGLRendererAPI.h"

#include <glad/glad.h>

namespace Strand {

	static GLenum OpenGLDepthFunc(RendererAPI::DepthFunc func)
	{
		switch (func)
		{
		case RendererAPI::DepthFunc::Never: return GL_NEVER;
		case RendererAPI::DepthFunc::Less: return GL_LESS;
		case RendererAPI::DepthFunc::Equal: return GL_EQUAL;
		case RendererAPI::DepthFunc::LessEqual: return GL_LEQUAL;
		case RendererAPI::DepthFunc::Greater: return GL_GREATER;
		case RendererAPI::DepthFunc::NotEqual: return GL_NOTEQUAL;
		case RendererAPI::DepthFunc::GreaterEqual: return GL_GEQUAL;
		case RendererAPI::DepthFunc::Always: return GL_ALWAYS;
		case RendererAPI::DepthFunc::None:
		default:
			SD_CORE_ASSERT(false, "Unknown Depth Function!");
			return GL_LEQUAL;
		}
	}

	void OpenGLMessageCallback(
		unsigned source,
		unsigned type,
		unsigned id,
		unsigned severity,
		int length,
		const char* message,
		const void* userParam)
	{
		switch (severity)
		{
		case GL_DEBUG_SEVERITY_HIGH:         SD_CORE_CRITICAL(message); return;
		case GL_DEBUG_SEVERITY_MEDIUM:       SD_CORE_ERROR(message); return;
		case GL_DEBUG_SEVERITY_LOW:          SD_CORE_WARN(message); return;
		case GL_DEBUG_SEVERITY_NOTIFICATION: SD_CORE_TRACE(message); return;
		}

		SD_CORE_ASSERT(false, "Unknown severity level!");
	}

	void OpenGLRendererAPI::Init()
	{
		SD_PROFILE_FUNCTION();

#ifdef SD_DEBUG
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(OpenGLMessageCallback, nullptr);

		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
#endif

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LINE_SMOOTH);
	}

	void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
	}

	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount)
	{
		vertexArray->Bind();
		uint32_t count = indexCount ? indexCount : vertexArray->GetIndexBuffer()->GetCount();
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
	}

	void OpenGLRendererAPI::DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount)
	{
		vertexArray->Bind();
		glDrawArrays(GL_LINES, 0, vertexCount);
	}

	void OpenGLRendererAPI::SetLineWidth(float width)
	{
		glLineWidth(width);
	}

	void OpenGLRendererAPI::SetDepthFunc(DepthFunc func)
	{
		glDepthFunc(OpenGLDepthFunc(func));
	}

	void OpenGLRendererAPI::SetDepthMask(bool enable)
	{
		glDepthMask(enable ? GL_TRUE : GL_FALSE);
	}

}