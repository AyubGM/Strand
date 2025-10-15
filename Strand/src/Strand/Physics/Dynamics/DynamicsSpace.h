#pragma once

#include "Strand/Physics/Collision/CollisionSpace.h"
#include "Strand/Physics/Dynamics/Rigidbody.h"
#include "Strand/Physics/Dynamics/Constraint.h"
#include "Strand/Core/Timestep.h"

#include <glm/glm.hpp>

namespace Strand {

	class DynamicsSpace : public CollisionSpace
	{
	public:

		virtual void AddRigidbody(Rigidbody* rigidbody);
		virtual void AddConstaint(Constraint* constraint);
		virtual void Step(Timestep ts);

		const glm::vec3& GetGravity() { return m_Gravity;}
		void SetGravity(const glm::vec3& gravity) { m_Gravity = gravity; TrySetGravity();}

		const std::vector<Constraint*>& GetConstraints() const { return m_Constraints; }

	private: 
		void TrySetGravity();
		void TryApplyGravity();

		void PredictTransforms(Timestep ts);

		void ClearForces();

	public:
		bool Paused = false;

	private:
		glm::vec3 m_Gravity;
		std::vector<Constraint*> m_Constraints;

	};


}