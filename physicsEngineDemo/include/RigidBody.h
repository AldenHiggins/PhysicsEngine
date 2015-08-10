#ifndef RIGID_BODY_H
#define RIGID_BODY_H

#include "MathDataTypes.h"

namespace PhysicsEngine
{
	class RigidBody
	{
	protected:
		// Basic object properties
		// Hold the inverse mass because it is more useful in calculations and approaches infinity rather than zero
		real inverseMass;
		Quaternion orientation;
		Vector3 position;
		// Linear movement values
		Vector3 linearVelocity;
		Vector3 linearAcceleration;
		Vector3 acceleration;
		Vector3 lastFrameAcceleration;
		// Angular movemement values
		Vector3 angularVelocity;
		// Store the inverse of the inertia tensor because that's the only form used in our calculations
		// and the tensor is a constant
		Matrix3 inverseInertiaTensor;
		// Store the inertia tensor in world coordinates to be used in conjunction with torque (also in world coords)
		Matrix3 inverseInertiaTensorWorld;

		// Transformation matrix for this object
		Matrix4 transformationMatrix;

		// All the accumulated forces on this object's center of mass this frame
		Vector3 forceAccum;

		// All the accumulated torque on this object this frame
		Vector3 torqueAccum;

	public:
		// Integrate this object based on the time elapsed this frame
		void integrate(real timeStep);

		// Display this rigid body
		virtual void display();

		// Add a force to this object's center of mass
		void addForce(const Vector3 &force);

		// Add a force at a position on this object in world space
		void addForceAtPoint(const Vector3 &force, const Vector3 &point);

		// Add a force at a position on this object in object space
		void addForceAtBodyPoint(const Vector3 &force, const Vector3 &point);

		// Clear all forces/torques active on the object
		void clearAccumulators();

		// Calculates all of the derived data that results from the current state of the rigidbody, this happens
		// automatically during integration
		void calculateDerivedData();

		// Set the inertia tensor of this rigid body
		void setInertiaTensor(const Matrix3 &inertiaTensor);

		// Set the mass of this object
		void setMass(real objectMass);

		// Get the mass of this object
		real getMass();

		// Set the position of this object
		void setPosition(Vector3 &positionInput);

		// Get the position of this object
		Vector3 getPosition();

		// Set the orientation of this rigid body
		void setOrientation(const Quaternion &orientationInput);

		// Get the orientation of this rigid body
		Quaternion getOrientation();

		// Set the velocity of the rigid body
		void setVelocity(const Vector3 &velocityInput);

		// Get the velocity of the rigid body
		Vector3 getVelocity() const;

		// Set the acceleration of the rigid body
		void setAcceleration(const Vector3 &accelerationInput);

		// Get the acceleration of the rigid body
		Vector3 getAcceleration() const;

		// Get this body's transformation matrix in a form that opengl can use
		void getGLTransform(float matrix[16]) const;

		// Transform a point from object space into world space
		Vector3 getPointInWorldSpace(const Vector3 &point) const;
	};

	class Square : public RigidBody
	{
	public:
		// Display this square
		virtual void display();
	};

}


#endif // RIGID_BODY_H