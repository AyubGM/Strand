#pragma once

#include "Rigidbody.h"
#include "glm/matrix.hpp"

namespace Strand {

	struct Constraint {
		Rigidbody* A;
		Rigidbody* B;

		Constraint(Rigidbody* a, Rigidbody* b): A(a), B(b) {}

		virtual void init(float dt) = 0;
		virtual bool solve(float dt) = 0; // Returns if constraint is solved

		bool Contains(Rigidbody* r) 
		{
			return r == A || r == B;
		}

		Rigidbody* Other(Rigidbody* r) 
		{
			if (r == A) return B;
			return A;
		}
	};

}