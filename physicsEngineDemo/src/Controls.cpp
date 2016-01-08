#include "Controls.h"
#include <iostream>
#include "ApplicationSettings.h"
#include "MathDataTypes.h"
#include <math.h>

using namespace PhysicsDemo;

void Controls::keyCheck
(
	unsigned char key,
	PhysicsEngine::Physics *physicsEngine,
	std::vector<Particle *> *particles,
	std::vector<Box *> *rectangularBodies,
	std::vector<Sphere *> *spheres,
	std::vector<Capsule *> *capsules,
	float theta,
	float phi
)
{
	switch (key)
	{
		case '1':
		{		
			addRigidCapsule(physicsEngine, capsules, PhysicsEngine::Vector3(-3.0f, 5.0f, 12.0f), PhysicsEngine::Quaternion::fromEuler(1, 0, 0), PhysicsEngine::Vector3(10.0f, 0, 0), 10.0f, 1.0f, 1.0f);
			addRigidCubeNoGravity(physicsEngine, rectangularBodies, PhysicsEngine::Vector3(0.0f, 5.0f, 12.0f), PhysicsEngine::Vector3(0.0f, 0.0f, 0.0f), 10.0f, PhysicsEngine::Vector3(1.0f, .5f, .5f));
			addSphere(physicsEngine, spheres, PhysicsEngine::Vector3(3.0f, 5.0f, 12.0f), PhysicsEngine::Vector3(), PhysicsEngine::Vector3(), 1.0f, 3.0f);
			break;
		}
		case '2':
		{
			addForceToCapsule(capsules);
			break;
		}
		case '3':
		{
			(*capsules)[0]->capsulePrimitive->body->addForceAtBodyPoint(PhysicsEngine::Vector3(-100.0f, 0.0f, 0.0f), PhysicsEngine::Vector3(0.5f, 1.5f, 1.0f));
			break;
		}
		case '4':
		{
			break;
		}
	}
}

// Add a sphere to the scene where you look
void Controls::addSphereWhereYouLook
(
	std::vector<Sphere *> *sphereBodies,
	float theta,
	float phi,
	float radius
)
{
	PhysicsEngine::Vector3 sphereCreationPosition = rotatePositionAlongYAxis(6.0f, 9.0f, theta);
	//addSphere(sphereBodies, sphereCreationPosition, PhysicsEngine::Vector3(0.0f, 0.0f, 0.0f), 1.0f, radius);
}

// Add a sphere to the scene with the given properties
void Controls::addSphere
(
	PhysicsEngine::Physics *physicsEngine,
	std::vector<Sphere *> *sphereBodies,
	PhysicsEngine::Vector3 position,
	PhysicsEngine::Vector3 velocity,
	PhysicsEngine::Vector3 acceleration,
	real mass,
	real radius
)
{
	Sphere *newSphere = new Sphere();
	newSphere->spherePrimitive = new PhysicsEngine::CollisionSphere();
	physicsEngine->createSphere(newSphere->spherePrimitive);
	newSphere->spherePrimitive->setState(position, velocity, acceleration, mass, radius);
	sphereBodies->push_back(newSphere);
}

// Add force to the first cube
void Controls::addForceToCube(std::vector<Box *> *rigidBodies)
{
	(*rigidBodies)[0]->boxPrimitive->body->addForceAtBodyPoint(PhysicsEngine::Vector3(80.0f, 0.0f, 0.0f), PhysicsEngine::Vector3(0.5f, 0.5f, 1.0f));
}

// Add force to the first capsule
void Controls::addForceToCapsule(std::vector<Capsule *> *capsuleBodies)
{
	(*capsuleBodies)[0]->capsulePrimitive->body->addForceAtBodyPoint(PhysicsEngine::Vector3(400.0f, 0.0f, 0.0f), PhysicsEngine::Vector3(0.5f, 1.5f, 1.0f));
	(*capsuleBodies)[0]->capsulePrimitive->body->addForceAtBodyPoint(PhysicsEngine::Vector3(-400.0f, 0.0f, 0.0f), PhysicsEngine::Vector3(0.5f, -1.5f, 1.0f));
}

// Add a rigid cube with the inputted parameters
void Controls::addRigidCubeNoGravity(PhysicsEngine::Physics *physicsEngine, std::vector<Box *> *rigidBodies, PhysicsEngine::Vector3 position, PhysicsEngine::Vector3 velocity, real mass, PhysicsEngine::Vector3 halfSize)
{
	Box *newSquare = new Box();
	newSquare->boxPrimitive = new PhysicsEngine::CollisionBox();
	physicsEngine->createBox(newSquare->boxPrimitive);
	newSquare->boxPrimitive->setState(position, velocity, PhysicsEngine::Vector3(0.0f, 0.0f, 0.0f), mass, halfSize);
	rigidBodies->push_back(newSquare);
}

// Add a rigid cube with the inputted parameters
void Controls::addRigidCube(std::vector<Box *> *rigidBodies, PhysicsEngine::Vector3 position, PhysicsEngine::Vector3 velocity, real mass, PhysicsEngine::Vector3 halfSize)
{
	Box *newSquare = new Box();
	newSquare->boxPrimitive->setState(position, velocity, PhysicsEngine::Vector3(0, -9.81, 0), mass, halfSize);
	rigidBodies->push_back(newSquare);
}

// Add a cube rigid body to the scene
void Controls::addRigidCubeWhereYouLook(std::vector<Box *> *rigidBodies, float theta, float phi)
{
	PhysicsEngine::Vector3 squareCreationPosition = rotatePositionAlongYAxis(6.0f, 4.0f, theta);

	addRigidCube(rigidBodies, squareCreationPosition, PhysicsEngine::Vector3(0.0f, 0.0f, 0.0f), 1.0f, PhysicsEngine::Vector3(.5f, .5f, .5f));
}

PhysicsEngine::Vector3 Controls::rotatePositionAlongYAxis(real depth, real height, real theta)
{
	PhysicsEngine::Vector3 objectInitialPosition(0.0f, height, depth);
	float thetaRads = theta * PI / 180.0;
	PhysicsEngine::Quaternion cameraRotation(cos(thetaRads / 2), 0.0f, 1.0f * sin(thetaRads / 2), 0.0f);
	PhysicsEngine::Matrix3 rotMatrix;
	rotMatrix.setOrientation(cameraRotation);

	return rotMatrix.transform(objectInitialPosition);
}

// Add a capsule with the inputted parameters
void Controls::addRigidCapsule
(
	PhysicsEngine::Physics *physicsEngine,
	std::vector<Capsule *> *capsuleBodies,
	PhysicsEngine::Vector3 position,
	PhysicsEngine::Quaternion rotation,
	PhysicsEngine::Vector3 velocity,
	real mass,
	real radius,
	real height
)
{
	Capsule *newCapsule = new Capsule();
	newCapsule->capsulePrimitive = new PhysicsEngine::CollisionCapsule();
	physicsEngine->createCapsule(newCapsule->capsulePrimitive);
	newCapsule->capsulePrimitive->setState(position, rotation, velocity, PhysicsEngine::Vector3(), mass, radius, height);
	capsuleBodies->push_back(newCapsule);
}