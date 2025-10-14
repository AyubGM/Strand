#include "sdpch.h"

#include "Strand/Physics/Collision/TestCollision.h"

namespace Strand {

	ManifoldPoints TestCollision(
		Collider* a, TransformComponent* at,
		Collider* b, TransformComponent* bt)
	{
		static TestCollisionFuncs<Dimension::D2> d2;
		static TestCollisionFuncs<Dimension::D3> d3;

		if (a->Dim != b->Dim) 
		{
			SD_CORE_WARN("Tried to collide 2d and 3d objects!");
			return {};
		}

		Dimension dim = a->Dim;

		uint32_t aid = a->get_id();
		uint32_t bid = b->get_id();

		bool swap = aid > bid;

		if (swap)
		{
			uint32_t tid = aid;
			aid = bid;
			bid = tid;

			Collider* tc = a;

			a = b;
			b = tc;

			TransformComponent* tt = at;
			at = bt;
			bt = tt;
		}

		ManifoldPoints points;

		if (dim == Dimension::D2)
		{
			auto& func = d2.tests[aid][bid];
			if (func)
			{
				points = func(a, at, b, bt);
			}
		}
		else if (dim == Dimension::D3)
		{
			auto& func = d3.tests[aid][bid];
			if (func)
			{
				points = func(a, at, b, bt);
			}
		}

		if (swap && points.HasCollision)
		{
			points.SwapPoints();
		}

		return points;
	}

}