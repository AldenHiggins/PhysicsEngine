#ifndef RENDERABLE_OBJECTS_H
#define RENDERABLE_OBJECTS_H

#include "CollisionPrimitives.h"
#include "ApplicationSettings.h"
#include <gl/glut.h>

namespace PhysicsEngine
{
	class Box
	{
	public:
		CollisionBox *boxPrimitive;

		Box()
		{
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
		CollisionSphere *spherePrimitive;

		Sphere()
		{
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
		CollisionCapsule *capsulePrimitive;

		Capsule();

		~Capsule()
		{
			delete capsulePrimitive;
		}

		// Display this sphere object
		void display();

	private:
		GLUquadric *quadricObject;
	};
}


#endif // RENDERABLE_OBJECTS_H