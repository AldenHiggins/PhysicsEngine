#include "Controls.h"
#include "ApplicationSettings.h"
#include "MathDataTypes.h"

using namespace PhysicsEngine;

// Add force to the first cube
void addForceToCube(std::vector<RigidBody *> *rigidBodies);
// Add a cube rigid body to the scene
void addRigidCube(std::vector<RigidBody *> *rigidBodies);
void rigidBodyKeyCheck(unsigned char key, std::vector<RigidBody *> *rigidBodies);
void particleKeyCheck(unsigned char key, std::vector<Particle *> *particles);

void PhysicsEngine::keyCheck(unsigned char key, std::vector<Particle *> *particles, std::vector<RigidBody *> *rigidBodies)
{
	particleKeyCheck(key, particles);
	rigidBodyKeyCheck(key, rigidBodies);
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

void rigidBodyKeyCheck(unsigned char key, std::vector<RigidBody *> *rigidBodies)
{
	switch (key)
	{
	case '6':
		addRigidCube(rigidBodies);
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
void addRigidCube(std::vector<RigidBody *> *rigidBodies)
{
	Square *newSquare = new Square();
	newSquare->setPosition(Vector3(0.0f, 4.0f, 6.0f));
	newSquare->setVelocity(Vector3(0.0f, 0.1f, 0.0f));
	newSquare->setMass(1.0f);
	Matrix3 tensor;
	// Vector3 -> half the length, width, and height of the box, real-> mass
	tensor.setBlockInertiaTensor(Vector3(.5f, .5f, .5f), 1.0f);
	newSquare->setInertiaTensor(tensor);

	rigidBodies->push_back(newSquare);
}