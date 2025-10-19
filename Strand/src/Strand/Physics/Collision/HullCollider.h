#pragma once

#include "Strand/Physics/Collision/Collider.h"
#include "Strand/Physics/Physics.h"
#include "Strand/Common/Algos/Polygon.h"
#include "Strand/Math/Math.h"

namespace Strand {

	template<Dimension D>
	struct HullCollider : ColliderShape<D>
	{
		using vec_t = _vec<D>;
		using aabb_t = AABB<D>;

		std::vector<vec_t> Points;

		HullCollider(const std::vector<vec_t>& points = {}, bool isMesh = false) : ColliderShape<D>(isMesh ? ColliderType::MESH : ColliderType::HULL)
			, Points(points)
		{
			ColliderShape<D>::Bounds();
		}

		virtual void AddPoint(const vec_t& p)
		{
			Points.push_back(p);
		}

		virtual void RemovePoint(const vec_t& p)
		{
			Points.erase(std::find(Points.begin(), Points.end(), p));
		}

		vec_t FindFurthestPoint(TransformComponent* transform, const vec_t& direction) const override
		{
			vec_t maxPoint = vec_t(0);
			float maxDistance = -FLT_MAX;

			for (const vec_t& point : Points)
			{
				vec_t p = Physics::TransformPoint<D>(point, transform);

				float distance = glm::dot(p, direction);
				if(distance > maxDistance) 
				{
					maxDistance = distance;
					maxPoint = p;
				}
			}

			return maxPoint;
		}

		aabb_t CalcBounds() const
		{
			return Common::GenPolygonBounds(Points);
		}

		bool CacheIsOld() const override
		{
			size_t size = Points.size();

			if (size != t_Points.size())
			{
				return true;
			}

			for (size_t i = 0; i < size; i++)
			{
				if (Points.at(i) != t_Points.at(i))
				{
					return true;
				}
			}

			return false;
		}

		void UpdateCache() override
		{
			t_Points = Points;
			ColliderShape<D>::UpdateCache();
		}

	private:
		std::vector<vec_t> t_Points;

	};

	inline HullCollider<Dimension::D3> MakeCubeCollider() 
	{
		HullCollider<Dimension::D3> collider;
		collider.AddPoint(glm::vec3(-1, -1, -1)); // 0 
		collider.AddPoint(glm::vec3(-1, 1, -1)); // 1 
		collider.AddPoint(glm::vec3(1, 1, -1)); // 2 
		collider.AddPoint(glm::vec3(1, -1, -1)); // 3 

		collider.AddPoint(glm::vec3(-1, -1, 1)); // 7
		collider.AddPoint(glm::vec3(1, -1, 1)); // 6
		collider.AddPoint(glm::vec3(1, 1, 1)); // 5
		collider.AddPoint(glm::vec3(-1, 1, 1)); // 4

		return collider;
	}


	inline HullCollider<Dimension::D3> MakeTetrahedronCollider() 
	{
		HullCollider<Dimension::D3> collider;
		collider.AddPoint(glm::vec3(cos(Math::Pi2 * 0 / 3), -1, sin(Math::Pi2 * 0 / 3)));
		collider.AddPoint(glm::vec3(cos(Math::Pi2 * 1 / 3), -1, sin(Math::Pi2 * 1 / 3)));
		collider.AddPoint(glm::vec3(cos(Math::Pi2 * 2 / 3), -1, sin(Math::Pi2 * 2 / 3)));
		collider.AddPoint(glm::vec3(0, 1, 0));

		return collider;
	}

	inline HullCollider<Dimension::D2> MakeSquareCollider() 
	{
		HullCollider<Dimension::D2> collider;
		collider.AddPoint(glm::vec2(-1, 1)); // 0 
		collider.AddPoint(glm::vec2(-1, -1)); // 1 
		collider.AddPoint(glm::vec2(1, -1)); // 2 
		collider.AddPoint(glm::vec2(1, 1)); // 3 

		return collider;
	}
	inline HullCollider<Dimension::D2> MakeTriangleCollider() 
	{
		HullCollider<Dimension::D2> collider;
		collider.AddPoint(glm::vec2(cos(Math::Pi2 * 0 / 3), sin(Math::Pi2 * 0 / 3)));
		collider.AddPoint(glm::vec2(cos(Math::Pi2 * 1 / 3), sin(Math::Pi2 * 1 / 3)));
		collider.AddPoint(glm::vec2(cos(Math::Pi2 * 2 / 3), sin(Math::Pi2 * 2 / 3)));

		return collider;
	}
}

