#ifndef COLLISIONS_H
#define COLLISIONS_H

#include "RigidBody.h"
#include <vector>

namespace PhysicsEngine
{
	class Collision
	{
	public:
		// The first object taking part in this collision
		RigidBody *firstObject;
		// The second object in the collision
		RigidBody *secondObject;
		// The point where the collision occured
		Vector3 contactPoint;
		// The normal pointing outwards in the direction of the collision
		Vector3 contactNormal;
		// The amount the two objects are interpenetrating
		real penetration;
		// Reduces the amount of velocity resulting from a collision
		real friction;
		// Determine if there is a collision between two cubic rigid bodies
		static unsigned int cubeCubeCollisionDetect(std::vector<Collision> *collisionList, RigidBody *first, RigidBody *other);
		// Find collisions between a cube and a plane
		static unsigned boxAndHalfSpace(RigidBody *box, const Vector3 planeDirection, real planeOffset, std::vector<Collision> *collisionList);
		// Add the required velocities and change the positions of the rigid bodies involved in the contacts in contactList
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
		static bool boxAndHalfSpaceIntersect(const RigidBody *box, Vector3 planeDirection, real planeOffset);
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
		inline Vector3 calculateFrictionImpulse(Matrix3 * inverseInertiaTensor);
	};


}

#endif // COLLISIONS_H