#include "Controls.h"
#include <iostream>
#include "ApplicationSettings.h"
#include "MathDataTypes.h"
#include <math.h>

using namespace PhysicsDemo;

void Controls::keyCheck
(
	unsigned char key,
	RenderingDemo *demo
)
{
	switch (key)
	{
		case '1':
		{		
			//addCapsule
			//(
			//	demo,
			//	rotatePositionAlongYAxis(13.0f, 5.7f, demo->player.getYaw()), Quaternion::fromEuler(1, 0, 0), Vector3(0.0f, 0, 0), Vector3(),
			//	10.0f, 1.0f, 1.0f
			//);

			//addCapsule
			//(
			//	demo,
			//	Vector3(-5.0f, 5.0f, 1.0f), Quaternion::fromEuler(1, 0, 0), Vector3(0.0f, 0, 0), Vector3(),
			//	1.0f, 1.0f, 1.0f
			//);

			//addForceToCapsule(demo, (demo->capsuleObjects.size() - 1), Vector3(300.0f, 0.0f, 0.0f), Vector3());
			//void Controls::addSphere
			//	(
			//		RenderingDemo *demo,
			//		Vector3 position,
			//		Vector3 velocity,
			//		Vector3 acceleration,
			//		real mass,
			//		real radius
			//		)

			addSphere
			(
				demo,
				Vector3(0.0f, 5.0f, 1.0f), Vector3(), Vector3(),
				1.0f, 1.0f
			);

			addForceToSphere(demo, demo->sphereObjects.size() - 1, Vector3(300.0f, 0.0f, 0.0f), Vector3());

			addCube
			(
				demo,
				Vector3(10.0f, 5.0f, 0.0f), Vector3(), Vector3(),
				10.0f, Vector3(0.5f, 5.0f, 10.0f)
			);

			break;
		}
		case '2':
		{
			addCube
			(
				demo,
				Vector3(-20.0f, 5.0f, 0.0f), Vector3(), Vector3(),
				10.0f, Vector3(0.5f, 5.0f, 10.0f)
			);
			addCube
			(
				demo,
				Vector3(20.0f, 5.0f, 0.0f), Vector3(), Vector3(),
				10.0f, Vector3(0.5f, 5.0f, 10.0f)
			);
			addCube
			(
				demo,
				Vector3(0.0f, 5.0f, 20.0f), Vector3(), Vector3(),
				10.0f, Vector3(10.0f, 5.0f, 0.5f)
			);
			addCube
			(
				demo,
				Vector3(0.0f, 5.0f, -20.0f), Vector3(), Vector3(),
				10.0f, Vector3(10.0f, 5.0f, 0.5f)
			);

			//addCube
			//(
			//	demo,
			//	Vector3(0.0f, 5.0f, 12.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(),
			//	10.0f, Vector3(1.0f, .5f, .5f)
			//);
			//addCube
			//(
			//	demo,
			//	Vector3(3.0f, 7.0f, 12.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(),
			//	10.0f, Vector3(1.0f, .5f, .5f)
			//);
			//addCube
			//(
			//	demo,
			//	Vector3(6.0f, 12.0f, 12.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(),
			//	10.0f, Vector3(1.0f, .5f, .5f)
			//);
			//addCube
			//(
			//	demo,
			//	Vector3(9.0f, 15.0f, 12.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(),
			//	10.0f, Vector3(1.0f, .5f, .5f)
			//);

			//addParticle(physicsEngine, particles, Vector3(0.0f, 4.0f, 6.0f), 1.0f, 1.0f, 1.0f);
			break;
		}
	}
}

/////////////////////////////////////////////////////////////////////////
//////////////////////// ADD OBJECTS TO RENDER //////////////////////////
/////////////////////////////////////////////////////////////////////////
void Controls::addParticle
(
	RenderingDemo *demo,
	Vector3 position,
	real mass,
	real radius,
	real height
)
{
	RenderableParticle *particle = CreateParticle::createParticle(demo, position, 5.0f, radius, Vector3(.5f, .5f, .5f));
}

void Controls::addSphere
(
	RenderingDemo *demo,
	Vector3 position,
	Vector3 velocity,
	Vector3 acceleration,
	real mass,
	real radius
)
{
	Sphere *newSphere = new Sphere(demo, position, velocity, acceleration, mass, radius);
}

void Controls::addCube
(
	RenderingDemo *demo,
	Vector3 position,
	Vector3 velocity,
	Vector3 acceleration,
	real mass,
	Vector3 halfSize
)
{
	Box *newSquare = new Box(demo, position, velocity, acceleration, mass, halfSize);
}

void Controls::addCapsule
(
	RenderingDemo *demo,
	Vector3 position,
	Quaternion rotation,
	Vector3 velocity,
	Vector3 acceleration,
	real mass,
	real radius,
	real height
)
{
	Capsule *newCapsule = new Capsule(demo, position, rotation, velocity, acceleration, mass, radius, height);
}

/////////////////////////////////////////////////////////////////////////
////////////////////////  OBJECT INTERACTIONS  //////////////////////////
/////////////////////////////////////////////////////////////////////////
// Add force to the cube at cubeindex
void Controls::addForceToCube(RenderingDemo *demo, int cubeIndex, Vector3 force, Vector3 position)
{
	(demo->rectangleObjects)[0]->boxPrimitive->body->addForceAtBodyPoint(force, position);
}

// Add force to the capsule at capsuleIndex
void Controls::addForceToCapsule(RenderingDemo *demo, int capsuleIndex, Vector3 force, Vector3 position)
{
	(demo->capsuleObjects)[capsuleIndex]->capsulePrimitive->body->addForceAtBodyPoint(force, position);
}

// Add force to the sphere at sphereIndex
void Controls::addForceToSphere(RenderingDemo *demo, int sphereIndex, Vector3 force, Vector3 position)
{
	(demo->sphereObjects)[sphereIndex]->spherePrimitive->body->addForceAtBodyPoint(force, position);
}

/////////////////////////////////////////////////////////////////////////
////////////////////////    HELPER FUNCTIONS   //////////////////////////
/////////////////////////////////////////////////////////////////////////
Vector3 Controls::rotatePositionAlongYAxis(real depth, real height, real theta)
{
	Vector3 objectInitialPosition(0.0f, height, depth);
	real thetaRads = theta * PI / 180.0;
	Quaternion cameraRotation(cos(thetaRads / 2), 0.0f, 1.0f * sin(thetaRads / 2), 0.0f);
	Matrix3 rotMatrix;
	rotMatrix.setOrientation(cameraRotation);

	return rotMatrix.transform(objectInitialPosition);
}