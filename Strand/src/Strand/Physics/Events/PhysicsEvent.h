#pragma once

#include "Strand/Events/Event.h"
#include "Strand/Core/Timestep.h"
#include "Strand/Physics/Collision/Manifold.h"

namespace Strand {

	class PhysicsEvent: public Event
	{
	public:
		Timestep GetTimestep() const { return m_Timestep; }
		EVENT_CLASS_CATEGORY(EventCategoryPhysics | EventCategoryApplication)

	protected:
		PhysicsEvent(Timestep timestep) : m_Timestep(timestep) {}

		Timestep m_Timestep;

	};

	class CollisionEvent : public PhysicsEvent
	{

	public:
		CollisionEvent(Manifold& manifold, Timestep timestep) : PhysicsEvent(timestep), m_Manifold(manifold) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "CollisionEvent: HasCollision=" << m_Manifold.HasCollision << ", Time=" << m_Timestep.GetSeconds();
			return ss.str();
		}

		EVENT_CLASS_TYPE(PhysicsCollision)

	private:
		Manifold m_Manifold;

	};
}