#ifndef RENDERABLE_OBJECTS_H
#define RENDERABLE_OBJECTS_H

#include "CollisionPrimitives.h"
#include "ApplicationSettings.h"

namespace PhysicsEngine
{
	class RectangleObject : public CollisionBox
	{
	public:
		RectangleObject()
		{
			body = new RigidBody();
		}

		~RectangleObject()
		{
			delete body;
		}

		// Set all of the parameters for this rigid body/Collision Box
		void setState(Vector3 position, Vector3 velocity, Vector3 acceleration, real mass, Vector3 halfSizeInput)
		{
			halfSize = halfSizeInput;
			body->setPosition(position);
			body->setVelocity(velocity);
			body->setAcceleration(acceleration);
			body->setMass(mass);
			Matrix3 tensor;
			tensor.setBlockInertiaTensor(halfSizeInput, mass);
			body->setInertiaTensor(tensor);
			body->setDamping(SQUARE_LINEAR_DAMPING, SQUARE_ANGULAR_DAMPING);
		}

		// Display this rectangle
		void display();
	};
}


#endif // RENDERABLE_OBJECTS_H