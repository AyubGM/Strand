#pragma once

#include "Strand/Physics/Collision/Collider.h"
#include "Strand/Physics/Collision/ManifoldPoints.h"

namespace Strand {

	template<Dimension D>
	struct Simplex
	{
	public:
		using vec_t = _vec<D>;

	public:
		Simplex(): m_Points(), m_Size(0)
		{
			static_assert(D == 2 || D == 3);
		}

		Simplex& operator=(std::initializer_list<vec_t> list)
		{
			for (auto v = list.begin(); v != list.end(); v++) 
			{
				m_Points[std::distance(list.begin(), v)] = *v;
			}
			m_Size = list.size();

			return *this;
		}

		void push_front(vec_t point)
		{
			if constexpr (D == 3) 
			{
				m_Points = { point, m_Points[0], m_Points[1], m_Points[2] };
			}

			else if constexpr (D == 2) 
			{
				m_Points = { point, m_Points[0], m_Points[1] };
			}

			m_Size = Math::Min<size_t>(m_Size + 1, D + 1);
		}

		vec_t& operator[](unsigned i) { return m_Points[i]; }
		size_t size() const { return m_Size; }

		auto begin() const { return m_Points.begin(); }
		auto end()   const { return m_Points.end() - (D + 1u - m_Size); }

	private:
		std::array<vec_t, D + 1u> m_Points;
		size_t m_Size;
	};

#ifndef GJK_EPA_MAX_ITER
#	define GJK_EPA_MAX_ITER 32
#endif
	template<Dimension D>
	std::pair<bool, Simplex<D>> GJK(
		ColliderShape<D>* colliderA, TransformComponent* transformA,
		ColliderShape<D>* colliderB, TransformComponent* transformB)
	{
		using vec_t = _vec<D>;

		vec_t support = Support<D>(
			colliderA, transformA,
			colliderB, transformB, vec_t(1));

		Simplex<D> points;
		points.push_front(support);

		vec_t direction = -support;

		size_t iterations = 0;
		while (iterations++ < GJK_EPA_MAX_ITER) {
			support = Support<D>(
				colliderA, transformA,
				colliderB, transformB, direction);

			if (dot(support, direction) <= 0) {
				break;
			}

			points.push_front(support);

			if (NextSimplex<D>(points, direction)) {
				return std::make_pair(true, points);
			}
		}

		return std::make_pair(false, points);
	}

	template<Dimension D> bool NextSimplex(Simplex<D>& points, _vec<D>& direction);
	template<Dimension D> bool Line(Simplex<D>& points, _vec<D>& direction);
	template<Dimension D> bool Triangle(Simplex<D>& points, _vec<D>& direction);
	template<Dimension D> bool Tetrahedron(Simplex<D>& points, _vec<D>& direction);

	template<> bool NextSimplex(Simplex<Dimension::D2>& points, _vec<Dimension::D2>& direction);
	template<> bool Line(Simplex<Dimension::D2>& points, _vec<Dimension::D2>& direction);
	template<> bool Triangle(Simplex<Dimension::D2>& points, _vec<Dimension::D2>& direction);

	template<> bool NextSimplex(Simplex<Dimension::D3>& points, _vec<Dimension::D3>& direction);
	template<> bool Line(Simplex<Dimension::D3>& points, _vec<Dimension::D3>& direction);
	template<> bool Triangle(Simplex<Dimension::D3>& points, _vec<Dimension::D3>& direction);
	template<> bool Tetrahedron(Simplex<Dimension::D3>& points, _vec<Dimension::D3>& direction);

	template<
		Dimension D>
	ManifoldPoints EPA(
		const Simplex <D>& simplex,
		ColliderShape<D>* colliderA, TransformComponent* transformA,
		ColliderShape<D>* colliderB, TransformComponent* transformB);

	template<>
	ManifoldPoints EPA(
		const Simplex <Dimension::D2>& simplex,
		ColliderShape<Dimension::D2>* colliderA, TransformComponent* transformA,
		ColliderShape<Dimension::D2>* colliderB, TransformComponent* transformB);

	template<>
	ManifoldPoints EPA(
		const Simplex <Dimension::D3>& simplex,
		ColliderShape<Dimension::D3>* colliderA, TransformComponent* transformA,
		ColliderShape<Dimension::D3>* colliderB, TransformComponent* transformB);

	template<
		Dimension D>
	_vec<D> Support(
		ColliderShape<D>* colliderA, TransformComponent* transformA,
		ColliderShape<D>* colliderB, TransformComponent* transformB,
		const _vec    <D>& direction)
	{
		return colliderA->FindFurthestPoint(transformA, direction)
			- colliderB->FindFurthestPoint(transformB, -direction);
	}

	template<
		Dimension D>
	bool SameDirection(
		const _vec<D>& direction,
		const _vec<D>& ao)
	{
		return dot(direction, ao) > 0;
	}

	// For Dimension::D3 EPA

	std::pair<std::vector<glm::vec4>, size_t> GetFaceNormals(
		const std::vector<glm::vec3>& polytope,
		const std::vector<size_t>& faces);

	void AddIfUniqueEdge(
		std::vector<std::pair<size_t, size_t>>& edges,
		const std::vector<size_t>& faces,
		size_t a,
		size_t b);



}