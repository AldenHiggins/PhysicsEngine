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

	float cM; // cylinder mass
	float hsM; // mass of hemispheres
	float rSq = radiusInput * radiusInput;
	cM = PI * heightInput * rSq * density;
	hsM = PI * 2 * oneDiv3 * rSq * radiusInput * density;

	// from cylinder
	tensor.data[4] = rSq * cM * 0.5f;
	tensor.data[0] = tensor.data[8] = tensor.data[4] * 0.5f + cM * heightInput * heightInput * oneDiv12;
	// from hemispheres
	float temp0 = hsM * 2.0f * rSq / 5.0f;
	tensor.data[4] += temp0 * 2.0f;
	float temp1 = heightInput * 0.5f;
	float temp2 = temp0 + hsM * (temp1 * temp1 + 3.0f * oneDiv8 * heightInput * heightInput);
	tensor.data[0] += temp2 * 2.0f;
	tensor.data[8] += temp2 * 2.0f;
	tensor.data[1] = tensor.data[2] = tensor.data[3] = tensor.data[5] = tensor.data[6] = tensor.data[7] = 0.0f;

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


