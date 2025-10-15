#include "sdpch.h"
#include "Strand/Physics/Dynamics/Rigidbody.h"

namespace Strand {

	Rigidbody::Rigidbody(
		bool isKinematic
	)
		: CollisionObject(true)
		, Gravity(0.f)
		, NetForce(0.f)
		, Velocity(0.f)
		, NetTorque(0.f)
		, AngularVelocity(0.f)
		, Inertia(1.f)
		, AxisLock(0.f)
		, IsAxisLocked(0.f)
		, InvMass(1)
		, TakesGravity(true)
		, SimGravity(true)
		, IsSimulated(isKinematic)
		, StaticFriction(.5)
		, DynamicFriction(.5)
		, Restitution(.5)
	{}

	void Rigidbody::ApplyForce(
		const glm::vec3& force,
		const glm::vec3& position)
	{
		NetForce += force;
		ApplyTorque(glm::cross(position, force));
	}

	void Rigidbody::ApplyTorque(
		const glm::vec3& torque)
	{
		NetTorque += torque;
	}

	void Rigidbody::ApplyGravity() {
		ApplyForce(Gravity * Mass());
	}

	float Rigidbody::Mass() const {
		return 1.0f / InvMass;
	}

	const TransformComponent& Rigidbody::LastTrans() const {
		return m_LastTrans;
	}

	const TransformComponent& Rigidbody::NextTrans() const {
		return m_NextTrans;
	}

	void Rigidbody::SetMass(
		float mass)
	{
		InvMass = 1.0f / mass;
	}

	void Rigidbody::SetTransform(
		TransformComponent* transform)
	{
		SetLastTrans(*transform);
		SetNextTrans(*transform);
		CollisionObject::SetTransform(transform);
	}

	void Rigidbody::SetLastTrans(
		const TransformComponent& lastTrans)
	{
		m_LastTrans = lastTrans;
	}

	void Rigidbody::SetNextTrans(
		const TransformComponent& nextTrans)
	{
		m_NextTrans = nextTrans;
	}

	void Rigidbody::Move(
		const glm::vec3 delta)
	{
		Transform.Translation += delta;
		Velocity = delta;
	}
}