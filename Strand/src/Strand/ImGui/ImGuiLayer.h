#pragma once

#include "Strand/Core/Layer.h"

#include "Strand/Events/ApplicationEvent.h"
#include "Strand/Events/KeyEvent.h"
#include "Strand/Events/MouseEvent.h"

namespace Strand {

	class  ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer() = default;

		virtual	void OnAttach() override;
		virtual	void OnDetach() override;
		virtual void OnEvent(Event& e) override;

		void Begin();
		void End();

		void BlockEvents(bool block) { m_BlockEvents = block; }
	private:
		bool m_BlockEvents = true;
		float m_Time = 0.0f;
	};

}