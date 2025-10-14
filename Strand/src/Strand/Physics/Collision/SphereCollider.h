#pragma once

#include "Collider.h"
#include "Strand/Physics/Physics.h"

namespace Strand {

	template<Dimension D>
	struct SphereCollider : ColliderShape<D>
	{
		using vec_t = _vec<D>;
		using aabb_t = AABB<D>;

		vec_t Center;
		float Radius;

	public:
		SphereCollider() : ColliderShape<D>(ColliderType::SPHERE), Center(0.0f), Radius(1.0f)
		{
			ColliderShape<D>::Bounds();
		}

		SphereCollider(vec_t center, float radius) : ColliderShape<D>(ColliderType::SPHERE), Center(center), Radius(radius)
		{
			ColliderShape<D>::Bounds();
		}

		vec_t FindFurthestPoint(TransformComponent* transform, const vec_t& direction) const override
		{
			return Center + (vec_t)transform->Translation
				+ Radius * glm::normalize(direction) * Major(transform->Scale);
		}

		aabb_t CalcBounds() const
		{
			return aabb_t(Center, sqrt(Radius * Radius + Radius * Radius));
		}

		bool CacheIsOld() const override
		{
			return  Center != t_Center
				|| Radius != t_Radius;
		}

		void UpdateCache() override
		{
			t_Center = Center;
			t_Radius = Radius;

			ColliderShape<D>::UpdateCache();
		}
	private:
		vec_t t_Center;
		float t_Radius;

	};

}