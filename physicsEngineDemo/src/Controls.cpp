#include "Controls.h"
#include <iostream>
#include "ApplicationSettings.h"
#include "MathDataTypes.h"
#include <math.h>

using namespace PhysicsEngine;

// Add force to the first cube
void addForceToCube(std::vector<RigidBody *> *rigidBodies);
// Add a cube rigid body to the scene
void addRigidCube(std::vector<RigidBody *> *rigidBodies, float theta, float phi);
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
		addRigidCube(rigidBodies, theta, phi);
		break;
	case '7':
		addForceToCube(rigidBodies);
		break;
	}
}

// Add force to the first cube
void addForceToCube(std::vector<RigidBody *> *rigidBodies)
{
	(*rigidBodies)[0]->addForceAtBodyPoint(Vector3(10.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f));
}

// Add a cube rigid body to the scene
void addRigidCube(std::vector<RigidBody *> *rigidBodies, float theta, float phi)
{
	Square *newSquare = new Square();
	//// Rotate the camera based on mouse movements
	//glRotatef(-phi, 1, 0, 0);
	//glRotatef(theta, 0, 1, 0);
	Vector3 oldSquareCreationPosition(0.0f, 4.0f, 6.0f);
	float thetaRads = theta * PI / 180.0;
	Quaternion cameraRotation(cos(thetaRads / 2), 0.0f, 1.0f * sin(thetaRads / 2), 0.0f);
	Matrix3 rotMatrix;
	rotMatrix.setOrientation(cameraRotation);
	Vector3 squareCreationPosition = rotMatrix.transform(oldSquareCreationPosition);
	newSquare->setPosition(squareCreationPosition);
	newSquare->setVelocity(Vector3(0.0f, 0.0f, 0.0f));
	newSquare->setMass(1.0f);
	Matrix3 tensor;
	// Vector3 -> half the length, width, and height of the box, real-> mass
	tensor.setBlockInertiaTensor(Vector3(.5f, .5f, .5f), 1.0f);
	newSquare->setInertiaTensor(tensor);

	rigidBodies->push_back(newSquare);
}