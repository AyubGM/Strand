#pragma once

#include "Strand/Scene/Components.h"
#include "Strand/Physics/Collision/Collider.h"

namespace Strand {

	struct Manifold;

	using CollisionCallbackFn = std::function<void(Manifold&, float)>;

	class CollisionObject
	{
	public:
		CollisionObject(bool isDynamic = false) : Collider(nullptr), 
			IsTrigger(false)
			, IsStatic(!isDynamic)
			, IsDynamic(isDynamic) {};

		virtual void SetTransform(TransformComponent* transform) { Transform = *transform; }

	public:
		TransformComponent Transform;
		Collider* Collider;

		bool IsTrigger;
		bool IsStatic;
		const bool IsDynamic;

		CollisionCallbackFn OnCollision;
	};

}
