#pragma once
#include "Solver.h"

namespace Strand {

	class PositionSolver: public Solver
	{
	public:
	void Solve(std::vector<Manifold>& manifolds, float dt) override;
	};
}