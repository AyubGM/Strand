#include "sdpch.h"

#include "CollisionSpace.h"
#include "Strand/Core/Base.h"
#include "TestCollision.h"

namespace Strand {

	void CollisionSpace::AddCollisionObject(CollisionObject* object)
	{
		if (object == nullptr) {
			SD_CORE_WARN("Tried to add null object to collision space");
			return;
		}

		m_Objects.push_back(object);
	}

	void CollisionSpace::RemoveCollisionObject(
		CollisionObject* object)
	{
		if (object == nullptr) {
			SD_CORE_WARN("Tried to remove null object from collision space");
			return;
		}

		auto itr = std::find(m_Objects.begin(), m_Objects.end(), object);

		if (itr == m_Objects.end()) {
			SD_CORE_WARN("Tried to remove object that doesn't exist in the collision space");
			return;
		}

		m_Objects.erase(itr);
	}

	void CollisionSpace::AddSolver(Solver* solver)
	{
		if (solver == nullptr) {
			SD_CORE_WARN("Tried to add null solver to collision space");
			return;
		}

		m_Solvers.push_back(solver);
	}

	void CollisionSpace::RemoveSolver(Solver* solver)
	{
		if (solver == nullptr) {
			SD_CORE_WARN("Tried to remove null solver from dynamics space");
			return;
		}

		auto itr = std::find(m_Solvers.begin(), m_Solvers.end(), solver);

		if (itr == m_Solvers.end()) {
			SD_CORE_WARN("Tried to remove solver that doesn't exist in the collision space");
			return;
		}

		m_Solvers.erase(itr);
	}

	void CollisionSpace::ResolveConstrains(
		float dt)
	{
		// Update cache

		auto updateCahce = [](
			CollisionObject* obj)
			{
				Collider* collider = obj->Collider;
				if (collider
					&& collider->CacheIsOld())
				{
					collider->UpdateCache();
				}
			};

		if (m_Task) 
		{
			for (CollisionObject* obj : m_Objects)
			{
				updateCahce(obj);
			}

			/*	m_Task->foreach(m_Objects, [&](int index) {
					updateCahce(m_Objects.at(index));
				});
			*/

		}
		else 
		{
			for (CollisionObject* obj : m_Objects) {
				updateCahce(obj);
			}
		}

		// should have a container that keeps track of this
		std::vector<std::pair<CollisionObject*, CollisionObject*>> pairs;

		for (CollisionObject* a : m_Objects)
		{
			if (!a->Collider) {
				continue;
			}

			for (CollisionObject* b : m_Objects) {
				if (a == b) break;

				if (!b->Collider
					|| (a->IsTrigger && b->IsTrigger)
					|| (a->IsStatic && b->IsStatic))
				{
					continue;
				}

				// temp 'broad' phase 
				if (a->Collider->Dim == Dimension::D2
					&& b->Collider->Dim == Dimension::D2)
				{
					const auto& abounds = ((ColliderShape<Dimension::D2>*)a->Collider)->Bounds();
					const auto& bbounds = ((ColliderShape<Dimension::D2>*)b->Collider)->Bounds();

					if (!abounds.Intersects(&a->Transform, bbounds, &b->Transform))
					{
						continue;
					}
				}

				pairs.emplace_back(a, b);
			}
		}

		std::vector<Manifold> triggers;
		std::vector<Manifold> collisions;
		std::mutex mutexTriggers, mutexCollisions;

		auto findManifold = [&](
			CollisionObject* a,
			CollisionObject* b)
			{
				ManifoldPoints points = TestCollision(a->Collider, &a->Transform, b->Collider, &b->Transform);

				if (points.HasCollision)
				{
					if (a->IsTrigger
						|| b->IsTrigger) // establish more formal rules for what can collide with what, see next comments
					{
						std::unique_lock lock(mutexTriggers); // locks on single thread too
						triggers.emplace_back(a, b, points);
					}

					else {
						std::unique_lock lock(mutexCollisions);
						collisions.emplace_back(a, b, points);
					}
				}
			};

		if (m_Task) {
			m_Task->foreach(pairs, [&](size_t i) {
				findManifold(pairs[i].first, pairs[i].second);
				});
		}

		else {
			for (auto& [a, b] : pairs) {
				findManifold(a, b);
			}
		}

		SendCollisionCallbacks(collisions, dt);
		SendCollisionCallbacks(triggers, dt);

		for (int i = 0; i < collisions.size(); i++) // in a collision callback they could disable the response
		{
			if (!collisions[i].HasCollision)
			{
				collisions[i] = collisions.back();
				collisions.pop_back();
			}
		}

		SolveManifolds(collisions, dt);
	}

}