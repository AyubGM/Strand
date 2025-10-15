#include "sdpch.h"
#include "Strand/Physics/Dynamics/SmoothPositionSolver.h"
#include "Strand/Physics/Dynamics/Rigidbody.h"


namespace Strand {

	void SmoothPositionSolver::Solve(std::vector<Manifold>& manifolds, float dt)
	{
		std::vector<std::pair<glm::vec3, glm::vec3>> deltas;

		for (Manifold& manifold : manifolds) {
			Rigidbody* aBody = manifold.ObjA->IsDynamic ? (Rigidbody*)manifold.ObjA : nullptr;
			Rigidbody* bBody = manifold.ObjB->IsDynamic ? (Rigidbody*)manifold.ObjB : nullptr;

			float aInvMass = aBody ? aBody->InvMass : 0.0f;
			float bInvMass = bBody ? bBody->InvMass : 0.0f;

			const float percent = 0.8f;
			const float slop = 0.01f;

			glm::vec3 correction = manifold.Normal * percent
				* fmax(manifold.Depth - slop, 0.0f)
				/ (aInvMass + bInvMass);

			glm::vec3 deltaA;
			glm::vec3 deltaB;

			if (aBody ? aBody->IsSimulated : false) {
				deltaA = aInvMass * correction;
			}

			if (bBody ? bBody->IsSimulated : false) {
				deltaB = bInvMass * correction;
			}

			deltas.emplace_back(deltaA, deltaB);
		}

		for (unsigned i = 0; i < manifolds.size(); i++) {
			Rigidbody* aBody = manifolds[i].ObjA->IsDynamic ? (Rigidbody*)manifolds[i].ObjA : nullptr;
			Rigidbody* bBody = manifolds[i].ObjB->IsDynamic ? (Rigidbody*)manifolds[i].ObjB : nullptr;

			if (aBody ? aBody->IsSimulated : false) {
				aBody->Transform.Translation -= deltas[i].first;
			}

			if (bBody ? bBody->IsSimulated : false) {
				bBody->Transform.Translation += deltas[i].second;
			}
		}
	}
}
