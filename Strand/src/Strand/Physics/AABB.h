#pragma once

#include "Strand/Core/Base.h"

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
			return (Min + MAX) / float(2);
		}

	};

}