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
	std::vector<RenderableParticle *> *particles,
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
			addCapsule
			(
				physicsEngine, capsules,
				PhysicsEngine::Vector3(-3.0f, 5.0f, 12.0f), PhysicsEngine::Quaternion::fromEuler(1, 0, 0), PhysicsEngine::Vector3(10.0f, 0, 0), PhysicsEngine::Vector3(),
				10.0f, 1.0f, 1.0f
			);
			addCube
			(
				physicsEngine, rectangularBodies,
				PhysicsEngine::Vector3(0.0f, 5.0f, 12.0f), PhysicsEngine::Vector3(0.0f, 0.0f, 0.0f), PhysicsEngine::Vector3(),
				10.0f, PhysicsEngine::Vector3(1.0f, .5f, .5f)
			);
			addSphere
			(
				physicsEngine, spheres,
				PhysicsEngine::Vector3(3.0f, 5.0f, 12.0f), PhysicsEngine::Vector3(), PhysicsEngine::Vector3(),
				1.0f, 3.0f
			);
			break;
		}
		case '2':
		{
			addParticle(physicsEngine, particles, PhysicsEngine::Vector3(0.0f, 4.0f, 6.0f), 1.0f, 1.0f, 1.0f);
			break;
		}
	}
}

/////////////////////////////////////////////////////////////////////////
//////////////////////// ADD OBJECTS TO RENDER //////////////////////////
/////////////////////////////////////////////////////////////////////////
void Controls::addParticle
(
	PhysicsEngine::Physics *physicsEngine,
	std::vector<RenderableParticle *> *particles,
	PhysicsEngine::Vector3 position,
	PhysicsEngine::real mass,
	PhysicsEngine::real radius,
	PhysicsEngine::real height
)
{
	RenderableParticle *particle = CreateParticle::createParticle(position, 5.0f, radius, PhysicsEngine::Vector3(.5f, .5f, .5f));
	particles->push_back(particle);
	physicsEngine->createParticle(particle->physicsParticle);
}

void Controls::addSphere
(
	PhysicsEngine::Physics *physicsEngine,
	std::vector<Sphere *> *sphereBodies,
	PhysicsEngine::Vector3 position,
	PhysicsEngine::Vector3 velocity,
	PhysicsEngine::Vector3 acceleration,
	PhysicsEngine::real mass,
	PhysicsEngine::real radius
)
{
	Sphere *newSphere = new Sphere();
	physicsEngine->createSphere(newSphere->spherePrimitive);
	newSphere->spherePrimitive->setState(position, velocity, acceleration, mass, radius);
	newSphere->spherePrimitive->body->calculateDerivedData();
	sphereBodies->push_back(newSphere);
}

void Controls::addCube
(
	PhysicsEngine::Physics *physicsEngine,
	std::vector<Box *> *rigidBodies,
	PhysicsEngine::Vector3 position,
	PhysicsEngine::Vector3 velocity,
	PhysicsEngine::Vector3 acceleration,
	PhysicsEngine::real mass,
	PhysicsEngine::Vector3 halfSize
)
{
	Box *newSquare = new Box();
	physicsEngine->createBox(newSquare->boxPrimitive);
	newSquare->boxPrimitive->body->setStatic(true);
	newSquare->boxPrimitive->setState(position, velocity, PhysicsEngine::Vector3(0, -9.81, 0), mass, halfSize);
	newSquare->boxPrimitive->body->calculateDerivedData();
	rigidBodies->push_back(newSquare);
}

void Controls::addCapsule
(
	PhysicsEngine::Physics *physicsEngine,
	std::vector<Capsule *> *capsuleBodies,
	PhysicsEngine::Vector3 position,
	PhysicsEngine::Quaternion rotation,
	PhysicsEngine::Vector3 velocity,
	PhysicsEngine::Vector3 acceleration,
	PhysicsEngine::real mass,
	PhysicsEngine::real radius,
	PhysicsEngine::real height
)
{
	Capsule *newCapsule = new Capsule();
	physicsEngine->createCapsule(newCapsule->capsulePrimitive);
	newCapsule->capsulePrimitive->setState(position, rotation, velocity, acceleration, mass, radius, height);
	newCapsule->capsulePrimitive->body->calculateDerivedData();
	capsuleBodies->push_back(newCapsule);
}

/////////////////////////////////////////////////////////////////////////
////////////////////////  OBJECT INTERACTIONS  //////////////////////////
/////////////////////////////////////////////////////////////////////////
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

/////////////////////////////////////////////////////////////////////////
////////////////////////    HELPER FUNCTIONS   //////////////////////////
/////////////////////////////////////////////////////////////////////////
PhysicsEngine::Vector3 Controls::rotatePositionAlongYAxis(PhysicsEngine::real depth, PhysicsEngine::real height, PhysicsEngine::real theta)
{
	PhysicsEngine::Vector3 objectInitialPosition(0.0f, height, depth);
	float thetaRads = theta * PI / 180.0;
	PhysicsEngine::Quaternion cameraRotation(cos(thetaRads / 2), 0.0f, 1.0f * sin(thetaRads / 2), 0.0f);
	PhysicsEngine::Matrix3 rotMatrix;
	rotMatrix.setOrientation(cameraRotation);

	return rotMatrix.transform(objectInitialPosition);
}