#pragma once

#include "Strand/Physics/Collision/Collider.h"

namespace Strand {

	template<Dimension D>
	struct CapsuleCollider : ColliderShape<D>
	{
		using vec_t = _vec<D>;
		using aabb_t = AABB<D>;

		vec_t Center;
		float Radius;
		float Height;

		CapsuleCollider(): ColliderShape<D>(ColliderType::CAPSULE), Center(0.0f), Height(2.0f), Radius(0.5f)
		{
			ColliderShape<D>::Bounds();
		}

		CapsuleCollider(vec_t center, float height, float radius): ColliderShape<D>(ColliderType::CAPSULE), Center(center), Height(height), Radius(radius)
		{
			ColliderShape<D>::Bounds();
		}

		vec_t FindFurthestPoint(TransformComponent* transform, const vec_t& direction) const override
		{
			assert(false);
			return vec_t(0);
		}

		// TODO: check if this is correct
		aabb_t CalcBounds() const
		{
			vec_t y(0); y[1] = 1;

			vec_t a = Center + y * (Height / 2) + vec_t(1) * Radius;
			vec_t b = Center - y * (Height / 2) - vec_t(1) * Radius;

			return aabb_t(a, b);
		}

		bool CacheIsOld() const override
		{
			return  Center != t_Center || Radius != t_Radius || Height != t_Height;
		}

		void UpdateCache() override
		{
			t_Center = Center;
			t_Radius = Radius;
			t_Height = Height;

			ColliderShape<D>::UpdateCache();
		}

	private:
		vec_t t_Center;
		float t_Radius;
		float t_Height;
	};
}