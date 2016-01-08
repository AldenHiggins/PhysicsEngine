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
			std::vector<Particle *> *particles,
			std::vector<Box *> *boxes,
			std::vector<Sphere *> *spheres,
			std::vector<Capsule *> *capsules,
			float theta,
			float phi
		);
	private:
		// Add force to the first cylinder
		static void addForceToCapsule(std::vector<Capsule *> *capsuleBodies);
		// Add force to the first cube
		static void addForceToCube(std::vector<Box *> *rectangularBodies);
		// Add a cube rigid body to the scene
		static void addRigidCubeWhereYouLook(std::vector<Box *> *rectangularBodies, float theta, float phi);
		// Add a rigid cube with no gravity
		static void addRigidCubeNoGravity
		(
			PhysicsEngine::Physics *physicsEngine,
			std::vector<Box *> *rigidBodies,
			PhysicsEngine::Vector3 position,
			PhysicsEngine::Vector3 velocity,
			real mass,
			PhysicsEngine::Vector3 halfSize
		);
		// Add a sphere to the scene where you look
		static void addSphereWhereYouLook
		(
			std::vector<Sphere *> *sphereBodies,
			float theta,
			float phi,
			float radius
		);
		// Add a sphere to the scene with the given properties
		static void addSphere
		(
			PhysicsEngine::Physics *physicsEngine,
			std::vector<Sphere *> *sphereBodies,
			PhysicsEngine::Vector3 position,
			PhysicsEngine::Vector3 velocity,
			PhysicsEngine::Vector3 acceleration,
			real mass,
			real radius
		);
		// Add a rigid cube with the inputted parameters
		static void addRigidCube
		(
			std::vector<Box *> *rectangularBodies,
			PhysicsEngine::Vector3 position,
			PhysicsEngine::Vector3 velocity,
			real mass,
			PhysicsEngine::Vector3 halfSize
		);
		// Add a capsule with the inputted parameters
		static void addRigidCapsule
		(
			PhysicsEngine::Physics *physicsEngine,
			std::vector<Capsule *> *capsuleBodies,
			PhysicsEngine::Vector3 position,
			PhysicsEngine::Quaternion rotation,
			PhysicsEngine::Vector3 velocity,
			real mass,
			real radius,
			real height
		);

		static PhysicsEngine::Vector3 rotatePositionAlongYAxis(real depth, real height, real theta);
	};
	
}

#endif // PHYSICS_ENGINE_CONTROLS_H