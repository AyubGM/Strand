#pragma once

#include "Strand/Physics/Collision/Collider.h"
#include "Strand/Physics/Collision/ManifoldPoints.h"

namespace Strand {

	ManifoldPoints TestCollision(
		Collider* a, TransformComponent* at,
		Collider* b, TransformComponent* bt);
}