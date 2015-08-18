#include "Controls.h"
#include <iostream>
#include "ApplicationSettings.h"
#include "MathDataTypes.h"
#include <math.h>

using namespace PhysicsEngine;

// Add force to the first cube
void addForceToCube(std::vector<RigidBody *> *rigidBodies);
// Add a cube rigid body to the scene
void addRigidCubeWhereYouLook(std::vector<RigidBody *> *rigidBodies, float theta, float phi);
// Add a rigid cube with the inputted parameters
void addRigidCube(std::vector<RigidBody *> *rigidBodies, Vector3 position, Vector3 velocity, real mass, real halfSize);
void rigidBodyKeyCheck(unsigned char key, std::vector<RigidBody *> *rigidBodies, float theta, float phi);
void particleKeyCheck(unsigned char key, std::vector<Particle *> *particles);

void PhysicsEngine::keyCheck(unsigned char key, std::vector<Particle *> *particles, std::vector<RigidBody *> *rigidBodies, float theta, float phi)
{
	particleKeyCheck(key, particles);
	rigidBodyKeyCheck(key, rigidBodies, theta, phi);
}

void particleKeyCheck(unsigned char key, std::vector<Particle *> *particles)
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
		particles->push_back(CreateParticle::createFireWorkParticle(.3f, 0.1f, Vector3(0.0f, 4.0f, 6.0f), Vector3(.75f, 0.23f, 0.68f)));
		break;
	}
}

void rigidBodyKeyCheck(unsigned char key, std::vector<RigidBody *> *rigidBodies, float theta, float phi)
{
	switch (key)
	{
	case '6':
		addRigidCubeWhereYouLook(rigidBodies, theta, phi);
		break;
	case '7':
		//addForceToCube(rigidBodies);
		addRigidCube(rigidBodies, Vector3(8.0f, 3.0f, 6.0f), Vector3(0.0f, 0.0f, 0.0f), 10.0f, .5f);
		addRigidCube(rigidBodies, Vector3(8.0f, 3.0f, 6.0f), Vector3(0.0f, 0.0f, 0.0f), 10.0f, .5f);
		addRigidCube(rigidBodies, Vector3(8.0f, 3.0f, 6.0f), Vector3(0.0f, 0.0f, 0.0f), 10.0f, .5f);
		addRigidCube(rigidBodies, Vector3(8.0f, 3.0f, 6.0f), Vector3(0.0f, 0.0f, 0.0f), 10.0f, .5f);
		addRigidCube(rigidBodies, Vector3(8.0f, 3.0f, 6.0f), Vector3(0.0f, 0.0f, 0.0f), 10.0f, .5f);
		addRigidCube(rigidBodies, Vector3(8.0f, 3.0f, 6.0f), Vector3(0.0f, 0.0f, 0.0f), 10.0f, .5f);
		addRigidCube(rigidBodies, Vector3(8.0f, 3.0f, 6.0f), Vector3(0.0f, 0.0f, 0.0f), 10.0f, .5f);
		addRigidCube(rigidBodies, Vector3(8.0f, 3.0f, 6.0f), Vector3(0.0f, 0.0f, 0.0f), 10.0f, .5f);
		break;
	case '8':
		addRigidCube(rigidBodies, Vector3(0.0f, 2.4f, 6.0f), Vector3(1.0f, 0.0f, 0.0f), 10.0f, .5f);
		addRigidCube(rigidBodies, Vector3(4.0f, 2.0f, 6.0f), Vector3(0.0f, 0.0f, 0.0f), 10.0f, .5f);
		addRigidCube(rigidBodies, Vector3(0.0f, 3.4f, 6.0f), Vector3(1.0f, 0.0f, 0.0f), 10.0f, .5f);
		addRigidCube(rigidBodies, Vector3(4.0f, 3.0f, 6.0f), Vector3(0.0f, 0.0f, 0.0f), 10.0f, .5f);
		break;
	case '9':
		addRigidCube(rigidBodies, Vector3(0.0f, 2.4f, 6.0f), Vector3(10.0f, 0.0f, 0.0f), 10.0f, .5f);
		break;
	case '0':
		addRigidCube(rigidBodies, Vector3(0.0f, 2.4f, 6.0f), Vector3(60.0f, 0.0f, 0.0f), 10.0f, .5f);
		break;
	}
}

// Add force to the first cube
void addForceToCube(std::vector<RigidBody *> *rigidBodies)
{
	(*rigidBodies)[0]->addForceAtBodyPoint(Vector3(10.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f));
}

// Add a rigid cube with the inputted parameters
void addRigidCube(std::vector<RigidBody *> *rigidBodies, Vector3 position, Vector3 velocity, real mass, real halfSize)
{
	Square *newSquare = new Square();
	newSquare->setPosition(position);
	newSquare->setVelocity(velocity);
	newSquare->setMass(mass);
	Matrix3 tensor;
	// Vector3 -> half the length, width, and height of the box, real-> mass
	tensor.setBlockInertiaTensor(Vector3(halfSize, halfSize, halfSize), mass);
	newSquare->setInertiaTensor(tensor);

	rigidBodies->push_back(newSquare);
}

// Add a cube rigid body to the scene
void addRigidCubeWhereYouLook(std::vector<RigidBody *> *rigidBodies, float theta, float phi)
{
	Vector3 oldSquareCreationPosition(0.0f, 4.0f, 6.0f);
	float thetaRads = theta * PI / 180.0;
	Quaternion cameraRotation(cos(thetaRads / 2), 0.0f, 1.0f * sin(thetaRads / 2), 0.0f);
	Matrix3 rotMatrix;
	rotMatrix.setOrientation(cameraRotation);
	Vector3 squareCreationPosition = rotMatrix.transform(oldSquareCreationPosition);

	addRigidCube(rigidBodies, squareCreationPosition, Vector3(0.0f, 0.0f, 0.0f), 1.0f, 0.5f);
}