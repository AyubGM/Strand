#pragma once

#include "Strand/Physics/Collision/HullCollider.h"

namespace Strand {

	template<Dimension D>
	struct MeshCollider : HullCollider<D>
	{
		using vec_t = _vec<D>;
		using aabb_t = AABB<D>;
		using hull_t = HullCollider<D>;

		std::vector<uint32_t> Triangles;
		std::vector<hull_t> ConvexParts;

		MeshCollider(
			const std::vector<vec_t>& points = {},
			const std::vector<uint32_t>& triangles = {}
		) : hull_t(points, true), Triangles(triangles)
		{}

		void AddTriangle(uint32_t a, uint32_t b, uint32_t c)
		{
			Triangles.push_back(a);
			Triangles.push_back(b);
			Triangles.push_back(c);
		}

		void RemoveTriangle(uint32_t a, uint32_t b, uint32_t c)
		{
			for (auto itr = Triangles.begin(); itr != Triangles.end(); itr += 3)
			{
				if (*(itr) == a && *(itr + 1) == b && *(itr + 2) == c)
				{
					Triangles.erase(itr, itr + 3); //TODO NOT SURE ABOUT THIS ONE YET
					break;
				}
			}
		}

		bool CacheIsOld() const override
		{
			size_t size = Triangles.size();

			if (size != t_Triangles.size())
			{
				return true;
			}

			for (size_t i = 0; i < size; i++)
			{
				if (Triangles.at(i) != t_Triangles.at(i))
				{
					return true;
				}
			}

			return false;

		}

		void UpdateCache() override
		{
			t_Triangles = Triangles;

			ConvexParts.clear();

			for (size_t i = 0; i < Triangles.size(); i += 3)
			{
				ConvexParts.push_back(hull_t({
					this->Points[Triangles[i]],
					this->Points[Triangles[i + 1]],
					this->Points[Triangles[i + 2]],
					}));
			}

			ColliderShape<D>::UpdateCache();
		}

	private:
		std::vector<uint32_t> t_Triangles;
	};
}