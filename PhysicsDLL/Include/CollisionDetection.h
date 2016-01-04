#ifndef COLLISION_DETECTION_H
#define COLLISION_DETECTION_H

#include <vector>
#include "Collisions.h"
#include "CollisionPrimitives.h"

namespace PhysicsEngine
{
	class CollisionDetection
	{
	public:
		// Determine if there is a collision between two cubic rigid bodies
		static unsigned int cubeCubeCollisionDetect
		(
			std::vector<Collision> *collisionList,
			CollisionBox *first,
			CollisionBox *other
		);
		// Find collisions between a cube and a plane
		static unsigned boxAndHalfSpaceCollisionDetect
		(
			CollisionBox *box,
			const Vector3 planeDirection,
			real planeOffset,
			std::vector<Collision> *collisionList
		);

		// Determine if there is a collision between a sphere and a plane
		static unsigned sphereAndHalfSpaceCollisionDetect
		(
			CollisionSphere *sphere,
			const Vector3 planeDirection,
			real planeOffset,
			std::vector<Collision> *collisionList
		);

		// Determine if two spheres have collided
		static unsigned sphereSphereCollisionDetect
		(
			CollisionSphere *first,
			CollisionSphere *other,
			std::vector<Collision> *collisionList
		);

		// Determine if a sphere has collided with a cube
		static unsigned sphereCubeCollisionDetect
		(
			CollisionSphere *sphere,
			CollisionBox *cube,
			std::vector<Collision> *collisionList
		);

		// Determine if a capsule has collided with a plane
		static unsigned int capsuleHalfSpaceCollisionDetect
		(
			CollisionCapsule *capsule,
			const Vector3 planeDirection,
			real planeOffset,
			std::vector<Collision> *collisionList
		);

		// Determine if a capsule has collided with a sphere
		static unsigned int capsuleSphereCollisionDetect
		(
			CollisionCapsule *capsule,
			CollisionSphere *sphere,
			std::vector<Collision> *collisionList
		);

		// Determine if a capsule has collided with another capsule
		static unsigned int capsuleCapsuleCollisionDetect
		(
			CollisionCapsule *first,
			CollisionCapsule *second,
			std::vector<Collision> *collisionList
		);

		// Determine if a capsule has collided with a square
		static unsigned int capsuleSquareCollisionDetect
		(
			CollisionCapsule *first,
			CollisionBox *second,
			std::vector<Collision> *collisionList
		);
		
	private:
		// Perform an interesection test between the given box and plane
		static bool boxAndHalfSpaceIntersect
		(
			const CollisionBox *box,
			Vector3 planeDirection,
			real planeOffset
		);
		// Return the length of this box along the given axis
		static inline real transformToAxis
		(
			const CollisionBox *box,
			const Vector3 &axis
		);

		// Find the amount of penetration between the two objects on the given axis
		static inline real penetrationOnAxis
		(
			const CollisionBox *one,
			const CollisionBox *two,
			const Vector3 &axis,
			const Vector3 &toCentre
		);

		static inline bool CollisionDetection::tryAxis
		(
			const CollisionBox *one,
			const CollisionBox *two,
			Vector3 axis,
			const Vector3& toCentre,
			unsigned index,

			// These values may be updated
			real& smallestPenetration,
			unsigned &smallestCase
		);

		static void fillPointFaceBoxBox
		(
			CollisionBox *one,
			CollisionBox *two,
			const Vector3 &toCentre,
			std::vector<Collision> *data,
			unsigned best,
			real pen
		);

		static inline Vector3 contactPointCalculate
		(
			const Vector3 &pOne,
			const Vector3 &dOne,
			real oneSize,
			const Vector3 &pTwo,
			const Vector3 &dTwo,
			real twoSize,

			// If this is true, and the contact point is outside
			// the edge (in the case of an edge-face contact) then
			// we use one's midpoint, otherwise we use two's.
			bool useOne
		);
	};
}


#endif // COLLISION_DETECTION_H