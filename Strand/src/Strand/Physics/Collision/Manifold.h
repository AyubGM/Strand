#pragma once

#include "Strand/Physics/Collision/ManifoldPoints.h"
#include "Strand/Physics/Collision/CollisionObject.h"

namespace Strand {
	struct Manifold 
	{
		CollisionObject* ObjA;
		CollisionObject* ObjB;
		glm::vec3 A;
		glm::vec3 B;
		glm::vec3 Normal;
		float Depth;
		bool HasCollision;

		Manifold(CollisionObject* a, CollisionObject* b, const ManifoldPoints& points
		)
			: ObjA(a)
			, ObjB(b)
			, A(points.A)
			, B(points.B)
			, Normal(points.Normal)
			, Depth(points.Depth)
			, HasCollision(points.HasCollision)
		{}
	
	};
}