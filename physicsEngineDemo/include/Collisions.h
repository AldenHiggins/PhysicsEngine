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
		// Calculate the internals of this collision
		void calculateInternals(real duration);
		Vector3 calculateLocalVelocity(unsigned bodyIndex, real duration);
		void calculateDesiredDeltaVelocity(real duration);

		/**
		* Holds the required change in velocity for this contact to be
		* resolved.
		*/
		real desiredDeltaVelocity;

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

		/**
		* Holds the world space position of the contact point relative to
		* centre of each body. This is set when the calculateInternals
		* function is run.
		*/
		Vector3 relativeContactPosition[2];

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

	private:
		
		inline void calculateContactBasis();
		
		
		static bool boxAndHalfSpaceIntersect(const RigidBody *box, Vector3 planeDirection, real planeOffset);
		
		inline Vector3 calculateFrictionlessImpulse(Matrix3 * inverseInertiaTensor);
		inline Vector3 calculateFrictionImpulse(Matrix3 * inverseInertiaTensor);

		/**
		* Sets the number of iterations for each resolution stage.
		*/
		void setIterations(unsigned velocityIterations,
			unsigned positionIterations);
	};


}

#endif // COLLISIONS_H