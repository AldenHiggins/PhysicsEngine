#include "Controls.h"
#include <iostream>
#include "ApplicationSettings.h"
#include "MathDataTypes.h"
#include <math.h>

using namespace PhysicsEngine;

void Controls::keyCheck
(
	unsigned char key,
	std::vector<Particle *> *particles,
	std::vector<RectangleObject *> *rectangularBodies,
	std::vector<SphereObject *> *spheres,
	std::vector<CapsuleObject *> *capsules,
	float theta,
	float phi
)
{
	switch (key)
	{
		// Create a capsule
		case '1':
		{		
			addRigidCapsule(capsules, Vector3(-3.0f, 5.0f, 12.0f), Quaternion::fromEuler(90, 0, 0), Vector3(0.0f, 0, 0), 1.0f, 1.0f, 1.0f);
			addRigidCubeNoGravity(rectangularBodies, Vector3(0.0f, 5.0f, 12.0f), Vector3(0.0f, 0.0f, 0.0f), 10.0f, Vector3(1.0f, .5f, .5f));
			break;
		}
		// Create a different capsule
		case '2':
		{
			addForceToCapsule(capsules);
			//addRigidCapsule(capsuleBodies, Vector3(0.0f, 3.0f, 12.0f), Vector3(0.0f, 0.1f, 0.0f), 1.0f, 3.0f, 1.0f);
			break;
		}
		// Create a different capsule
		case '3':
		{
			(*capsules)[0]->body->addForceAtBodyPoint(Vector3(-100.0f, 0.0f, 0.0f), Vector3(0.5f, 1.5f, 1.0f));
			//addRigidCapsule(capsuleBodies, Vector3(0.0f, 3.0f, 12.0f), Vector3(0.0f, 0.1f, 0.0f), 4.0f, 4.0f, 1.0f);
			break;
		}
		// Create a different capsule
		case '4':
		{
			//addRigidCapsule(capsules, Vector3(0.0f, 3.0f, 12.0f), Vector3(0.0f, 0.1f, 0.0f), 1.0f, 0.2f, 4.0f);
			break;
		}
	}
}

// Add a sphere to the scene where you look
void Controls::addSphereWhereYouLook
(
	std::vector<SphereObject *> *sphereBodies,
	float theta,
	float phi,
	float radius
)
{
	Vector3 sphereCreationPosition = rotatePositionAlongYAxis(6.0f, 9.0f, theta);
	addSphere(sphereBodies, sphereCreationPosition, Vector3(0.0f, 0.0f, 0.0f), 1.0f, radius);
}

// Add a sphere to the scene with the given properties
void Controls::addSphere
(
	std::vector<SphereObject *> *sphereBodies,
	Vector3 position,
	Vector3 velocity,
	real mass,
	real radius
)
{
	SphereObject *newSphere = new SphereObject();
	newSphere->setState(position, velocity, Vector3::GRAVITY, mass, radius);
	sphereBodies->push_back(newSphere);
}

// Add force to the first cube
void Controls::addForceToCube(std::vector<RectangleObject *> *rigidBodies)
{
	(*rigidBodies)[0]->body->addForceAtBodyPoint(Vector3(80.0f, 0.0f, 0.0f), Vector3(0.5f, 0.5f, 1.0f));
}

// Add force to the first capsule
void Controls::addForceToCapsule(std::vector<CapsuleObject *> *capsuleBodies)
{
	(*capsuleBodies)[0]->body->addForceAtBodyPoint(Vector3(400.0f, 0.0f, 0.0f), Vector3(0.5f, 1.5f, 1.0f));
	(*capsuleBodies)[0]->body->addForceAtBodyPoint(Vector3(-400.0f, 0.0f, 0.0f), Vector3(0.5f, -1.5f, 1.0f));
}

// Add a rigid cube with the inputted parameters
void Controls::addRigidCubeNoGravity(std::vector<RectangleObject *> *rigidBodies, Vector3 position, Vector3 velocity, real mass, Vector3 halfSize)
{
	RectangleObject *newSquare = new RectangleObject();
	newSquare->setState(position, velocity, Vector3(0.0f, 0.0f, 0.0f), mass, halfSize);
	rigidBodies->push_back(newSquare);
}

// Add a rigid cube with the inputted parameters
void Controls::addRigidCube(std::vector<RectangleObject *> *rigidBodies, Vector3 position, Vector3 velocity, real mass, Vector3 halfSize)
{
	RectangleObject *newSquare = new RectangleObject();
	newSquare->setState(position, velocity, Vector3::GRAVITY, mass, halfSize);
	rigidBodies->push_back(newSquare);
}

// Add a cube rigid body to the scene
void Controls::addRigidCubeWhereYouLook(std::vector<RectangleObject *> *rigidBodies, float theta, float phi)
{
	Vector3 squareCreationPosition = rotatePositionAlongYAxis(6.0f, 4.0f, theta);

	addRigidCube(rigidBodies, squareCreationPosition, Vector3(0.0f, 0.0f, 0.0f), 1.0f, Vector3(.5f, .5f, .5f));
}

Vector3 Controls::rotatePositionAlongYAxis(real depth, real height, real theta)
{
	Vector3 objectInitialPosition(0.0f, height, depth);
	float thetaRads = theta * PI / 180.0;
	Quaternion cameraRotation(cos(thetaRads / 2), 0.0f, 1.0f * sin(thetaRads / 2), 0.0f);
	Matrix3 rotMatrix;
	rotMatrix.setOrientation(cameraRotation);

	return rotMatrix.transform(objectInitialPosition);
}

// Add a capsule with the inputted parameters
void Controls::addRigidCapsule(std::vector<CapsuleObject *> *capsuleBodies, Vector3 position, Quaternion rotation, Vector3 velocity, real mass, real radius, real height)
{
	CapsuleObject *newCapsule = new CapsuleObject();
	newCapsule->setState(position, rotation, velocity, Vector3(), mass, radius, height);
	capsuleBodies->push_back(newCapsule);
}