#ifndef PHYSICS_ENGINE_CONTROLS_H
#define PHYSICS_ENGINE_CONTROLS_H

#include "RenderParticles.h"
#include "RenderableObjects.h"
#include "PhysicsEngine.h"
#include "RigidBody.h"
#include "PlayerControls.h"
#include "PhysicsDemo.h"
#include "DataTypeRedefinition.h"
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
			Vector3 position,
			real mass,
			real radius,
			real height
		);
		static void addSphere
		(
			RenderingDemo *demo,
			Vector3 position,
			Vector3 velocity,
			Vector3 acceleration,
			real mass,
			real radius
		);
		static void addCube
		(
			RenderingDemo *demo,
			Vector3 position,
			Vector3 velocity,
			Vector3 acceleration,
			real mass,
			Vector3 halfSize
		);
		static void addCapsule
		(
			RenderingDemo *demo,
			Vector3 position,
			Quaternion rotation,
			Vector3 velocity,
			Vector3 acceleration,
			real mass,
			real radius,
			real height
		);

		/////////////////////////////////////////////////////////////////////////
		////////////////////////  OBJECT INTERACTIONS  //////////////////////////
		/////////////////////////////////////////////////////////////////////////
		// Add force to the capsule at capsuleIndex
		static void addForceToCapsule(RenderingDemo *demo, int capsuleIndex, Vector3 force, Vector3 position);
		// Add force to the cube at cubeIndex
		static void addForceToCube(RenderingDemo *demo, int cubeIndex, Vector3 force, Vector3 position);
		// Add force to the sphere at sphereIndex
		static void addForceToSphere(RenderingDemo *demo, int sphereIndex, Vector3 force, Vector3 position);

		/////////////////////////////////////////////////////////////////////////
		////////////////////////    HELPER FUNCTIONS   //////////////////////////
		/////////////////////////////////////////////////////////////////////////
		static Vector3 rotatePositionAlongYAxis(real depth, real height, real theta);
	};	
}

#endif // PHYSICS_ENGINE_CONTROLS_H