#include "sdpch.h"
#include "Entity.h"

namespace Strand {

	Entity::Entity(entt::entity handle, Scene* scene)
		: m_EntityHandle(handle), m_Scene(scene)
	{
	}

}