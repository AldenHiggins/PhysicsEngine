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
			addCapsule
			(
				demo,
				rotatePositionAlongYAxis(13.0f, 5.7f, demo->player.getYaw()), PhysicsEngine::Quaternion::fromEuler(1, 0, 0), PhysicsEngine::Vector3(0.0f, 0, 0), PhysicsEngine::Vector3(),
				10.0f, 1.0f, 1.0f
			);

			addForceToCapsule(demo, (demo->capsuleObjects.size() - 1), PhysicsEngine::Vector3(3000.0f, 450.0f, 0.0f), PhysicsEngine::Vector3());
			break;
		}
		case '2':
		{
			//addCube
			//(
			//	physicsEngine, renderableObjects, rectangularBodies,
			//	PhysicsEngine::Vector3(-20.0f, 5.0f, 0.0f), PhysicsEngine::Vector3(), PhysicsEngine::Vector3(),
			//	10.0f, PhysicsEngine::Vector3(0.5f, 5.0f, 20.0f)
			//);
			//addCube
			//(
			//	physicsEngine, renderableObjects, rectangularBodies,
			//	PhysicsEngine::Vector3(20.0f, 5.0f, 0.0f), PhysicsEngine::Vector3(), PhysicsEngine::Vector3(),
			//	10.0f, PhysicsEngine::Vector3(0.5f, 5.0f, 20.0f)
			//);
			//addCube
			//(
			//	physicsEngine, renderableObjects, rectangularBodies,
			//	PhysicsEngine::Vector3(0.0f, 5.0f, 20.0f), PhysicsEngine::Vector3(), PhysicsEngine::Vector3(),
			//	10.0f, PhysicsEngine::Vector3(20.0f, 5.0f, 0.5f)
			//);
			//addCube
			//(
			//	physicsEngine, renderableObjects, rectangularBodies,
			//	PhysicsEngine::Vector3(0.0f, 5.0f, -20.0f), PhysicsEngine::Vector3(), PhysicsEngine::Vector3(),
			//	10.0f, PhysicsEngine::Vector3(20.0f, 5.0f, 0.5f)
			//);

			addCube
			(
				demo,
				PhysicsEngine::Vector3(0.0f, 5.0f, 12.0f), PhysicsEngine::Vector3(0.0f, 0.0f, 0.0f), PhysicsEngine::Vector3(),
				10.0f, PhysicsEngine::Vector3(1.0f, .5f, .5f)
			);
			addCube
			(
				demo,
				PhysicsEngine::Vector3(3.0f, 7.0f, 12.0f), PhysicsEngine::Vector3(0.0f, 0.0f, 0.0f), PhysicsEngine::Vector3(),
				10.0f, PhysicsEngine::Vector3(1.0f, .5f, .5f)
			);
			addCube
			(
				demo,
				PhysicsEngine::Vector3(6.0f, 12.0f, 12.0f), PhysicsEngine::Vector3(0.0f, 0.0f, 0.0f), PhysicsEngine::Vector3(),
				10.0f, PhysicsEngine::Vector3(1.0f, .5f, .5f)
			);
			addCube
			(
				demo,
				PhysicsEngine::Vector3(9.0f, 15.0f, 12.0f), PhysicsEngine::Vector3(0.0f, 0.0f, 0.0f), PhysicsEngine::Vector3(),
				10.0f, PhysicsEngine::Vector3(1.0f, .5f, .5f)
			);

			//addParticle(physicsEngine, particles, PhysicsEngine::Vector3(0.0f, 4.0f, 6.0f), 1.0f, 1.0f, 1.0f);
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
	PhysicsEngine::Vector3 position,
	PhysicsEngine::real mass,
	PhysicsEngine::real radius,
	PhysicsEngine::real height
)
{
	RenderableParticle *particle = CreateParticle::createParticle(position, 5.0f, radius, PhysicsEngine::Vector3(.5f, .5f, .5f));
	particles->push_back(particle);
	physicsEngine->createParticle(particle->physicsParticle);
	renderableObjects->push_back(particle);
}

void Controls::addSphere
(
	RenderingDemo *demo,
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
	renderableObjects->push_back(newSphere);
}

void Controls::addCube
(
	RenderingDemo *demo,
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
	renderableObjects->push_back(newSquare);
}

void Controls::addCapsule
(
	RenderingDemo *demo,
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
	renderableObjects->push_back(newCapsule);
}

/////////////////////////////////////////////////////////////////////////
////////////////////////  OBJECT INTERACTIONS  //////////////////////////
/////////////////////////////////////////////////////////////////////////
// Add force to the first cube
void Controls::addForceToCube(RenderingDemo *demo, int cubeIndex, PhysicsEngine::Vector3 force, PhysicsEngine::Vector3 position)
{
	(demo->rectangleObjects)[0]->boxPrimitive->body->addForceAtBodyPoint(force, position);
}

// Add force to the first capsule
void Controls::addForceToCapsule(RenderingDemo *demo, int capsuleIndex, PhysicsEngine::Vector3 force, PhysicsEngine::Vector3 position)
{
	(demo->capsuleObjects)[capsuleIndex]->capsulePrimitive->body->addForceAtBodyPoint(force, position);
	//(*capsuleBodies)[0]->capsulePrimitive->body->addForceAtBodyPoint(PhysicsEngine::Vector3(-400.0f, 0.0f, 0.0f), PhysicsEngine::Vector3(0.5f, -1.5f, 1.0f));
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