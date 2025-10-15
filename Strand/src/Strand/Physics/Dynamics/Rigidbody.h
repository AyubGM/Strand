#pragma once

#include "Strand/Physics/Collision/CollisionObject.h"

namespace Strand {

	class Rigidbody : public CollisionObject {

	public:
	 Rigidbody(bool isKinematic = true);

	// maybe create a rigidbody 2d
	// void ApplyForce2 (const glm::vec2& force, const glm::vec2& position = glm::vec2());
	// void ApplyTorque2(float torque);
	 void ApplyForce(const glm::vec3& force, const glm::vec3& position = glm::vec3());
	 void ApplyTorque(const glm::vec3& torque);
	 void ApplyGravity();

	 const TransformComponent& LastTrans() const;
	 const TransformComponent& NextTrans() const;

	 float  Mass() const;
	 void SetMass(float mass);

	 void SetTransform(TransformComponent* transform) override;
	 void SetLastTrans(const TransformComponent& lastTrans);
	 void SetNextTrans(const TransformComponent& nextTrans);

	 void Move(const glm::vec3 delta);
	

	public:
		glm::vec3 Gravity; 
		glm::vec3 NetForce; 
		glm::vec3 Velocity;

		glm::vec3 NetTorque;
		glm::vec3 AngularVelocity;
		glm::mat3 Inertia; // for testing

		glm::vec3 AxisLock;     // Lock to axis
		glm::vec3 IsAxisLocked; // If its locked to each axis casted to bool

		float InvMass;       // 1 / Mass of rigidbody
		bool TakesGravity;    // If the rigidbody will use its own gravity or take it from the space
		bool SimGravity;      // If the rigidbody will simulate gravity
		bool IsSimulated;     // If the rigidbody gets simulated. Still participates with collisions, but is unaffected.

		float StaticFriction;  // Static friction coefficient
		float DynamicFriction; // Dynamic friction coefficient
		float Restitution;     // Elasticity of collisions

	private:
		TransformComponent m_LastTrans; // Where the rigidbody was last step
		TransformComponent m_NextTrans; // Where the rigidbody will be next step if there is no interference
	};

}