#ifndef PHYSICS_ENGINE_CONTROLS_H
#define PHYSICS_ENGINE_CONTROLS_H

#include "RenderParticles.h"
#include "RenderableObjects.h"
#include "PhysicsEngine.h"
#include "RigidBody.h"
#include <vector>

namespace PhysicsDemo
{
	class Controls
	{
	public:
		// Check for keystrokes and take the appropriate action based on them
		static void keyCheck
		(
			unsigned char key,
			PhysicsEngine::Physics *physicsEngine,
			std::vector<RenderableParticle *> *particles,
			std::vector<Box *> *boxes,
			std::vector<Sphere *> *spheres,
			std::vector<Capsule *> *capsules,
			float theta,
			float phi
		);
	private:
		/////////////////////////////////////////////////////////////////////////
		//////////////////////// ADD OBJECTS TO RENDER //////////////////////////
		/////////////////////////////////////////////////////////////////////////
		static void addParticle
		(
			PhysicsEngine::Physics *physicsEngine,
			std::vector<RenderableParticle *> *particles,
			PhysicsEngine::Vector3 position,
			PhysicsEngine::real mass,
			PhysicsEngine::real radius,
			PhysicsEngine::real height
		);
		static void addSphere
		(
			PhysicsEngine::Physics *physicsEngine,
			std::vector<Sphere *> *sphereBodies,
			PhysicsEngine::Vector3 position,
			PhysicsEngine::Vector3 velocity,
			PhysicsEngine::Vector3 acceleration,
			PhysicsEngine::real mass,
			PhysicsEngine::real radius
		);
		static void addCube
		(
			PhysicsEngine::Physics *physicsEngine,
			std::vector<Box *> *rectangularBodies,
			PhysicsEngine::Vector3 position,
			PhysicsEngine::Vector3 velocity,
			PhysicsEngine::Vector3 acceleration,
			PhysicsEngine::real mass,
			PhysicsEngine::Vector3 halfSize
		);
		static void addCapsule
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
		);

		/////////////////////////////////////////////////////////////////////////
		////////////////////////  OBJECT INTERACTIONS  //////////////////////////
		/////////////////////////////////////////////////////////////////////////
		// Add force to the first cylinder
		static void addForceToCapsule(std::vector<Capsule *> *capsuleBodies);
		// Add force to the first cube
		static void addForceToCube(std::vector<Box *> *rectangularBodies);


		/////////////////////////////////////////////////////////////////////////
		////////////////////////    HELPER FUNCTIONS   //////////////////////////
		/////////////////////////////////////////////////////////////////////////
		static PhysicsEngine::Vector3 rotatePositionAlongYAxis(PhysicsEngine::real depth, PhysicsEngine::real height, PhysicsEngine::real theta);
	};	
}

#endif // PHYSICS_ENGINE_CONTROLS_H