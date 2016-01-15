#ifndef PHYSICS_ENGINE_CONTROLS_H
#define PHYSICS_ENGINE_CONTROLS_H

#include "RenderParticles.h"
#include "RenderableObjects.h"
#include "PhysicsEngine.h"
#include "RigidBody.h"
#include "PlayerControls.h"
#include "PhysicsDemo.h"
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
			RenderingDemo *demo
		);
	private:
		/////////////////////////////////////////////////////////////////////////
		//////////////////////// ADD OBJECTS TO RENDER //////////////////////////
		/////////////////////////////////////////////////////////////////////////
		static void addParticle
		(
			RenderingDemo *demo,
			PhysicsEngine::Vector3 position,
			PhysicsEngine::real mass,
			PhysicsEngine::real radius,
			PhysicsEngine::real height
		);
		static void addSphere
		(
			RenderingDemo *demo,
			PhysicsEngine::Vector3 position,
			PhysicsEngine::Vector3 velocity,
			PhysicsEngine::Vector3 acceleration,
			PhysicsEngine::real mass,
			PhysicsEngine::real radius
		);
		static void addCube
		(
			RenderingDemo *demo,
			PhysicsEngine::Vector3 position,
			PhysicsEngine::Vector3 velocity,
			PhysicsEngine::Vector3 acceleration,
			PhysicsEngine::real mass,
			PhysicsEngine::Vector3 halfSize
		);
		static void addCapsule
		(
			RenderingDemo *demo,
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
		static void addForceToCapsule(RenderingDemo *demo, int capsuleIndex, PhysicsEngine::Vector3 force, PhysicsEngine::Vector3 position);
		// Add force to the first cube
		static void addForceToCube(RenderingDemo *demo, int capsuleIndex, PhysicsEngine::Vector3 force, PhysicsEngine::Vector3 position);


		/////////////////////////////////////////////////////////////////////////
		////////////////////////    HELPER FUNCTIONS   //////////////////////////
		/////////////////////////////////////////////////////////////////////////
		static PhysicsEngine::Vector3 rotatePositionAlongYAxis(PhysicsEngine::real depth, PhysicsEngine::real height, PhysicsEngine::real theta);
	};	
}

#endif // PHYSICS_ENGINE_CONTROLS_H