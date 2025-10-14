#pragma once

#include "Strand/Core/Base.h"
#include "Strand/Physics/AABB.h"
#include "Strand/Scene/Components.h"

namespace Strand
{

	enum class ColliderType
	{
		PLANE,
		SPHERE,
		CAPSULE,
		HULL,
		MESH
	};

	struct Collider;

	template<Dimension D>
	struct ColliderShape;

	struct Collider
	{
		const ColliderType Type;
		const Dimension Dim;

		Collider(ColliderType type, Dimension dim) : Type(type), Dim(dim) {}

		template<typename T>
		T* as() 
		{
			SD_CORE_ASSERT(T().get_id() == get_id());
			return (T*)this;
		}

		template<Dimension D>
		ColliderShape<D>* as_dim()
		{
			SD_CORE_ASSERT(D == Dim);
			return (ColliderShape<D>*)this;
		}

		uint32_t get_id() const {
			return (uint32_t)Type;
		}

		virtual bool CacheIsOld() const = 0;
		virtual void UpdateCache() = 0;

	};

	template<Dimension D>
	struct ColliderShape : Collider
	{
		using vec_t = _vec<D>;
		using aabb_t = AABB<D>;

		ColliderShape(ColliderType type) : Collider(type, D)
		{
			static_assert(D == Dimension::D2 || D == Dimension::D3);
		}

		virtual vec_t FindFurthestPoint(TransformComponent* transform, const vec_t& direction) const = 0;
		virtual aabb_t CalcBounds() const = 0;

		aabb_t Bounds()
		{
			if (CacheIsOld())
			{
				UpdateCache();
			}

			return t_Bounds;
		}

		void UpdateCache() override
		{
			t_Bounds = CalcBounds();
		}

	private:
		aabb_t t_Bounds;

	};

}