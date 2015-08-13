#ifndef COLLISIONS_H
#define COLLISIONS_H

#include "RigidBody.h"
#include <vector>

namespace PhysicsEngine
{
	struct Collision
	{
		RigidBody *firstObject;
		RigidBody *secondObject;

		Vector3 contactPoint;
		Vector3 contactNormal;
		real penetration;
	public:
		static unsigned int cubeCubeCollisionDetect(std::vector<Collision> *collisionList, RigidBody *first, RigidBody *other);
	};


}

#endif // COLLISIONS_H