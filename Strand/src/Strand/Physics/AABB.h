#pragma once

#include "Strand/Core/Base.h"
#include "Strand/Scene/Components.h"

namespace Strand {

	template<Dimension D>
	struct AABB {
		using vec_t = _vec<D>;
		using pair_t = std::pair<vec_t, vec_t>;

		vec_t Min;
		vec_t Max;

		AABB() : Min(FLT_MAX), Max(-FLT_MAX) {}

		AABB(const vec_t& center, float scale) : Min(center - scale), Max(center + scale) {}

		AABB(const vec_t& p1, const vec_t& p2)
		{
			*this = FromPair(pair_t(p1, p2));
		}

		AABB(const pair_t& ps)
		{
			*this = FromPair(ps);
		}

		AABB& operator=(const pair_t& ps)
		{
			*this = FromPair(ps);
			return *this;
		}

		operator pair_t() const
		{
			return pair_t(Min, Max);
		}

		vec_t Center() const 
		{
			return (Min + Max) / float(2);
		}

		bool Intersects(TransformComponent* transform, const AABB& other, TransformComponent* otherTransform) const
		{
			AABB me = TransformBounds(*this, transform);
			AABB them = TransformBounds(other, otherTransform);

			for (uint32_t i = 0; i < uint32_t(D); i++) {
				if (me.Min[i] > them.Max[i] || me.Max[i] < them.Min[i]) return false;
			}
			return true;
		}

		bool Fits(TransformComponent* transform, const AABB& other, TransformComponent* otherTransform) const
		{
			AABB me = TransformBounds(*this, transform);
			AABB them = TransformBounds(other, otherTransform);

			for (uint32_t i = 0; i < uint32_t(D); i++) {
				if (me.Min[i] >= them.Min[i] || me.Max[i] <= them.Max[i]) return false;
			}
			return true;
		}

	private:
		static AABB FromPair(const std::pair<_vec<D2>, _vec<D2>>& ps)
		{
			const auto& [p1, p2] = ps;

			bool p1x = p1.x < p2.x;
			bool p1y = p1.y < p2.y;

			AABB aabb;
			aabb.Min.x = p1x ? p1.x : p2.x;
			aabb.Min.y = p1y ? p1.y : p2.y;
			aabb.Max.x = !p1x ? p1.x : p2.x;
			aabb.Max.y = !p1y ? p1.y : p2.y;

			return aabb;
		}

		static AABB FromPair(
			const std::pair<_vec<D3>, _vec<D3>>& ps)
		{
			const auto& [p1, p2] = ps;

			bool p1x = p1.x < p2.x;
			bool p1y = p1.y < p2.y;
			bool p1z = p1.z < p2.z;

			AABB aabb;
			aabb.Min.x = p1x ? p1.x : p2.x;
			aabb.Min.y = p1y ? p1.y : p2.y;
			aabb.Min.z = p1z ? p1.z : p2.z;
			aabb.Max.x = !p1x ? p1.x : p2.x;
			aabb.Max.y = !p1y ? p1.y : p2.y;
			aabb.Max.z = !p1z ? p1.z : p2.z;

			return aabb;
		}
	};

}