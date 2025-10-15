#pragma once
#include "Strand/Physics/Collision/Solver.h"

namespace Strand {

	class SmoothPositionSolver : public Solver
	{
	public:

		void Solve(std::vector<Manifold>& manifolds, float dt) override;

	};
}