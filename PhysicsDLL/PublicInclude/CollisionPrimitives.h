#ifndef COLLISION_PRIMITIVES_H
#define COLLISION_PRIMITIVES_H

#ifdef COLLISION_PRIMITIVES_H_EXPORTS
#define COLLISION_PRIMITIVES_H_API __declspec(dllexport) 
#else
#define COLLISION_PRIMITIVES_H_API __declspec(dllimport) 
#endif

#include "MathDataTypes.h"
#include "RigidBody.h"

namespace PhysicsEngine
{
	class CollisionPrimitive
	{
	public:
		RigidBody *body;
		//Matrix4 transformOffset;
	};

	class CollisionBox : public CollisionPrimitive
	{
	public:
		Vector3 halfSize;
		// Set all of the parameters for this collision box
		COLLISION_PRIMITIVES_H_API void setState(Vector3 position, Vector3 velocity, Vector3 acceleration, real mass, Vector3 halfSizeInput);
	};

	class CollisionSphere : public CollisionPrimitive
	{
	public:
		real radius;
		// Set all of the parameters for this collision sphere
		COLLISION_PRIMITIVES_H_API void setState(Vector3 position, Vector3 velocity, Vector3 acceleration, real mass, real radiusInput);
	};

	class CollisionCapsule : public CollisionPrimitive
	{
	public:
		COLLISION_PRIMITIVES_H_API  CollisionCapsule();

		real radius;
		real height;

		// Set all of the parameters for this rigid body/Collision Box
		COLLISION_PRIMITIVES_H_API void setState(Vector3 position, Quaternion rotation, Vector3 velocity, Vector3 acceleration, real mass, real radiusInput, real heightInput);
	};
}

#endif // COLLISION_PRIMITIVES_H