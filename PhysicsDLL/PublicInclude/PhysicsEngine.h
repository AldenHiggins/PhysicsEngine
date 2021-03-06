#ifdef PHYSICSDLL_EXPORTS
#define PHYSICSDLL_API __declspec(dllexport) 
#else
#define PHYSICSDLL_API __declspec(dllimport) 
#endif

#include "CollisionPrimitives.h"
#include "Particle.h"
#include "Collisions.h"

namespace PhysicsEngine
{
	class Physics
	{
	private:
		// Contains all of the particles in the scene
		std::vector<Particle *> particles;
		// Contains all the rectangular objects in the scene
		std::vector<CollisionBox *> rectangleObjects;
		// Contains all of the spherical objects in the scene
		std::vector<CollisionSphere *> sphereObjects;
		// Contains all of the capsules in the scene
		std::vector<CollisionCapsule *> capsuleObjects;
		// Contains all of the planes in the scene
		std::vector<CollisionPlane *> planeObjects;
		// Contains all of the collisions this frame
		std::vector<Collision> collisionList;

		// Integrate all of the rigid bodies
		void integrateRigidBodies(real duration);
		// Detect collisions
		void detectCollisions(std::vector<Collision> *collisionList);
		// Resolve the found collisions
		void resolveCollisions(std::vector<Collision> *collisionList, real duration);
	public:
		// Construct the physics engine
		PHYSICSDLL_API Physics();

		// Update the physics engine based on the elapsed time
		PHYSICSDLL_API void updatePhysics(float duration);

		// Get the current collisions this frame
		PHYSICSDLL_API std::vector<Collision> *getCollisionList();

		// Add objects to the physics engine
		PHYSICSDLL_API void createCapsule(CollisionCapsule *createdCapsule);
		PHYSICSDLL_API void createSphere(CollisionSphere *createdCapsule);
		PHYSICSDLL_API void createBox(CollisionBox *createdBox);
		PHYSICSDLL_API void createParticle(Particle *createdParticle);
		PHYSICSDLL_API void createPlane(CollisionPlane *createdPlane);
	};

}