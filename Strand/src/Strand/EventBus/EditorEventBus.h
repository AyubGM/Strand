#pragma once

#include "Strand/Events/Event.h"
#include <functional>
#include <map>
#include <vector>

namespace Strand {

	// A simple static event bus for editor-wide communication
	class EditorEventBus
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		// Subscribe a listener to a specific event type
		static void Subscribe(EventType type, EventCallbackFn&& callback);

		// Dispatch an event to all subscribed listeners
		static void Dispatch(Event& event);

	private:
		static std::map<EventType, std::vector<EventCallbackFn>> s_Subscribers;
	};

}