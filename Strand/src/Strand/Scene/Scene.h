#pragma once

#include "entt.hpp"


#include "Strand/Core/Timestep.h"

namespace Strand {

	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = std::string());

		// TEMP
		entt::registry& Reg() { return m_Registry; }

		void OnUpdate(Timestep ts);
	private:
		entt::registry m_Registry;


		friend class Entity;
	};

}