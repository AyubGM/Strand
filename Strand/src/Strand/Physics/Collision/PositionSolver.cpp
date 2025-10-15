#include "sdpch.h"
#include "PositionSolver.h"
#include "Strand/Math/Math.h"

namespace Strand {
	void PositionSolver::Solve(
		std::vector<Manifold>& manifolds,
		float dt)
	{
		for (Manifold& manifold : manifolds) {
			CollisionObject* aBody = manifold.ObjA;
			CollisionObject* bBody = manifold.ObjB;

			float aStatic = (float)(int)aBody->IsStatic;
			float bStatic = (float)(int)bBody->IsStatic;

			_vec<Dimension::D3> resolution = manifold.Normal
				* manifold.Depth
				/ Math::Max<float>(1, aStatic + bStatic);

			aBody->Transform.Translation -= resolution * float(1 - aStatic);
			bBody->Transform.Translation += resolution * float(1 - bStatic);
		}
	}
}