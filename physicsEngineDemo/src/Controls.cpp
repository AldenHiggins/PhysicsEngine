#include "Controls.h"
#include <iostream>
#include "ApplicationSettings.h"
#include "MathDataTypes.h"
#include <math.h>

using namespace PhysicsEngine;

void addRigidCubeNoGravity(std::vector<RectangleObject *> *rigidBodies, Vector3 position, Vector3 velocity, real mass, Vector3 halfSize);

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
		addRigidCapsule(capsules, Vector3(-2.0f, 5.0f, 12.0f), Quaternion::fromEuler(90, 0, 0), Vector3(0.0f, 0, 0), 1.0f, 1.0f, 1.0f);
		addRigidCube(rectangularBodies, Vector3(0.0f, 5.0f, 12.0f), Vector3(0.0f, 0.0f, 0.0f), 10.0f, Vector3(1.0f, .5f, .5f));
		break;
	}
	// Create a different capsule
	case '2':
		addForceToCapsule(capsules);
		//addRigidCapsule(capsuleBodies, Vector3(0.0f, 3.0f, 12.0f), Vector3(0.0f, 0.1f, 0.0f), 1.0f, 3.0f, 1.0f);
		break;

		// Create a different capsule
	case '3':
		(*capsules)[0]->body->addForceAtBodyPoint(Vector3(-100.0f, 0.0f, 0.0f), Vector3(0.5f, 1.5f, 1.0f));
		//addRigidCapsule(capsuleBodies, Vector3(0.0f, 3.0f, 12.0f), Vector3(0.0f, 0.1f, 0.0f), 4.0f, 4.0f, 1.0f);
		break;

		// Create a different capsule
	case '4':
		//addRigidCapsule(capsules, Vector3(0.0f, 3.0f, 12.0f), Vector3(0.0f, 0.1f, 0.0f), 1.0f, 0.2f, 4.0f);
		break;
	}

	//particleKeyCheck(key, particles);
	//sphereKeyCheck(key, spheres, theta, phi);
	//rectangleKeyCheck(key, rectangularBodies, theta, phi);
	//capsuleKeyCheck(key, capsules, theta, phi);
}

void Controls::capsuleKeyCheck(unsigned char key, std::vector<CapsuleObject *> *capsuleBodies, float theta, float phi)
{

	std::cout << "This is happening!!!" << std::endl;
	// Note we omit passing on the x and y: they are rarely needed.
	switch (key)
	{
	// Create a capsule
	case '1':
		//addRigidCapsule(capsuleBodies, Vector3(0.0f, 5.0f, 12.0f), Vector3(0.0f, 0.1f, 0.0f), 1.0f, 1.0f, 1.0f);
		break;

	// Create a different capsule
	case '2':
		addForceToCapsule(capsuleBodies);
		//addRigidCapsule(capsuleBodies, Vector3(0.0f, 3.0f, 12.0f), Vector3(0.0f, 0.1f, 0.0f), 1.0f, 3.0f, 1.0f);
		break;

	// Create a different capsule
	case '3':
		(*capsuleBodies)[0]->body->addForceAtBodyPoint(Vector3(-100.0f, 0.0f, 0.0f), Vector3(0.5f, 1.5f, 1.0f));
		//addRigidCapsule(capsuleBodies, Vector3(0.0f, 3.0f, 12.0f), Vector3(0.0f, 0.1f, 0.0f), 4.0f, 4.0f, 1.0f);
		break;

	// Create a different capsule
	case '4':
		break;
	}
}

void Controls::particleKeyCheck(unsigned char key, std::vector<Particle *> *particles)
{
	if (particles->size() >= MAX_PARTICLE_COUNT)
	{
		return;
	}

	// Note we omit passing on the x and y: they are rarely needed.
	switch (key)
	{
		// Create a slow particle
	case '1':
		particles->push_back(CreateParticle::createParticle(.3f, .1f, Vector3(.5f, 0.0f, 1.0f)));
		break;
		// Create a fast particle
	case '2':
		particles->push_back(CreateParticle::createParticle(.8f, .1f, Vector3(0.0f, 0.5f, 0.5f)));
		break;
		// Create a big particle
	case '3':
		particles->push_back(CreateParticle::createParticle(.3f, .3f, Vector3(.75f, 0.23f, 0.68f)));
		break;
		// Create a sphere particle
	case '4':
		particles->push_back(CreateParticle::createCircularParticle(.3f, 0.1f, Vector3(.75f, 0.23f, 0.68f)));
		break;
		// Create a sphere particle
	case '5':
		particles->push_back(CreateParticle::createFireWorkParticle(.6f, 0.1f, Vector3(0.0f, 4.0f, 6.0f), Vector3(.75f, 0.23f, 0.68f), true, 3));
		break;
	}
}

void Controls::rectangleKeyCheck
(
	unsigned char key,
	std::vector<RectangleObject *> *rectangularBodies,
	float theta,
	float phi
)
{
	switch (key)
	{
	case '6':
		addRigidCubeWhereYouLook(rectangularBodies, theta, phi);
		break;
	case '7':
		addRigidCube(rectangularBodies, Vector3(0.0f, 3.0f, 6.0f), Vector3(0.0f, 0.0f, 0.0f), 10.0f, Vector3(1.0f, .5f, .5f));
		break;
	case '8':
		// Create a cool scene to demonstrate cubic collisions
		addRigidCubeNoGravity(rectangularBodies, Vector3(0.0f, 6.0f, 6.0f), Vector3(0.0f, 0.0f, 0.0f), 1.0f, Vector3(0.5f, 0.5f, 0.5f));
		addForceToCube(rectangularBodies);
		//addRigidCubeNoGravity(rectangularBodies, Vector3(3.0f, 6.0f, 6.0f), Vector3(0.0f, 0.0f, 0.0f), 1.0f, Vector3(0.5f, 0.5f, 0.5f));
		break;
	case '9':
		addRigidCubeNoGravity(rectangularBodies, Vector3(-5.0f, 5.0f, 6.0f), Vector3(15.0f, 0.0f, 0.0f), 10.0f, Vector3(1.5f, 1.2f, 0.5f));
		break;
	case '0':
		for (unsigned int rigidBodyIndex = 0; rigidBodyIndex < rectangularBodies->size(); rigidBodyIndex++)
		{
			(*rectangularBodies)[rigidBodyIndex]->body->setIsAwake(false);
		}
		break;
	}
}

void Controls::sphereKeyCheck
(
	unsigned char key,
	std::vector<SphereObject *> *sphereBodies,
	float theta,
	float phi
)
{
	// Note we omit passing on the x and y: they are rarely needed.
	switch (key)
	{
	// Create a sphere where you look
	case '1':
		for (int i = 0; i < 30; i++)
		{
			addSphereWhereYouLook(sphereBodies, theta + i, phi, 0.1f);
		}
		break;
	// Create a cool collision scene with lots of spheres
	case '2':
		// Top
		addSphere(sphereBodies,Vector3(0.0f, 6.0f, 6.0f), Vector3(0.0f, 0.0f, 0.0f), 1.0f, 0.3f);
		addSphere(sphereBodies, Vector3(1.0f, 6.0f, 6.0f), Vector3(0.0f, 0.0f, 0.0f), 1.0f, 0.3f);
		addSphere(sphereBodies, Vector3(-1.0f, 6.0f, 6.0f), Vector3(0.0f, 0.0f, 0.0f), 1.0f, 0.3f);

		// Come up from below
		addSphere(sphereBodies, Vector3(0.0f, 3.0f, 6.1f), Vector3(0.0f, 10.0f, 0.0f), 1.0f, 0.3f);
		addSphere(sphereBodies, Vector3(1.1f, 3.0f, 6.3f), Vector3(0.0f, 10.0f, 0.0f), 1.0f, 0.3f);
		addSphere(sphereBodies, Vector3(-1.1f, 3.0f, 5.9f), Vector3(0.0f, 10.0f, 0.0f), 1.0f, 0.3f);

		// Come up and in from outside
		addSphere(sphereBodies, Vector3(0.0f, 3.0f, 8.1f), Vector3(0.0f, 10.0f, -7.0f), 1.0f, 0.3f);
		addSphere(sphereBodies, Vector3(1.1f, 3.0f, 8.3f), Vector3(0.0f, 10.0f, -7.0f), 1.0f, 0.3f);
		addSphere(sphereBodies, Vector3(-1.1f, 3.0f, 8.9f), Vector3(0.0f, 10.0f, -7.0f), 1.0f, 0.3f);

		// Fast sphere coming in from the right
		addSphere(sphereBodies, Vector3(5.1f, 4.0f, 6.0f), Vector3(-15.0f, 6.0f, 0.0f), 1.0f, 0.3f);
		break;
	// Create a big particle
	case '3':
		break;
	// Create a sphere particle
	case '4':
		break;
	// Create a sphere particle
	case '5':
		break;
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
void addRigidCubeNoGravity(std::vector<RectangleObject *> *rigidBodies, Vector3 position, Vector3 velocity, real mass, Vector3 halfSize)
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