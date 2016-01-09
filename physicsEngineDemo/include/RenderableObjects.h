#ifndef RENDERABLE_OBJECTS_H
#define RENDERABLE_OBJECTS_H

#include "CollisionPrimitives.h"
#include "ApplicationSettings.h"
#include <gl/glut.h>

namespace PhysicsDemo
{
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