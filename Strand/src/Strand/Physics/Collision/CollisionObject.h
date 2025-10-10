#pragma once

#include "Strand/Scene/Components.h"
#include "Collider.h"

namespace Strand {

	struct Manifold;

	using CollisionCallbackFn = std::function<void(Manifold&, float)>;

	class CollisionObject
	{
	public:
		CollisionObject(bool isDynamic = false);

		virtual void SetTransform();

	public:
		TransformComponent Transform;
		Collider* Collider;

		bool IsTrigger;
		bool IsStatic;
		const bool IsDynamic;

		CollisionCallbackFn OnCollision;
	};

}
