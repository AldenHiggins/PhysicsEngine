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
		// Add a sphere
		case '1':
		{		
			addSphere
			(
				demo,
				rotatePositionAlongYAxis(5.0f, 5.0f, demo->player.getYaw()), Vector3(), Vector3(),
				1.0f, 1.0f
			);

			addForceToSphere(demo, demo->sphereObjects.size() - 1, Vector3(300.0f, 0.0f, 0.0f), Vector3());

			break;
		}

		// Add a cube
		case '2':
		{
			addCube
			(
				demo,
				rotatePositionAlongYAxis(5.0f, 5.0f, demo->player.getYaw()), Vector3(), Vector3(),
				1.0f, Vector3(1.0f, 1.0f, 1.0f),
				false
			);

			addForceToCube(demo, demo->rectangleObjects.size() - 1, Vector3(300.0f, 0.0f, 0.0f), Vector3(0.0f, 0.2f, 0.5f));

			break;
		}

		// Add a capsule
		case '3':
		{
			addCapsule
			(
				demo,
				rotatePositionAlongYAxis(5.0f, 5.0f, demo->player.getYaw()), Quaternion::fromEuler(1, 0, 0), Vector3(0.0f, 0, 0), Vector3(),
				1.0f, 1.0f, 3.0f
			);

			addForceToCapsule(demo, (demo->capsuleObjects.size() - 1), Vector3(300.0f, 0.0f, 0.0f), Vector3(0.0f, 0.6f, 0.3f));
			break;
		}

		// Fire a sphere bullet from where the player is looking
		case '4':
		{
			Vector3 bulletStart = demo->player.getPosition() + demo->player.getForward();
			addSphere
			(
				demo,
				bulletStart, Vector3(0.0f, 0, 0), Vector3(),
				1.0f, 0.1f
			);

			addForceToSphere(demo, (demo->sphereObjects.size() - 1), (demo->player.getForward() * 1000.0f), Vector3());
			break;
		}

		// Fire box bullets
		case '5':
		{
			Vector3 bulletStart = demo->player.getPosition() + demo->player.getForward();
			addCube
			(
				demo,
				bulletStart, Vector3(0.0f, 0, 0), Vector3(),
				1.0f, Vector3(0.1f, 0.1f, 0.1f),
				false
			);

			addForceToCube(demo, (demo->rectangleObjects.size() - 1), (demo->player.getForward() * 1000.0f), Vector3());
			break;
		}

		// Create the level out of really heavy cubes
		case '0':
		{
			bool staticObjects = false;
			real cubeMass = 1000.0f;

			addCube
			(
				demo,
				Vector3(-20.0f, 5.0f, 0.0f), Vector3(), Vector3(),
				cubeMass, Vector3(0.5f, 5.0f, 20.0f),
				staticObjects
			);
			addCube
			(
				demo,
				Vector3(20.0f, 5.0f, 0.0f), Vector3(), Vector3(),
				cubeMass, Vector3(0.5f, 5.0f, 20.0f),
				staticObjects
			);
			addCube
			(
				demo,
				Vector3(0.0f, 5.0f, 20.0f), Vector3(), Vector3(),
				cubeMass, Vector3(20.0f, 5.0f, 0.5f),
				staticObjects
			);
			addCube
			(
				demo,
				Vector3(0.0f, 5.0f, -20.0f), Vector3(), Vector3(),
				cubeMass, Vector3(20.0f, 5.0f, 0.5f),
				staticObjects
			);

			addCube
			(
				demo,
				Vector3(0.0f, 0.0f, 0.0f), Vector3(), Vector3(),
				cubeMass, Vector3(20.0f, 1.0f, 20.0f),
				staticObjects
			);
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
	Vector3 halfSize,
	bool isStatic
)
{
	Box *newSquare = new Box(demo, position, velocity, acceleration, mass, halfSize, isStatic);
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
	(demo->rectangleObjects)[cubeIndex]->boxPrimitive->body->addForceAtBodyPoint(force, position);
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