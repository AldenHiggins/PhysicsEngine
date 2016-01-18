#include "CollisionPrimitives.h"
#include "ApplicationSettings.h"

using namespace PhysicsEngine;

// Constants used for the inertia tensor calculation of the capsules
const float oneDiv3 = (float)(1.0 / 3.0);
const float oneDiv8 = (float)(1.0 / 8.0);
const float oneDiv12 = (float)(1.0 / 12.0);
const float density = 1.0f;

// Set all of the parameters for this collision box
void CollisionBox::setState(Vector3 position, Vector3 velocity, Vector3 acceleration, real mass, Vector3 halfSizeInput)
{
	halfSize = halfSizeInput;
	body->setPosition(position);
	body->setVelocity(velocity);
	body->setAcceleration(acceleration);
	body->setMass(mass);
	Matrix3 tensor;
	tensor.setBlockInertiaTensor(halfSizeInput, mass);
	body->setInertiaTensor(tensor);
	body->setDamping(SQUARE_LINEAR_DAMPING, SQUARE_ANGULAR_DAMPING);
}

// Set all of the parameters for this collision sphere
void CollisionSphere::setState(Vector3 position, Vector3 velocity, Vector3 acceleration, real mass, real radiusInput)
{
	radius = radiusInput;
	body->setPosition(position);
	body->setVelocity(velocity);
	body->setAcceleration(acceleration);
	body->setMass(mass);
	body->setDamping(SQUARE_LINEAR_DAMPING, SQUARE_ANGULAR_DAMPING);
	// Set the inertia tensor for the sphere
	Matrix3 tensor;
	real coeff = 0.4f*mass*radius*radius;
	tensor.setInertiaTensorCoeffs(coeff, coeff, coeff);
	body->setInertiaTensor(tensor);
}

// Set all of the parameters for this rigid body/Collision Box
void CollisionCapsule::setState(Vector3 position, Quaternion rotation, Vector3 velocity, Vector3 acceleration, real mass, real radiusInput, real heightInput)
{
	radius = radiusInput;
	height = heightInput;
	body->setPosition(position);
	body->setOrientation(rotation);
	body->setVelocity(velocity);
	body->setAcceleration(acceleration);
	body->setMass(mass);
	body->setDamping(SQUARE_LINEAR_DAMPING, SQUARE_ANGULAR_DAMPING);

	// Set the inertia tensor for the capsule
	Matrix3 tensor;
	real xValue = (1.0f / 12.0f) * mass * height * height + (1.0f / 4.0f) * mass * radius * radius;
	real yValue = (1.0f / 2.0f) * mass * radius * radius;
	real zValue = (1.0f / 12.0f) * mass * height * height + (1.0f / 4.0f) * mass * radius * radius;
	tensor.setDiagonal(xValue, yValue, zValue);

	body->setInertiaTensor(tensor);
}

CollisionCapsule::CollisionCapsule()
{
	body = new RigidBody;
}

CollisionSphere::CollisionSphere()
{
	body = new RigidBody;
}

CollisionBox::CollisionBox()
{
	body = new RigidBody;
}


