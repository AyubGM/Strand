#pragma once

namespace Strand {
	class GraphicsContext
	{
	public:
		virtual ~GraphicsContext() = default;

		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;
		virtual ~GraphicsContext() = default;
		static Scope<GraphicsContext> Create(void* window);
	};
} // namespace Strand