#pragma once

#include "Strand/Physics/Dynamics/Rigidbody.h"
#include "Strand/Math/Matrix.h"
#include "Strand/Math/MathTranslation.h"
#include "Strand/Core/Log.h"

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

	struct VelocityConstraint : Constraint {
		Math::Matrix J; // jacobian
		Math::Matrix W; // inv mass / inertia

		Math::Matrix M; // W * Jt

		Math::Matrix K; // constraint space
		Math::Matrix P; // saved impluse

		Math::Matrix E; // position error
		float biasStrength = .1f; // strength of position correction

		VelocityConstraint(
			Rigidbody* a,
			Rigidbody* b
		)
			: Constraint(a, b)
		{
		}

		virtual void init(
			float dt)
		{
			W = Math::Matrix(12, 1); W.diagonal = true;

			Rigidbody* r = A;
			glm::mat4 t = r->Transform.GetTransform();

			for (size_t i = 0; i < 12; i += 6)
			{
				Math::Matrix transformation(4, 4);

				for (size_t i = 0; i < 4; i++)
					for (size_t j = 0; j < 4; j++)
					{
						transformation.set(i, j, t[glm::length_t(j)][glm::length_t(i)]);
					}

				Math::Matrix x(4, 1);
				Math::Matrix y(4, 1);
				Math::Matrix z(4, 1);

				x.set(0, 1);
				y.set(1, 1);
				z.set(2, 1);

				Math::Matrix invInrtLocal = Math::From_glm(r->Inertia);

				Math::Matrix X = transformation * x;
				Math::Matrix Y = transformation * y;
				Math::Matrix Z = transformation * z;

				Math::Matrix world2Local(3, 3);

				world2Local.insert(0, 0, X.resize(3));
				world2Local.insert(1, 0, Y.resize(3));
				world2Local.insert(2, 0, Z.resize(3));

				Math::Matrix invInrtWorld = world2Local * invInrtLocal * transpose(world2Local); // prob a way to condence this /\

				W.set(i + 0, r->InvMass);             // [ 1/mass                           ]
				W.set(i + 1, r->InvMass);             // |     1/mass                       |
				W.set(i + 2, r->InvMass);             // |         1/mass                   |
				W.set(i + 3, invInrtWorld.get(0, 0)); // |              [                 ] |
				W.set(i + 4, invInrtWorld.get(1, 1)); // |              | inertial tensor | |
				W.set(i + 5, invInrtWorld.get(2, 2)); // [              [  (world space)  ] ]

				r = B;
			}

			M = W * transpose(J);
			K = J * M;

			if (P.height == 1) P = Math::vec<3>();
		}

		bool solve(
			float dt) override
		{
			Math::vec<12> v0;

			Math::vec<3> eA, eB;
			eA.set(0, E.get(0)); eB.set(0, E.get(0));
			eA.set(1, E.get(1)); eB.set(1, E.get(1));
			eA.set(2, E.get(2)); eB.set(2, E.get(2));

			Math::vec<3> vA = Math::From_glm(A->Velocity) - eA;
			Math::vec<3> vB = Math::From_glm(B->Velocity) + eB;

			v0.insert(0, 0, vA); v0.insert(0, 3, Math::From_glm(A->AngularVelocity));
			v0.insert(0, 6, vB); v0.insert(0, 9, Math::From_glm(B->AngularVelocity));

			Math::vec<3> R = -J * v0;

			Math::vec<3> lambda = P;

			for (size_t i = 0; i < K.height; i++)
			{
				float dR = R.get(i) - dot(K.row_begin(i), lambda.col_begin(0));
				lambda.get(i) += dR / K.get(i, i);
			}

			Math::Matrix impluse = M * lambda;

			if (A->IsAxisLocked.x && !B->IsAxisLocked.x) { impluse.get(6) -= impluse.get(0); impluse.get(0) = 0; } // is 1 object is locked add vel to other
			if (A->IsAxisLocked.y && !B->IsAxisLocked.y) { impluse.get(7) -= impluse.get(1); impluse.get(1) = 0; }
			if (A->IsAxisLocked.z && !B->IsAxisLocked.z) { impluse.get(8) -= impluse.get(2); impluse.get(2) = 0; }

			if (!A->IsAxisLocked.x && B->IsAxisLocked.x) { impluse.get(1) += impluse.get(6); impluse.get(6) = 0; }
			if (!A->IsAxisLocked.y && B->IsAxisLocked.y) { impluse.get(2) += impluse.get(7); impluse.get(7) = 0; }
			if (!A->IsAxisLocked.z && B->IsAxisLocked.z) { impluse.get(3) += impluse.get(8); impluse.get(8) = 0; }

			Rigidbody* r = A;
			for (size_t i = 0; i < 12; i += 6) {
				r->Velocity.x += impluse.get(i);
				r->Velocity.y += impluse.get(i + 1);
				r->Velocity.z += impluse.get(i + 2);
				r->AngularVelocity.x += impluse.get(i + 3);
				r->AngularVelocity.y += impluse.get(i + 4);
				r->AngularVelocity.z += impluse.get(i + 5);

				//r->Velocity        *= 0.98f;
				//r->AngularVelocity *= 0.98f;

				r = B;
			}

			P = lambda;

			return true; // Always 'solved'
		}
	};

	struct BallInSocketConstraint : VelocityConstraint {
		Math::vec<3> localAnchorA;
		Math::vec<3> localAnchorB;

		BallInSocketConstraint(
			Rigidbody* a,
			Rigidbody* b,
			Math::vec<3> localAnchorA,
			Math::vec<3> localAnchorB
		)
			: VelocityConstraint(a, b)
			, localAnchorA(localAnchorA)
			, localAnchorB(localAnchorB)
		{
		}

		Math::vec<3> target(Rigidbody* r) {
			if (r == A) return Math::From_glm(A->Transform.Rotation * (A->Transform.Scale * Math::To_glm(localAnchorA)));
			else        return Math::From_glm(B->Transform.Rotation * (B->Transform.Scale * Math::To_glm(localAnchorB)));
		}

		Math::vec<3> world_target(Rigidbody* r) {
			if (r == A) return Math::From_glm(A->Transform.Translation) + target(A);
			else        return Math::From_glm(B->Transform.Translation) + target(B);
		}

		void init(
			float dt) override
		{
			// translate target into local space for both objects

			Math::vec<3> r_A = target(A);
			Math::vec<3> r_B = target(B);

			J = Math::mat<3, 12>(); // [I -cross(a) -I cross(b)]

			J.insert(0, 0, Math::Matrix(3, 3, 1));
			J.insert(3, 0, Math::cross_3(r_A)); // this one should be neg tho?

			J.insert(6, 0, Math::Matrix(3, 3, -1));
			J.insert(9, 0, Math::cross_3(-r_B));

			E = (world_target(B) - world_target(A)) * (biasStrength / dt);

			VelocityConstraint::init(dt);
		}
	};

	inline void testMultiply() {
		Math::Matrix m(3, 3);

		m.set(0, 0, 1);
		m.set(1, 0, 2);
		m.set(2, 0, 3);
		m.set(0, 1, 4);
		m.set(1, 1, 5);
		m.set(2, 1, 6);
		m.set(0, 2, 7);
		m.set(1, 2, 8);
		m.set(2, 2, 9);

		Math::Matrix md(3, 1); md.diagonal = true;
		md.set(0, 1);
		md.set(1, 2);
		md.set(2, 3);

		Math::Matrix mm = m * m;
		Math::Matrix mmd = m * md;
		Math::Matrix mdm = md * m;
		Math::Matrix mdmd = md * md;

		/*SD_CORE_INFO("m: {0}", m);
		SD_CORE_INFO("md: {0}", md);

		SD_CORE_INFO("mm: {0}", mm);
		SD_CORE_INFO("mmd: {0}", mmd);
		SD_CORE_INFO("mdm: {0}", mdm);
		SD_CORE_INFO("mdmd: {0}", mdmd);*/

	}

	inline void testTranspose() {
		Math::Matrix m(3, 3);

		m.set(0, 0, 1);
		m.set(1, 0, 2);
		m.set(2, 0, 3);
		m.set(0, 1, 4);
		m.set(1, 1, 5);
		m.set(2, 1, 6);
		m.set(0, 2, 7);
		m.set(1, 2, 8);
		m.set(2, 2, 9);

		Math::Matrix mt = transpose(m);

		/*SD_CORE_INFO("m: {0}", m);
		SD_CORE_INFO("mt: {0}", mt);*/
	}

}