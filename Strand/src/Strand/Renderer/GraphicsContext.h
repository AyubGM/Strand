#pragma once

namespace Strand {
	class GraphicsContext
	{
	public:
		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;
		virtual ~GraphicsContext() = default;
	};
} // namespace Strand