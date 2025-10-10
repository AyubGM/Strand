#include "sdpch.h"
#include "DynamicsSpace.h"

#include <assert.h>

namespace Strand {


	DynamicsSpace::DynamicsSpace()
	{
	}

	DynamicsSpace::~DynamicsSpace()
	{
	}

	void DynamicsSpace::AddRigidbody(Rigidbody* rigidbody)
	{
#ifdef SD_DEBUG

		if (rigidbody == nullptr)
		{
			SD_CORE_WARN("Tried to add null rigidbody to dynamics space");
			return;
		}
#endif // SD_DEBUG

		/*if (rigidbody->TakesGravity)
		{
			rigidbody->Gravity = m_Gravity;
		}

		AddCollisionObject(rigidbody);*/

	}

	void DynamicsSpace::AddConstaint(Constraint* constraint)
	{
		m_Constraints.push_back(constraint);
	}

	void DynamicsSpace::Step(Timestep ts)
	{
		if (Paused)
		{
			return;
		}

		TryApplyGravity();
		//ResolveConstrains(ts);

	//	for (CollisionObject* object : m_Objects)
		{

		}

		//for (Constraint* c : m_Constraints)
		{
			//c->init(dt);
		}
	}

}

