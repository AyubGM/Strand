#pragma once

#include "Strand/Physics/Collision/Collider.h"
#include "Strand/Physics/Collision/ManifoldPoints.h"
#include "Strand/Physics/Collision/PlaneCollider.h"
#include "Strand/Physics/Collision/SphereCollider.h"
#include "Strand/Physics/Collision/CapsuleCollider.h"
#include "Strand/Physics/Collision/HullCollider.h"
#include "Strand/Physics/Collision/MeshCollider.h"
#include "Strand/Physics/Collision/GJK.h"
#include "Strand/Math/Math.h"

namespace Strand {

	ManifoldPoints TestCollision(
		Collider* a, TransformComponent* at,
		Collider* b, TransformComponent* bt);

	using TestCollisionCallbackFn = ManifoldPoints(*)(
		Collider*, TransformComponent*,
		Collider*, TransformComponent*);

	template<Dimension D>
	struct TestCollisionFuncs
	{
		TestCollisionCallbackFn tests[5][5] = {
			{nullptr, TestPlaneSphare<D> , TestPlaneCapsule  <D>, TestPlaneHullMesh<D>, TestPlaneHullMesh<D>},
			{nullptr, TestSphereSphere<D>, TestSphereCapsule <D>, TestGJK          <D>, TestXMesh        <D>},
			{nullptr, nullptr,             TestCapsuleCapsule<D>, TestGJK          <D>, TestXMesh        <D>},
			{nullptr, nullptr,             nullptr,				  TestGJK          <D>, TestXMesh        <D>},
			{nullptr, nullptr,             nullptr,				  nullptr,              TestMeshMesh     <D>},
		};
	};

	template<Dimension D>
	_vec<D> RotVec(_vec<D> v, TransformComponent* t)
	{
		if constexpr (D == Dimension::D3)
		{
			v = v * t->Rotation;
		}
		else 
		{
			v = (_vec<D>)(_vec<Dimension::D3>(v, 0) * t->Rotation);
		}

		return v;
	}

	template<Dimension D>
	ManifoldPoints TestPlaneSphare(
		Collider* a, TransformComponent* at,
		Collider* b, TransformComponent* bt)
	{
		assert(a->Type == ColliderType::PLANE
			&& b->Type == ColliderType::SPHERE
			&& a->Dim == D
			&& b->Dim == D);

		using Plane = PlaneCollider<D>;
		using Sphere = SphereCollider<D>;
		using vec_t = _vec<D>;

		Plane* A = (Plane*)a;
		Sphere* B = (Sphere*)b;

		vec_t aCenter = B->Center + (vec_t)bt->Translation;
		float aRadius = B->Radius * Major(bt->Scale);
		vec_t normal = RotVec<D>(glm::normalize(A->Normal), at);
		vec_t onPlane = normal * A->Distance + (vec_t)at->Translation;

		float distance = glm::dot(aCenter - onPlane, normal);

		if (distance > aRadius) {
			return ManifoldPoints();
		}

		vec_t aDeep = aCenter - normal * aRadius;
		vec_t bDeep = aCenter - normal * distance;

		return ManifoldPoints(aDeep, bDeep, normal, distance);
	}

	template<Dimension D>
	ManifoldPoints TestPlaneCapsule(
		Collider* a, TransformComponent* at,
		Collider* b, TransformComponent* bt)
	{
		assert(a->Type == ColliderType::PLANE
			&& b->Type == ColliderType::CAPSULE
			&& a->Dim == D
			&& b->Dim == D);

		using Plane = PlaneCollider<D>;
		using Capsule = CapsuleCollider<D>;

		const Plane* A = (Plane*)a;
		const Capsule* B = (Capsule*)b;

		return ManifoldPoints();
	}

	template<
		Dimension D>
	ManifoldPoints TestPlaneHullMesh(
		Collider* a, TransformComponent* at,
		Collider* b, TransformComponent* bt)
	{
		assert(a->Type == ColliderType::PLANE
			&& (b->Type == ColliderType::HULL || b->Type == ColliderType::MESH)
			&& a->Dim == D
			&& b->Dim == D);

		using Plane = PlaneCollider<D>;
		using Hull = HullCollider<D>;
		using vec_t = _vec<D>;

		Plane* A = (Plane*)a;
		Hull* B = (Hull*)b;

		vec_t normal = RotVec<D>(normalize(A->Normal), at);

		vec_t plane = normal * A->Distance + (vec_t)at->Translation;
		vec_t bDeep = B->FindFurthestPoint(bt, -normal);

		vec_t ba = plane - bDeep;

		float distance = dot(ba, normal);

		if (distance < 0) {
			return ManifoldPoints();
		}

		// Might nudge 'plane' twoards bDeep (furthest point of plane in B)

		return ManifoldPoints(plane, bDeep, -normal, distance);
	}

	template<Dimension D>
	ManifoldPoints TestSphereSphere(
		Collider* a, TransformComponent* at,
		Collider* b, TransformComponent* bt)
	{
		assert(a->Type == ColliderType::SPHERE
			&& b->Type == ColliderType::SPHERE
			&& a->Dim == D
			&& b->Dim == D);

		using Sphere = SphereCollider<D>;
		using vec_t = _vec<D>;

		Sphere* A = (Sphere*)a;
		Sphere* B = (Sphere*)b;

		vec_t aCenter = A->Center + (vec_t)at->Translation;
		vec_t bCenter = B->Center + (vec_t)bt->Translation;

		vec_t ab = bCenter - aCenter;

		float aRadius = A->Radius * Major(at->Scale);
		float bRadius = B->Radius * Major(bt->Scale);

		float distance = length(ab);

		if (distance < 0.00001f
			|| distance > aRadius + bRadius)
		{
			return ManifoldPoints();
		}

		vec_t normal = normalize(ab);

		vec_t aDeep = aCenter + normal * aRadius;
		vec_t bDeep = bCenter - normal * bRadius;

		return ManifoldPoints(aDeep, bDeep);
	}

	template<Dimension D>
	ManifoldPoints TestSphereCapsule(
		Collider* a, TransformComponent* at,
		Collider* b, TransformComponent* bt)
	{
		assert(a->Type == ColliderType::SPHERE
			&& b->Type == ColliderType::CAPSULE
			&& a->Dim == D
			&& b->Dim == D);

		using Sphere = SphereCollider<D>;
		using Capsule = CapsuleCollider<D>;
		using vec_t = _vec<D>;

		Sphere* A = (Sphere*)a;
		Capsule* B = (Capsule*)b;

		vec_t bScale = bt->Scale;

		float bHeightScale = bScale.y;
		float bRadiusScale = bScale.x;

		if constexpr (D == Dimension::D3) {
			bRadiusScale = Math::Max(bRadiusScale, bScale.z);
		}

		vec_t y(0); y[1] = 1;

		vec_t bOffset = RotVec<D>(y, bt) * (B->Height * bHeightScale / 2 - B->Radius * bRadiusScale);

		vec_t aCenter = A->Center + (vec_t)at->Translation;
		vec_t bBottom = B->Center - bOffset + (vec_t)bt->Translation;
		vec_t bTop = B->Center + bOffset + (vec_t)bt->Translation;

		float aRadius = A->Radius * Math::Max(bHeightScale, bRadiusScale);
		float bRadius = B->Radius * bRadiusScale;

		vec_t ba = aCenter - bBottom;
		vec_t bbt = bTop - bBottom;
		vec_t nbbt = normalize(bbt);

		vec_t bCenterProj = bBottom + nbbt * Math::Clamp(dot(nbbt, ba), 0.0f, length(bbt));

		vec_t bp = aCenter - bCenterProj;

		float distance = length(bp);

		if (distance > aRadius + bRadius) {
			return ManifoldPoints();
		}

		vec_t normal = normalize(bp);

		vec_t aDeep = aCenter - normal * aRadius;
		vec_t bDeep = bCenterProj + normal * bRadius;

		return ManifoldPoints(aDeep, bDeep);
	}

	template<Dimension D>
	ManifoldPoints TestCapsuleCapsule(
		Collider* a, TransformComponent* at,
		Collider* b, TransformComponent* bt)
	{
		assert(a->Type == ColliderType::CAPSULE
			&& b->Type == ColliderType::CAPSULE
			&& a->Dim == D
			&& b->Dim == D);

		using Capsule = CapsuleCollider<D>;

		const Capsule* A = (Capsule*)a;
		const Capsule* B = (Capsule*)b;

		return ManifoldPoints();
	}


	// For meshes

	inline ManifoldPoints GetMaxPen(std::vector<ManifoldPoints>& manifolds)
	{
		if (manifolds.size() == 0) return ManifoldPoints(); // exit if no collision

		size_t maxNormalIndex = 0;
		float  maxNormalDist = FLT_MIN;

		for (size_t i = 0; i < manifolds.size(); i++) {
			if (manifolds[i].Depth > maxNormalDist) {
				maxNormalDist = manifolds[i].Depth;
				maxNormalIndex = i;
			}
		}

		return manifolds[maxNormalIndex];
	}

	template<Dimension D>
	ManifoldPoints TestXMesh(
		Collider* a, TransformComponent* at,
		Collider* b, TransformComponent* bt)
	{
		assert(a->Type != ColliderType::MESH
			&& b->Type == ColliderType::MESH
			&& a->Dim == D
			&& b->Dim == D);

		using Collider = ColliderShape<D>;
		using Mesh = MeshCollider<D>;

		Collider* A = (Collider*)a;
		Mesh* B = (Mesh*)b;

		std::vector<ManifoldPoints> manifolds;

		for (auto& part : B->ConvexParts)
		{
			if (!part.Bounds().Intersects(bt, A->Bounds(), at)) continue;

			auto [collision, simplex] = GJK(A, at, &part, bt);
			if (collision) {
				manifolds.push_back(EPA(simplex, A, at, &part, bt));
			}
		}

		return GetMaxPen(manifolds);
	}
	

	template<Dimension D>
	ManifoldPoints TestMeshMesh(
		Collider* a, TransformComponent* at,
		Collider* b, TransformComponent* bt)
	{
		assert(a->Type == ColliderType::MESH
			&& b->Type == ColliderType::MESH
			&& a->Dim == D
			&& b->Dim == D);

		using Mesh = MeshCollider<D>;

		Mesh* A = (Mesh*)a;
		Mesh* B = (Mesh*)b;

		std::vector<ManifoldPoints> manifolds;

		for (auto& aPart : A->ConvexParts)
			for (auto& bPart : B->ConvexParts)
			{
				// could add each triangle to a broad phase...
				if (!aPart.Bounds().Intersects(at, bPart.Bounds(), bt)) continue;

				auto [collision, simplex] = GJK(&aPart, at, &bPart, bt);
				if (collision) {
					manifolds.push_back(EPA(simplex, &aPart, at, &bPart, bt));
				}
			}

		return GetMaxPen(manifolds);
	}
	

	template<Dimension D>
	ManifoldPoints TestGJK(
		Collider* a, TransformComponent* at,
		Collider* b, TransformComponent* bt)
	{
		assert((a->Type == ColliderType::HULL || b->Type == ColliderType::HULL)
			&& (a->Type != ColliderType::MESH && b->Type != ColliderType::MESH)
			&& a->Dim == D
			&& b->Dim == D);

		using Collider = ColliderShape<D>;

		Collider* A = (Collider*)a;
		Collider* B = (Collider*)b;

		auto [collision, simplex] = GJK(A, at, B, bt);

		if (collision) {
			return EPA(simplex, A, at, B, bt);
		}

		return ManifoldPoints();
	}
}