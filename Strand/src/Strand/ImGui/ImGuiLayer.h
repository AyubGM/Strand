#pragma once

#include "Strand/Layer.h"

#include "Strand/Events/ApplicationEvent.h"
#include "Strand/Events/KeyEvent.h"
#include "Strand/Events/MouseEvent.h"

namespace Strand {

	class STRAND_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual	void OnAttach() override;
		virtual	void OnDetach() override;
		virtual	void OnImGuiRender() override;

		void Begin();
		void End();

	private:
		float m_Time = 0.0f;
	};

}