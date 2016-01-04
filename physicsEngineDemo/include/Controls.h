#ifndef PHYSICS_ENGINE_CONTROLS_H
#define PHYSICS_ENGINE_CONTROLS_H

#include "Particle.h"
#include "RenderableObjects.h"
#include <vector>

namespace PhysicsEngine
{
	class Controls
	{
	public:
		// Check for keystrokes and take the appropriate action based on them
		static void keyCheck
		(
			unsigned char key,
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
		static void addRigidCubeNoGravity(std::vector<Box *> *rigidBodies, Vector3 position, Vector3 velocity, real mass, Vector3 halfSize);
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
			std::vector<Sphere *> *sphereBodies,
			Vector3 position,
			Vector3 velocity,
			real mass,
			real radius
		);
		// Add a rigid cube with the inputted parameters
		static void addRigidCube
		(
			std::vector<Box *> *rectangularBodies,
			Vector3 position,
			Vector3 velocity,
			real mass,
			Vector3 halfSize
		);
		// Add a capsule with the inputted parameters
		static void addRigidCapsule
		(
			std::vector<Capsule *> *capsuleBodies,
			Vector3 position,
			Quaternion rotation,
			Vector3 velocity,
			real mass,
			real radius,
			real height
		);

		static Vector3 rotatePositionAlongYAxis(real depth, real height, real theta);
	};
	
}

#endif // PHYSICS_ENGINE_CONTROLS_H