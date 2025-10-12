#pragma once

#include "Strand/Physics/Collision/CollisionObject.h"
#include "Strand/Physics/Collision/Manifold.h"

namespace Strand {


	class Solver 
	{
	public:
		virtual void Solve(std::vector<Manifold>& manifolds, float dt) = 0;
	};

}