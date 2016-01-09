#ifndef RENDERABLE_OBJECTS_H
#define RENDERABLE_OBJECTS_H

#include "CollisionPrimitives.h"
#include "ApplicationSettings.h"
#include <gl/glut.h>

namespace PhysicsDemo
{
	class Plane
	{
	public:
		PhysicsEngine::Vector3 position;
		PhysicsEngine::Vector3 normal;
		PhysicsEngine::Vector3 up;
		PhysicsEngine::Vector3 color;
		PhysicsEngine::real halfSize;

		Plane
		(
			PhysicsEngine::Vector3 positionInput,
			PhysicsEngine::Vector3 normalInput,
			PhysicsEngine::Vector3 upInput,
			PhysicsEngine::Vector3 colorInput,
			PhysicsEngine::real halfSizeInput
		)
		{
			position = positionInput;
			normal = normalInput;
			up = upInput;
			color = colorInput;
			halfSize = halfSizeInput;
		}

		~Plane()
		{
		}

		// Display this plane
		void display();
	};

	class Box
	{
	public:
		PhysicsEngine::CollisionBox *boxPrimitive;

		Box()
		{
			boxPrimitive = new PhysicsEngine::CollisionBox();
		}

		~Box()
		{
			delete boxPrimitive;
		}

		// Display this rectangle
		void display();
	};

	class Sphere
	{
	public:
		PhysicsEngine::CollisionSphere *spherePrimitive;

		Sphere()
		{
			spherePrimitive = new PhysicsEngine::CollisionSphere();
		}

		~Sphere()
		{
			delete spherePrimitive;
		}


		// Display this sphere object
		void display();
	};


	class Capsule
	{
	public:
		PhysicsEngine::CollisionCapsule *capsulePrimitive;

		Capsule();

		~Capsule();

		// Display this sphere object
		void display();

	private:
		GLUquadric *quadricObject;
	};
}


#endif // RENDERABLE_OBJECTS_H