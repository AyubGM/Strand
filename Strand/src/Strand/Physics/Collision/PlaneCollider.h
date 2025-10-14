#pragma once

#include "Collider.h"

namespace Strand {

	template<Dimension D>
	struct PlaneCollider : ColliderShape<D>
	{
		using vec_t = _vec<D>;
		using aabb_t = AABB<D>;

		vec_t Normal;
		float Distance;

		PlaneCollider() : ColliderShape<D>(ColliderType::PLANE), Normal(0), Distance(0)
		{
			Normal[1] = 1; // y = 1 as default
			ColliderShape<D>::Bounds();
		}

		PlaneCollider(vec_t normal, float distance) : ColliderShape<D>(ColliderType::PLANE), Normal(normal), Distance(distance)
		{
			ColliderShape<D>::Bounds();
		}

		vec_t FindFurthestPoint(TransformComponent* transform, const vec_t& direction) const override
		{
			assert(false);
			return vec_t(0);
		}

		aabb_t CalcBounds() const
		{
			return aabb_t();
		}

		bool CacheIsOld() const override
		{
			return Normal != t_Normal || Distance != t_Distance;
		}

		void UpdateCache() override
		{
			t_Normal = Normal;
			t_Distance = Distance;

			ColliderShape<D>::UpdateCache();
		}

	private:
		vec_t t_Normal;
		float t_Distance;
	};
}