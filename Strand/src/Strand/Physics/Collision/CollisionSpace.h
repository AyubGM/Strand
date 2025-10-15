#pragma once

#include "Strand/Physics/Collision/Solver.h"
#include "Strand/Utils/ThreadPool.h"

#include <set>

namespace Strand {

	struct DistanceQueryResult
	{
		std::set<std::pair<float, CollisionObject*>> Objects;

		CollisionObject* Closest()
		{
			if (Objects.size() == 0)
			{
				return nullptr;
			}

			return Objects.begin()->second;
		}

		float MinDistance()
		{
			if (Objects.size() == 0)
			{
				return -FLT_MAX;
			}

			return Objects.begin()->first;
		}
	};

	class CollisionSpace 
	{
	public:
		// this is for testing the swapping of manifold points, the order of objects SHOULD NOT matter
		inline void debug__ScrambleObjects()
		{
			/*std::vector<CollisionObject*> objects;
			while (m_Objects.size() > 0)
			{
				auto itr = m_Objects.begin() + iw::randi(int(m_Objects.size()) - 1);
				objects.push_back(*itr);
				m_Objects.erase(itr);
			}

			m_Objects = objects; */
		}

		virtual void AddCollisionObject(CollisionObject* object);
		virtual void RemoveCollisionObject(CollisionObject* object);

		void AddSolver(Solver* solver);
		void RemoveSolver(Solver* solver);

		void ResolveConstrains(float dt = 0);

		bool TestCollider(Collider& collider) const;
		bool TestObject(CollisionObject* object) const;

		DistanceQueryResult QueryPoint(const glm::vec3& position, float maxDistance) const;
		DistanceQueryResult QueryVector(const glm::vec3& position, const glm::vec3& vector, float maxDistance, float maxDistanceNorm) const;

		void SetCollisionCallback(const CollisionCallbackFn& callback);

		void SetMultithread(Ref<ThreadPool> task);

		const std::vector<CollisionObject*>& CollisionObjects() const;

	protected:
		void SolveManifolds(std::vector<Manifold>& manifolds,float dt = 0);

		void SendCollisionCallbacks(std::vector<Manifold>& manifolds,float dt = 0);

	protected:
		CollisionCallbackFn m_CollisionCallbackFn;
		std::vector<CollisionObject*> m_Objects;
	private:
		std::vector<Solver*> m_Solvers;
		Ref<ThreadPool> m_Task;
	};

}