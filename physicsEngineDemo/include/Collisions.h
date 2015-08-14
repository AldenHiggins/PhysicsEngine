#ifndef COLLISIONS_H
#define COLLISIONS_H

#include "RigidBody.h"
#include <vector>

namespace PhysicsEngine
{
	class Collision
	{
	public:
		RigidBody *firstObject;
		RigidBody *secondObject;

		Vector3 contactPoint;
		Vector3 contactNormal;
		real penetration;

		static unsigned int cubeCubeCollisionDetect(std::vector<Collision> *collisionList, RigidBody *first, RigidBody *other);
		static void resolveContacts(std::vector<Collision> *collisionList, real duration);
	protected:
		/**
		* A transform matrix that converts co-ordinates in the contact's
		* frame of reference to world co-ordinates. The columns of this
		* matrix form an orthonormal set of vectors.
		*/
		Matrix3 contactToWorld;

		/**
		* Holds the closing velocity at the point of contact. This is set
		* when the calculateInternals function is run.
		*/
		Vector3 contactVelocity;

		/**
		* Holds the required change in velocity for this contact to be
		* resolved.
		*/
		real desiredDeltaVelocity;

		/**
		* Holds the world space position of the contact point relative to
		* centre of each body. This is set when the calculateInternals
		* function is run.
		*/
		Vector3 relativeContactPosition[2];
	private:
		static void prepareContacts(std::vector<Collision> *collisionList, real duration);
		static void adjustVelocities(std::vector<Collision> *collisionList, real duration);
		static void adjustPositions(std::vector<Collision> *collisionList, real duration);
		inline void calculateContactBasis();
		void calculateInternals(real duration);
		Vector3 calculateLocalVelocity(unsigned bodyIndex, real duration);
		void calculateDesiredDeltaVelocity(real duration);
		/**
		* Performs an inertia-weighted impulse based resolution of this
		* contact alone.
		*/
		void applyVelocityChange(Vector3 velocityChange[2], Vector3 rotationChange[2]);

		/**
		* Performs an inertia weighted penetration resolution of this
		* contact alone.
		*/
		void applyPositionChange(Vector3 linearChange[2], Vector3 angularChange[2], real penetration);
		inline Vector3 calculateFrictionlessImpulse(Matrix3 * inverseInertiaTensor);
	};


}

#endif // COLLISIONS_H