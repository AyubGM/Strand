#include "sdpch.h"
#include "EditorEventBus.h"

namespace Strand {

	std::map<EventType, std::vector<EditorEventBus::EventCallbackFn>> EditorEventBus::s_Subscribers;

	void EditorEventBus::Subscribe(EventType type, EventCallbackFn&& callback)
	{
		s_Subscribers[type].push_back(std::move(callback));
	}

	void EditorEventBus::Dispatch(Event& event)
	{
		EventType type = event.GetEventType();
		if (s_Subscribers.find(type) == s_Subscribers.end())
			return;

		for (auto& callback : s_Subscribers[type])
		{
			callback(event);
		}
	}

}