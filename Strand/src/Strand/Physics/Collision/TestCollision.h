#pragma once

#include "Strand/Physics/Collision/Collider.h"
#include "Strand/Physics/Collision/ManifoldPoints.h"
#include "PlaneCollider.h"
#include "SphereCollider.h"

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
			{nullptr, TestPlaneSphare<D> , nullptr /*TestPlaneCapsule  <D>*/, nullptr /*TestPlaneHullMesh<D>*/, nullptr /*TestPlaneHullMesh<D>*/ },
			{nullptr, TestSphereSphere<D>, nullptr /*TestSphereCapsule <D>*/, nullptr /*TestGJK            <D>*/, nullptr /*TestXMesh         <D>*/ },
			{nullptr, nullptr,             nullptr /*TestCapsuleCapsule<D>*/, nullptr /*TestGJK            <D>*/,  nullptr /*TestXMesh         <D>*/ },
			{nullptr, nullptr,             nullptr,    nullptr      /*TestGJK            <D>*/, nullptr/* TestXMesh         <D> */},
			{nullptr, nullptr,             nullptr,    nullptr,                nullptr /*TestMeshMesh      <D>*/ },
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
}