#include "sdpch.h"
#include "DynamicsSpace.h"

#include <assert.h>

namespace Strand {

	void DynamicsSpace::AddRigidbody(Rigidbody* rigidbody)
	{
#ifdef SD_DEBUG

		if (rigidbody == nullptr)
		{
			SD_CORE_WARN("Tried to add null rigidbody to dynamics space");
			return;
		}
#endif // SD_DEBUG

		if (rigidbody->TakesGravity)
		{
			rigidbody->Gravity = m_Gravity;
		}

		AddCollisionObject(rigidbody);

	}

	void DynamicsSpace::AddConstaint(Constraint* constraint)
	{
		m_Constraints.push_back(constraint);
	}

	void DynamicsSpace::Step(Timestep ts)
	{
		if (Paused)
		{
			return;
		}

		TryApplyGravity();
		ResolveConstrains(ts);

		for (CollisionObject* object : m_Objects)
		{
			if (!object->IsDynamic) continue;

			Rigidbody* rigidbody = (Rigidbody*)object;

			if (rigidbody->IsSimulated)
			{
				//TODO ts remove the cast
				rigidbody->Velocity        += (float)ts * rigidbody->NetForce  * rigidbody->InvMass;
				rigidbody->AngularVelocity += (float)ts * rigidbody->NetTorque * rigidbody->Inertia;

				if (rigidbody->IsAxisLocked.x) {
					rigidbody->Velocity.x = 0;
				}

				if (rigidbody->IsAxisLocked.y) {
					rigidbody->Velocity.y = 0;
				}

				if (rigidbody->IsAxisLocked.z) {
					rigidbody->Velocity.z = 0;
				}
			}
		}

		for (Constraint* c : m_Constraints)
		{
			  c->init(ts);
		}

		for (int i = 0; i < 10; i++) // iterate until all are solved?
		for (Constraint* c : m_Constraints) 
		{
			c->solve(ts); // div by iteration count? // iterate until 'solved'?
		}

		for (CollisionObject* object : m_Objects) {
			if (!object->IsDynamic) continue;

			Rigidbody* rigidbody = (Rigidbody*)object;

			rigidbody->SetLastTrans(rigidbody->Transform);

			if (rigidbody->IsSimulated)
			{
				TransformComponent& transform = rigidbody->Transform;

				glm::quat rot = glm::angleAxis(
					glm::length(rigidbody->AngularVelocity) * ts,
					glm::length(rigidbody->AngularVelocity) == 0 ? glm::vec3(0, 0, 1)
					: glm::normalize(rigidbody->AngularVelocity)
				);

				//TODO
				transform.Translation += (float)ts * rigidbody->Velocity;
				transform.Rotation = rot * transform.Rotation;

				// Axis lock should be through constraints I think

				if (rigidbody->IsAxisLocked.x) {
					transform.Translation.x = rigidbody->AxisLock.x;
				}

				if (rigidbody->IsAxisLocked.y) {
					transform.Translation.y = rigidbody->AxisLock.y;
				}

				if (rigidbody->IsAxisLocked.z) {
					transform.Translation.z = rigidbody->AxisLock.z;
				}
			}
		}

		ClearForces();
	}

	void DynamicsSpace::TrySetGravity() {
		for (CollisionObject* object : m_Objects) {
			if (!object->IsDynamic) continue;

			Rigidbody* rigidbody = (Rigidbody*)object;
			if (rigidbody->TakesGravity && rigidbody->IsSimulated) {
				rigidbody->Gravity = m_Gravity;
			}
		}
	}

	void DynamicsSpace::TryApplyGravity() {
		for (CollisionObject* object : m_Objects) {
			if (!object->IsDynamic) continue;

			Rigidbody* rigidbody = (Rigidbody*)object;
			if (rigidbody->SimGravity && rigidbody->IsSimulated) {
				rigidbody->ApplyGravity();
			}
		}
	}

	void DynamicsSpace::PredictTransforms(Timestep ts)
	{
		for (CollisionObject* object : m_Objects) {
			if (!object->IsDynamic) continue;

			Rigidbody* rigidbody = (Rigidbody*)object;
			if (rigidbody->IsSimulated) {
				TransformComponent t = rigidbody->Transform;
				glm::vec3   v = rigidbody->Velocity;

				if (rigidbody->IsAxisLocked.x) {
					t.Translation.x = rigidbody->AxisLock.x;
					v.x = 0;
				}

				if (rigidbody->IsAxisLocked.y) {
					t.Translation.y = rigidbody->AxisLock.y;
					v.y = 0;
				}

				if (rigidbody->IsAxisLocked.z) {
					t.Translation.z = rigidbody->AxisLock.z;
					v.z = 0;
				}

				//TODO
				t.Translation += (float)ts * v + (float)ts * (float)ts * rigidbody->NetForce * rigidbody->InvMass;

				rigidbody->Velocity = v;
				rigidbody->SetNextTrans(t);
			}
		}
	}

	void DynamicsSpace::ClearForces() 
	{
		for (CollisionObject* object : m_Objects) {
			if (!object->IsDynamic) continue;
			Rigidbody* rigidbody = (Rigidbody*)object;

			if (rigidbody->IsSimulated) {
				rigidbody->NetForce = glm::vec3();
				rigidbody->NetTorque = glm::vec3();
			}
		}
	}
}

