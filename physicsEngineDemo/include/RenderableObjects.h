#ifndef RENDERABLE_OBJECTS_H
#define RENDERABLE_OBJECTS_H

#include "CollisionPrimitives.h"
#include "ApplicationSettings.h"
#include "DataTypeRedefinition.h"
#include <gl/glut.h>

namespace PhysicsDemo
{
	class RenderingDemo;

	class Renderable
	{
	public:
		virtual void display() = 0;
	};

	class Axis : Renderable
	{
	private:
		Vector3 color;
		Vector3 firstPoint;
		Vector3 secondPoint;
		Vector3 thirdPoint;
		Vector3 fourthPoint;
	public:
		Axis
		(
			RenderingDemo *demo,
			Vector3 colorInput,
			Vector3 firstPointInput,
			Vector3 secondPointInput,
			Vector3 thirdPointInput,
			Vector3 fourthPointInput
		);

		void display();
	};

	class Plane : Renderable
	{
	public:
		CollisionPlane *plane;

		Vector3 position;
		Vector3 normal;
		Vector3 up;
		Vector3 color;
		real halfSize;

		Plane
		(
			RenderingDemo *demo,
			Vector3 positionInput,
			Vector3 normalInput,
			Vector3 upInput,
			Vector3 colorInput,
			real halfSizeInput,
			real offset
		);

		~Plane()
		{
			delete plane;
		}

		// Display this plane
		void display();
	};

	class Box : public Renderable
	{
	public:
		CollisionBox *boxPrimitive;

		Box
		(
			RenderingDemo *demo,
			Vector3 position,
			Vector3 velocity,
			Vector3 acceleration,
			real mass,
			Vector3 halfSize
		);

		~Box()
		{
			delete boxPrimitive;
		}

		// Display this rectangle
		void display();
	};

	class Sphere : public Renderable
	{
	public:
		CollisionSphere *spherePrimitive;

		Sphere
		(
			RenderingDemo *demo,
			Vector3 position,
			Vector3 velocity,
			Vector3 acceleration,
			real mass,
			real radius
		);

		~Sphere()
		{
			delete spherePrimitive;
		}


		// Display this sphere object
		void display();
	};


	class Capsule : public Renderable
	{
	public:
		PhysicsEngine::CollisionCapsule *capsulePrimitive;

		Capsule
		(
			RenderingDemo *demo,
			Vector3 position,
			Quaternion rotation,
			Vector3 velocity,
			Vector3 acceleration,
			real mass,
			real radius,
			real height
		);

		~Capsule();

		// Display this sphere object
		void display();

	private:
		GLUquadric *quadricObject;
	};
}


#endif // RENDERABLE_OBJECTS_H