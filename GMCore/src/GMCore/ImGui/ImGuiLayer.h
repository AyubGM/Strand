#pragma once

#include "GMCore/Layer.h"

#include "GMCore/Events/ApplicationEvent.h"
#include "GMCore/Events/KeyEvent.h"
#include "GMCore/Events/MouseEvent.h"

namespace GMCore {

	class GMCORE_API ImGuiLayer : public Layer
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