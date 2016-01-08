#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include "PhysicsEngine.h"
#include "MathDataTypes.h"
#include "ApplicationSettings.h"
#include "Particle.h"
#include "RigidBody.h"
#include "BoundingVolumes.h"
#include "Collisions.h"
#include "CollisionResolver.h"
#include "CollisionDetection.h"
#include "CollisionPrimitives.h"

using namespace PhysicsEngine;

Physics::Physics()
{
	particles;
}

// Create a new capsule and register it with the physics engine
void Physics::createCapsule(CollisionCapsule *createdCapsule)
{
	capsuleObjects.push_back(createdCapsule);
}

void Physics::createSphere(CollisionSphere *createdSphere)
{
	sphereObjects.push_back(createdSphere);
}

void Physics::createBox(CollisionBox *createdBox)
{
	rectangleObjects.push_back(createdBox);
}

/**
* Update function that progresses the physics engine
*/
void Physics::updatePhysics(float duration)
{
	// Integrate all of the rigid bodies
	integrateRigidBodies(duration);

	// Check for and resolve collisions
	std::vector<Collision> collisionList;
	detectCollisions(&collisionList);
	resolveCollisions(&collisionList, duration);
}

// Resolve the found collisions
void Physics::resolveCollisions(std::vector<Collision> *collisionList, real duration)
{
	if (collisionList->size() > 0)
	{
		CollisionResolver resolver(collisionList->size() * 4, collisionList->size() * 4);
		resolver.resolveContacts(collisionList, duration);
	}
}

// Detect collisions
void Physics::detectCollisions(std::vector<Collision> *collisionList)
{
	// Detect cube collisions
	for (unsigned int rigidBodyIndex = 0; rigidBodyIndex < rectangleObjects.size(); rigidBodyIndex++)
	{
		// Check for collisions against the ground
		CollisionDetection::boxAndHalfSpaceCollisionDetect(rectangleObjects[rigidBodyIndex], Vector3(0, 1, 0), 0, collisionList);
		// Check for collisions against the walls
		CollisionDetection::boxAndHalfSpaceCollisionDetect(rectangleObjects[rigidBodyIndex], Vector3(-1, 0, 0), -20, collisionList);
		CollisionDetection::boxAndHalfSpaceCollisionDetect(rectangleObjects[rigidBodyIndex], Vector3(1, 0, 0), -20, collisionList);
		CollisionDetection::boxAndHalfSpaceCollisionDetect(rectangleObjects[rigidBodyIndex], Vector3(0, 0, -1), -20, collisionList);
		CollisionDetection::boxAndHalfSpaceCollisionDetect(rectangleObjects[rigidBodyIndex], Vector3(0, 0, 1), -20, collisionList);

		// Search for box/box collisions
		for (int otherRigidBodyIndex = rigidBodyIndex + 1; otherRigidBodyIndex < rectangleObjects.size(); otherRigidBodyIndex++)
		{
			CollisionDetection::cubeCubeCollisionDetect(collisionList, rectangleObjects[rigidBodyIndex], rectangleObjects[otherRigidBodyIndex]);
		}
	}

	// Detect sphere collisions
	for (int sphereIndex = 0; sphereIndex < sphereObjects.size(); sphereIndex++)
	{
		// Check for collisions against ground
		CollisionDetection::sphereAndHalfSpaceCollisionDetect(sphereObjects[sphereIndex], Vector3(0, 1, 0), 0, collisionList);
		// Check for collisions against the walls
		CollisionDetection::sphereAndHalfSpaceCollisionDetect(sphereObjects[sphereIndex], Vector3(-1, 0, 0), -20, collisionList);
		CollisionDetection::sphereAndHalfSpaceCollisionDetect(sphereObjects[sphereIndex], Vector3(1, 0, 0), -20, collisionList);
		CollisionDetection::sphereAndHalfSpaceCollisionDetect(sphereObjects[sphereIndex], Vector3(0, 0, -1), -20, collisionList);
		CollisionDetection::sphereAndHalfSpaceCollisionDetect(sphereObjects[sphereIndex], Vector3(0, 0, 1), -20, collisionList);

		// Check for collisions against other spheres
		for (int otherSphereIndex = sphereIndex + 1; otherSphereIndex < sphereObjects.size(); otherSphereIndex++)
		{
			CollisionDetection::sphereSphereCollisionDetect(sphereObjects[sphereIndex], sphereObjects[otherSphereIndex], collisionList);
		}

		// Check for collisions against cubes
		for (int cubeIndex = 0; cubeIndex < rectangleObjects.size(); cubeIndex++)
		{
			CollisionDetection::sphereCubeCollisionDetect(sphereObjects[sphereIndex], rectangleObjects[cubeIndex], collisionList);
		}
	}

	// Detect capsule collisions
	for (int capsuleIndex = 0; capsuleIndex < capsuleObjects.size(); capsuleIndex++)
	{
		CollisionDetection::capsuleHalfSpaceCollisionDetect(capsuleObjects[capsuleIndex], Vector3(0, 1, 0), 0, collisionList);
		// Check for collisions against the walls
		CollisionDetection::capsuleHalfSpaceCollisionDetect(capsuleObjects[capsuleIndex], Vector3(-1, 0, 0), -20, collisionList);
		CollisionDetection::capsuleHalfSpaceCollisionDetect(capsuleObjects[capsuleIndex], Vector3(1, 0, 0), -20, collisionList);
		CollisionDetection::capsuleHalfSpaceCollisionDetect(capsuleObjects[capsuleIndex], Vector3(0, 0, -1), -20, collisionList);
		CollisionDetection::capsuleHalfSpaceCollisionDetect(capsuleObjects[capsuleIndex], Vector3(0, 0, 1), -20, collisionList);

		// Check for collisions against spheres
		for (int sphereIndex = 0; sphereIndex< sphereObjects.size(); sphereIndex++)
		{
			CollisionDetection::capsuleSphereCollisionDetect(capsuleObjects[capsuleIndex], sphereObjects[sphereIndex], collisionList);
		}

		// Check for collisions against cubes
		for (int cubeIndex = 0; cubeIndex < rectangleObjects.size(); cubeIndex++)
		{
			CollisionDetection::capsuleSquareCollisionDetect(capsuleObjects[capsuleIndex], rectangleObjects[cubeIndex], collisionList);
		}

		// Check for collisions against other capsules
		for (int otherCapsuleIndex = capsuleIndex + 1; otherCapsuleIndex < capsuleObjects.size(); otherCapsuleIndex++)
		{
			CollisionDetection::capsuleCapsuleCollisionDetect(capsuleObjects[capsuleIndex], capsuleObjects[otherCapsuleIndex], collisionList);
		}
	}
}

// Integrate all of the rigid bodies in the scene
void Physics::integrateRigidBodies(real duration)
{
	// Integrate all of the particles
	for (int particleIndex = 0; particleIndex < particles.size(); particleIndex++)
	{
		particles[particleIndex]->integrate(duration);
	}

	// Integrate all of the cubes
	if (rectangleObjects.size() > 0)
	{
		// Integrate all of the rigid bodies and add them to the bounding sphere heirarchy
		for (int rigidBodyIndex = 0; rigidBodyIndex < rectangleObjects.size(); rigidBodyIndex++)
		{
			RigidBody *body = rectangleObjects[rigidBodyIndex]->body;
			body->integrate(duration);
		}
	}

	// Integrate all of the spheres
	if (sphereObjects.size() > 0)
	{
		for (int rigidBodyIndex = 0; rigidBodyIndex < sphereObjects.size(); rigidBodyIndex++)
		{
			sphereObjects[rigidBodyIndex]->body->integrate(duration);
		}
	}

	// Integrate all the cylinders
	for (unsigned int rigidBodyIndex = 0; rigidBodyIndex < capsuleObjects.size(); rigidBodyIndex++)
	{
		capsuleObjects[rigidBodyIndex]->body->integrate(duration);
	}
}

/**
* The main entry point. We pass arguments onto GLUT.
*/
int main(int argc, char** argv)
{
	return 0;
}