#include "Collisions.h"
#include <assert.h>
#include <iostream>

using namespace PhysicsEngine;

static inline real penetrationOnAxis(
	const RigidBody *one,
	const RigidBody *two,
	const Vector3 &axis,
	const Vector3 &toCentre
	);

static inline real transformToAxis(
	const RigidBody *box,
	const Vector3 &axis
	);

void fillPointFaceBoxBox(
	RigidBody *one,
	RigidBody *two,
	const Vector3 &toCentre,
	std::vector<Collision> *data,
	unsigned best,
	real pen
	);

inline Vector3 contactPointCalculate(
	const Vector3 &pOne,
	const Vector3 &dOne,
	real oneSize,
	const Vector3 &pTwo,
	const Vector3 &dTwo,
	real twoSize,

	// If this is true, and the contact point is outside
	// the edge (in the case of an edge-face contact) then
	// we use one's midpoint, otherwise we use two's.
	bool useOne);

#define CHECK_OVERLAP(axis, index) \
    if (!tryAxis(first, other, (axis), toCentre, (index), pen, best)) return 0;

static inline bool tryAxis(
	const RigidBody *one,
	const RigidBody *two,
	Vector3 axis,
	const Vector3& toCentre,
	unsigned index,

	// These values may be updated
	real& smallestPenetration,
	unsigned &smallestCase
	)
{
	// Make sure we have a normalized axis, and don't check almost parallel axes
	if (axis.squareMagnitude() < 0.0001) return true;
	axis.normalise();

	real penetration = penetrationOnAxis(one, two, axis, toCentre);

	if (penetration < 0) return false;
	if (penetration < smallestPenetration) {
		smallestPenetration = penetration;
		smallestCase = index;
	}
	return true;
}

/*
* This function checks if the two boxes overlap
* along the given axis, returning the ammount of overlap.
* The final parameter toCentre
* is used to pass in the vector between the boxes centre
* points, to avoid having to recalculate it each time.
*/
static inline real penetrationOnAxis(
	const RigidBody *one,
	const RigidBody *two,
	const Vector3 &axis,
	const Vector3 &toCentre
	)
{
	// Project the half-size of one onto axis
	real oneProject = transformToAxis(one, axis);
	real twoProject = transformToAxis(two, axis);

	// Project this onto the axis
	real distance = real_abs(toCentre * axis);

	// Return the overlap (i.e. positive indicates
	// overlap, negative indicates separation).
	return oneProject + twoProject - distance;
}

static inline real transformToAxis(
	const RigidBody *box,
	const Vector3 &axis
	)
{
	return 
		.5f * real_abs(axis * box->getTransformMatrix().getAxisVector(0)) +
		.5f * real_abs(axis * box->getTransformMatrix().getAxisVector(1)) +
		.5f * real_abs(axis * box->getTransformMatrix().getAxisVector(2));
}

unsigned int Collision::cubeCubeCollisionDetect(std::vector<Collision> *collisionList, RigidBody *first, RigidBody *other)
{
	//if (!IntersectionTests::boxAndBox(one, two)) return 0;

	// Find the vector between the two centres
	//Vector3 toCentre = two.getAxis(3) - one.getAxis(3);
	Vector3 toCentre = other->getPosition() - first->getPosition();

	Matrix4 firstTransform = first->getTransformMatrix();
	Matrix4 secondTransform = other->getTransformMatrix();

	// We start assuming there is no contact
	real pen = REAL_MAX;
	unsigned best = 0xffffff;

	// Now we check each axes, returning if it gives us
	// a separating axis, and keeping track of the axis with
	// the smallest penetration otherwise.
	CHECK_OVERLAP(firstTransform.getAxisVector(0), 0);
	CHECK_OVERLAP(firstTransform.getAxisVector(1), 1);
	CHECK_OVERLAP(firstTransform.getAxisVector(2), 2);

	CHECK_OVERLAP(secondTransform.getAxisVector(0), 3);
	CHECK_OVERLAP(secondTransform.getAxisVector(1), 4);
	CHECK_OVERLAP(secondTransform.getAxisVector(2), 5);

	// Store the best axis-major, in case we run into almost
	// parallel edge collisions later
	unsigned bestSingleAxis = best;

	CHECK_OVERLAP(firstTransform.getAxisVector(0) % secondTransform.getAxisVector(0), 6);
	CHECK_OVERLAP(firstTransform.getAxisVector(0) % secondTransform.getAxisVector(1), 7);
	CHECK_OVERLAP(firstTransform.getAxisVector(0) % secondTransform.getAxisVector(2), 8);
	CHECK_OVERLAP(firstTransform.getAxisVector(1) % secondTransform.getAxisVector(0), 9);
	CHECK_OVERLAP(firstTransform.getAxisVector(1) % secondTransform.getAxisVector(1), 10);
	CHECK_OVERLAP(firstTransform.getAxisVector(1) % secondTransform.getAxisVector(2), 11);
	CHECK_OVERLAP(firstTransform.getAxisVector(2) % secondTransform.getAxisVector(0), 12);
	CHECK_OVERLAP(firstTransform.getAxisVector(2) % secondTransform.getAxisVector(1), 13);
	CHECK_OVERLAP(firstTransform.getAxisVector(2) % secondTransform.getAxisVector(2), 14);

	// Make sure we've got a result.
	assert(best != 0xffffff);

	// We now know there's a collision, and we know which
	// of the axes gave the smallest penetration. We now
	// can deal with it in different ways depending on
	// the case.
	if (best < 3)
	{
		// We've got a vertex of box two on a face of box one.
		fillPointFaceBoxBox(first, other, toCentre, collisionList, best, pen);
		return 1;
	}
	else if (best < 6)
	{
		// We've got a vertex of box one on a face of box two.
		// We use the same algorithm as above, but swap around
		// one and two (and therefore also the vector between their
		// centres).
		fillPointFaceBoxBox(first, other, toCentre*-1.0f, collisionList, best - 3, pen);
		return 1;
	}
	else
	{
		// We've got an edge-edge contact. Find out which axes
		best -= 6;
		unsigned oneAxisIndex = best / 3;
		unsigned twoAxisIndex = best % 3;
		Vector3 oneAxis = first->getTransformMatrix().getAxisVector(oneAxisIndex);
		Vector3 twoAxis = other->getTransformMatrix().getAxisVector(twoAxisIndex);
		Vector3 axis = oneAxis % twoAxis;
		axis.normalise();

		// The axis should point from box one to box two.
		if (axis * toCentre > 0) axis = axis * -1.0f;

		// We have the axes, but not the edges: each axis has 4 edges parallel
		// to it, we need to find which of the 4 for each object. We do
		// that by finding the point in the centre of the edge. We know
		// its component in the direction of the box's collision axis is zero
		// (its a mid-point) and we determine which of the extremes in each
		// of the other axes is closest.
		Vector3 ptOnOneEdge = Vector3(.5f, .5f, .5f);
		Vector3 ptOnTwoEdge = Vector3(.5f, .5f, .5f);
		for (unsigned i = 0; i < 3; i++)
		{
			if (i == oneAxisIndex) ptOnOneEdge[i] = 0;
			else if (first->getTransformMatrix().getAxisVector(i) * axis > 0) ptOnOneEdge[i] = -ptOnOneEdge[i];

			if (i == twoAxisIndex) ptOnTwoEdge[i] = 0;
			else if (other->getTransformMatrix().getAxisVector(i) * axis < 0) ptOnTwoEdge[i] = -ptOnTwoEdge[i];
		}

		// Move them into world coordinates (they are already oriented
		// correctly, since they have been derived from the axes).
		ptOnOneEdge = first->getTransformMatrix() * ptOnOneEdge;
		ptOnTwoEdge = other->getTransformMatrix() * ptOnTwoEdge;

		// So we have a point and a direction for the colliding edges.
		// We need to find out point of closest approach of the two
		// line-segments.
		Vector3 vertex = contactPointCalculate(
			ptOnOneEdge, oneAxis, .5f,
			ptOnTwoEdge, twoAxis, .5f,
			bestSingleAxis > 2
			);

		// Generate a new collision and add it to the list
		Collision newCollision;

		newCollision.penetration = pen;
		newCollision.contactNormal = axis;
		newCollision.contactPoint = vertex;
		newCollision.firstObject = first;
		newCollision.secondObject = other;
		newCollision.friction = 0.9f;

		collisionList->push_back(newCollision);
		return 1;
	}
	return 0;
}

void fillPointFaceBoxBox(
	RigidBody *one,
	RigidBody *two,
	const Vector3 &toCentre,
	std::vector<Collision> *data,
	unsigned best,
	real pen
	)
{
	// This method is called when we know that a vertex from
	// box two is in contact with box one.

	//Contact* contact = data->contacts;
	Collision newCollision;

	// We know which axis the collision is on (i.e. best),
	// but we need to work out which of the two faces on
	// this axis.
	Vector3 normal = one->getTransformMatrix().getAxisVector(best);
	if (one->getTransformMatrix().getAxisVector(best) * toCentre > 0)
	{
		normal = normal * -1.0f;
	}

	// Work out which vertex of box two we're colliding with.
	// Using toCentre doesn't work!
	Vector3 vertex = Vector3(.5f, .5f, .5f);
	Vector3 thisAxisVector = two->getTransformMatrix().getAxisVector(0);
	real testValue = thisAxisVector * normal;
	if (two->getTransformMatrix().getAxisVector(0) * normal < 0) vertex.x = -vertex.x;
	if (two->getTransformMatrix().getAxisVector(1) * normal < 0) vertex.y = -vertex.y;
	if (two->getTransformMatrix().getAxisVector(2) * normal < 0) vertex.z = -vertex.z;

	// Create the contact data
	newCollision.contactNormal = normal;
	newCollision.penetration = pen;
	newCollision.contactPoint = two->getTransformMatrix() * vertex;
	newCollision.firstObject = one;
	newCollision.secondObject = two;
	newCollision.friction = 0.9f;
	// Now add the new contact to the list
	data->push_back(newCollision);
}

inline Vector3 contactPointCalculate(
	const Vector3 &pOne,
	const Vector3 &dOne,
	real oneSize,
	const Vector3 &pTwo,
	const Vector3 &dTwo,
	real twoSize,

	// If this is true, and the contact point is outside
	// the edge (in the case of an edge-face contact) then
	// we use one's midpoint, otherwise we use two's.
	bool useOne)
{
	Vector3 toSt, cOne, cTwo;
	real dpStaOne, dpStaTwo, dpOneTwo, smOne, smTwo;
	real denom, mua, mub;

	smOne = dOne.squareMagnitude();
	smTwo = dTwo.squareMagnitude();
	dpOneTwo = dTwo * dOne;

	toSt = pOne - pTwo;
	dpStaOne = dOne * toSt;
	dpStaTwo = dTwo * toSt;

	denom = smOne * smTwo - dpOneTwo * dpOneTwo;

	// Zero denominator indicates parrallel lines
	if (real_abs(denom) < 0.0001f) {
		return useOne ? pOne : pTwo;
	}

	mua = (dpOneTwo * dpStaTwo - smTwo * dpStaOne) / denom;
	mub = (smOne * dpStaTwo - dpOneTwo * dpStaOne) / denom;

	// If either of the edges has the nearest point out
	// of bounds, then the edges aren't crossed, we have
	// an edge-face contact. Our point is on the edge, which
	// we know from the useOne parameter.
	if (mua > oneSize ||
		mua < -oneSize ||
		mub > twoSize ||
		mub < -twoSize)
	{
		return useOne ? pOne : pTwo;
	}
	else
	{
		cOne = pOne + dOne * mua;
		cTwo = pTwo + dTwo * mub;

		return cOne * 0.5 + cTwo * 0.5;
	}
}



void Collision::resolveContacts(std::vector<Collision> *collisionList, real duration)
{
	// Prepare the contacts for processing
	prepareContacts(collisionList, duration);

	// Resolve the interpenetration problems with the contacts.
	adjustPositions(collisionList, duration);

	// Resolve the velocity problems with the contacts.
	adjustVelocities(collisionList, duration);
}

void Collision::prepareContacts(std::vector<Collision> *collisionList, real duration)
{
	// Generate contact velocity and axis information.
	for (int collisionIndex = 0; collisionIndex < collisionList->size(); collisionIndex++)
	{
		// Calculate the internal contact data (inertia, basis, etc).
		(*collisionList)[collisionIndex].calculateInternals(duration);
	}
}

void Collision::adjustVelocities(std::vector<Collision> *collisionList, real duration)
{
	Vector3 velocityChange[2], rotationChange[2];
	Vector3 deltaVel;

	// iteratively handle impacts in order of severity.
	int velocityIterationsUsed = 0;
	int velocityIterations = 10;
	while (velocityIterationsUsed < velocityIterations)
	{
		// Find contact with maximum magnitude of probable velocity change.
		real max = .01f;
		unsigned index = collisionList->size();
		for (unsigned i = 0; i < collisionList->size(); i++)
		{
			if ((*collisionList)[i].desiredDeltaVelocity > max)
			{
				max = (*collisionList)[i].desiredDeltaVelocity;
				index = i;
			}
		}
		if (index == collisionList->size()) break;

		//// Match the awake state at the contact
		//(*collisionList)[index].matchAwakeState();

		// Do the resolution on the contact that came out top.
		(*collisionList)[index].applyVelocityChange(velocityChange, rotationChange);

		// With the change in velocity of the two bodies, the update of
		// contact velocities means that some of the relative closing
		// velocities need recomputing.
		for (unsigned i = 0; i < collisionList->size(); i++)
		{
			// Check each body in the contact
			for (unsigned b = 0; b < 2; b++)
			{
				// Check for a match with each body in the newly
				// resolved contact
				for (unsigned d = 0; d < 2; d++)
				{
					RigidBody *firstRigidBody = NULL;
					if (b == 0)
					{
						firstRigidBody = (*collisionList)[i].firstObject;
					}
					else
					{
						firstRigidBody = (*collisionList)[i].secondObject;
					}

					RigidBody *secondRigidBody = NULL;
					if (d == 0)
					{
						secondRigidBody = (*collisionList)[index].firstObject;
					}
					else
					{
						secondRigidBody = (*collisionList)[index].secondObject;
					}


					if (firstRigidBody == secondRigidBody)
					{
						deltaVel = velocityChange[d] +
							rotationChange[d].vectorProduct(
							(*collisionList)[i].relativeContactPosition[b]);

						// The sign of the change is negative if we're dealing
						// with the second body in a contact.
						(*collisionList)[i].contactVelocity +=
							(*collisionList)[i].contactToWorld.transformTranspose(deltaVel)
							* (b ? -1 : 1);
						(*collisionList)[i].calculateDesiredDeltaVelocity(duration);
					}
				}
			}
		}
		velocityIterationsUsed++;
	}
}

void Collision::adjustPositions(std::vector<Collision> *collisionList, real duration)
{
	unsigned i, index;
	Vector3 linearChange[2], angularChange[2];
	real max;
	Vector3 deltaPosition;

	// iteratively resolve interpenetrations in order of severity.
	int positionIterationsUsed = 0;
	int positionIterations = 10;
	while (positionIterationsUsed < positionIterations)
	{
		// Find biggest penetration
		max = .01;
		index = collisionList->size();
		for (i = 0; i < collisionList->size(); i++)
		{
			if ((*collisionList)[i].penetration > max)
			{
				max = (*collisionList)[i].penetration;
				index = i;
			}
		}
		if (index == collisionList->size()) break;

		//// Match the awake state at the contact
		//(*collisionList)[index].matchAwakeState();

		// Resolve the penetration.
		(*collisionList)[index].applyPositionChange(
			linearChange,
			angularChange,
			max);

		// Again this action may have changed the penetration of other
		// bodies, so we update contacts.
		for (i = 0; i < collisionList->size(); i++)
		{
			// Check each body in the contact
			for (unsigned b = 0; b < 2; b++)
			{
				// Check for a match with each body in the newly
				// resolved contact
				for (unsigned d = 0; d < 2; d++)
				{
					RigidBody *firstBody = NULL;
					if (b == 0)
					{
						firstBody = (*collisionList)[i].firstObject;
					}
					else
					{
						firstBody = (*collisionList)[i].secondObject;
					}

					RigidBody *secondBody = NULL;
					if (d == 0)
					{
						secondBody = (*collisionList)[index].firstObject;
					}
					else
					{
						secondBody = (*collisionList)[index].secondObject;
					}


					if (firstBody == secondBody)
					{
						deltaPosition = linearChange[d] +
							angularChange[d].vectorProduct(
							(*collisionList)[i].relativeContactPosition[b]);

						// The sign of the change is positive if we're
						// dealing with the second body in a contact
						// and negative otherwise (because we're
						// subtracting the resolution)..
						(*collisionList)[i].penetration +=
							deltaPosition.scalarProduct((*collisionList)[i].contactNormal)
							* (b ? 1 : -1);
					}
				}
			}
		}
		positionIterationsUsed++;
	}
}

void Collision::calculateInternals(real duration)
{
	//// Check if the first object is NULL, and swap if it is.
	//if (!body[0]) swapBodies();
	//assert(body[0]);

	// Calculate an set of axis at the contact point.
	calculateContactBasis();

	// Store the relative position of the contact relative to each body
	relativeContactPosition[0] = contactPoint - firstObject->getPosition();
	if (secondObject) {
		relativeContactPosition[1] = contactPoint - secondObject->getPosition();
	}

	// Find the relative velocity of the bodies at the contact point.
	contactVelocity = calculateLocalVelocity(0, duration);
	if (secondObject) {
		contactVelocity -= calculateLocalVelocity(1, duration);
	}

	// Calculate the desired change in velocity for resolution
	calculateDesiredDeltaVelocity(duration);
}

inline void Collision::calculateContactBasis()
{
	Vector3 contactTangent[2];

	// Check whether the Z-axis is nearer to the X or Y axis
	if (real_abs(contactNormal.x) > real_abs(contactNormal.y))
	{
		// Scaling factor to ensure the results are normalised
		const real s = (real)1.0f / real_sqrt(contactNormal.z*contactNormal.z +
			contactNormal.x*contactNormal.x);

		// The new X-axis is at right angles to the world Y-axis
		contactTangent[0].x = contactNormal.z*s;
		contactTangent[0].y = 0;
		contactTangent[0].z = -contactNormal.x*s;

		// The new Y-axis is at right angles to the new X- and Z- axes
		contactTangent[1].x = contactNormal.y*contactTangent[0].x;
		contactTangent[1].y = contactNormal.z*contactTangent[0].x -
			contactNormal.x*contactTangent[0].z;
		contactTangent[1].z = -contactNormal.y*contactTangent[0].x;
	}
	else
	{
		// Scaling factor to ensure the results are normalised
		const real s = (real)1.0 / real_sqrt(contactNormal.z*contactNormal.z +
			contactNormal.y*contactNormal.y);

		// The new X-axis is at right angles to the world X-axis
		contactTangent[0].x = 0;
		contactTangent[0].y = -contactNormal.z*s;
		contactTangent[0].z = contactNormal.y*s;

		// The new Y-axis is at right angles to the new X- and Z- axes
		contactTangent[1].x = contactNormal.y*contactTangent[0].z -
			contactNormal.z*contactTangent[0].y;
		contactTangent[1].y = -contactNormal.x*contactTangent[0].z;
		contactTangent[1].z = contactNormal.x*contactTangent[0].y;
	}

	// Make a matrix from the three vectors.
	contactToWorld.setComponents(
		contactNormal,
		contactTangent[0],
		contactTangent[1]);
}

Vector3 Collision::calculateLocalVelocity(unsigned bodyIndex, real duration)
{
	RigidBody *thisBody = NULL;
	if (bodyIndex == 0)
	{
		thisBody = firstObject;
	}
	else
	{
		thisBody = secondObject;
	}

	// Work out the velocity of the contact point.
	Vector3 velocity =
		thisBody->getRotation() % relativeContactPosition[bodyIndex];
	velocity += thisBody->getVelocity();

	// Turn the velocity into contact-coordinates.
	Vector3 contactVelocity = contactToWorld.transformTranspose(velocity);

	// Calculate the ammount of velocity that is due to forces without
	// reactions.
	Vector3 accVelocity = thisBody->getLastFrameAcceleration() * duration;

	// Calculate the velocity in contact-coordinates.
	accVelocity = contactToWorld.transformTranspose(accVelocity);

	// We ignore any component of acceleration in the contact normal
	// direction, we are only interested in planar acceleration
	accVelocity.x = 0;

	// Add the planar velocities - if there's enough friction they will
	// be removed during velocity resolution
	contactVelocity += accVelocity;

	// And return it
	return contactVelocity;
}

void Collision::calculateDesiredDeltaVelocity(real duration)
{
	const static real velocityLimit = (real)0.25f;

	// Calculate the acceleration induced velocity accumulated this frame
	real velocityFromAcc = 0;

	//if (body[0]->getAwake())
	//{
	velocityFromAcc +=
		firstObject->getLastFrameAcceleration() * duration * contactNormal;
	//}

	//if (body[1] && body[1]->getAwake())
	//{
	velocityFromAcc -=
		secondObject->getLastFrameAcceleration() * duration * contactNormal;
	//}

	// If the velocity is very slow, limit the restitution
	real thisRestitution = .2;
	if (real_abs(contactVelocity.x) < velocityLimit)
	{
		thisRestitution = (real)0.0f;
	}

	// Combine the bounce velocity with the removed
	// acceleration velocity.
	desiredDeltaVelocity =
		-contactVelocity.x
		- thisRestitution * (contactVelocity.x - velocityFromAcc);
}

/**
* Performs an inertia-weighted impulse based resolution of this
* contact alone.
*/
void Collision::applyVelocityChange(Vector3 velocityChange[2], Vector3 rotationChange[2])
{
	// Get hold of the inverse mass and inverse inertia tensor, both in
	// world coordinates.
	Matrix3 inverseInertiaTensor[2];
	firstObject->getInverseInertiaTensorWorld(&inverseInertiaTensor[0]);
	if (secondObject)
		secondObject->getInverseInertiaTensorWorld(&inverseInertiaTensor[1]);

	// We will calculate the impulse for each contact axis
	Vector3 impulseContact;

	if (friction == (real)0.0)
	{
		// Use the short format for frictionless contacts
		impulseContact = calculateFrictionlessImpulse(inverseInertiaTensor);
	}
	else
	{
		// Otherwise we may have impulses that aren't in the direction of the
		// contact, so we need the more complex version.
		impulseContact = calculateFrictionImpulse(inverseInertiaTensor);
	}

	// Convert impulse to world coordinates
	Vector3 impulse = contactToWorld.transform(impulseContact);

	// Split in the impulse into linear and rotational components
	Vector3 impulsiveTorque = relativeContactPosition[0] % impulse;
	rotationChange[0] = inverseInertiaTensor[0].transform(impulsiveTorque);
	velocityChange[0].clear();
	velocityChange[0].addScaledVector(impulse, firstObject->getInverseMass());

	// Apply the changes
	firstObject->addVelocity(velocityChange[0]);
	firstObject->addRotation(rotationChange[0]);

	if (secondObject)
	{
		// Work out body one's linear and angular changes
		Vector3 impulsiveTorque = impulse % relativeContactPosition[1];
		rotationChange[1] = inverseInertiaTensor[1].transform(impulsiveTorque);
		velocityChange[1].clear();
		velocityChange[1].addScaledVector(impulse, -secondObject->getInverseMass());

		// And apply them.
		secondObject->addVelocity(velocityChange[1]);
		secondObject->addRotation(rotationChange[1]);
	}
}

/**
* Performs an inertia weighted penetration resolution of this
* contact alone.
*/
void Collision::applyPositionChange(Vector3 linearChange[2], Vector3 angularChange[2], real penetration)
{
	const real angularLimit = (real)0.2f;
	real angularMove[2];
	real linearMove[2];

	real totalInertia = 0;
	real linearInertia[2];
	real angularInertia[2];

	// We need to work out the inertia of each object in the direction
	// of the contact normal, due to angular inertia only.
	for (unsigned i = 0; i < 2; i++)
	{
		RigidBody *body = NULL;
		if (i == 0)
		{
			body = firstObject;
		}
		else
		{
			body = secondObject;
		}

		Matrix3 inverseInertiaTensor;
		body->getInverseInertiaTensorWorld(&inverseInertiaTensor);

		// Use the same procedure as for calculating frictionless
		// velocity change to work out the angular inertia.
		Vector3 angularInertiaWorld =
			relativeContactPosition[i] % contactNormal;
		angularInertiaWorld =
			inverseInertiaTensor.transform(angularInertiaWorld);
		angularInertiaWorld =
			angularInertiaWorld % relativeContactPosition[i];
		angularInertia[i] =
			angularInertiaWorld * contactNormal;

		// The linear component is simply the inverse mass
		linearInertia[i] = body->getInverseMass();

		// Keep track of the total inertia from all components
		totalInertia += linearInertia[i] + angularInertia[i];

		// We break the loop here so that the totalInertia value is
		// completely calculated (by both iterations) before
		// continuing.
	}

	// Loop through again calculating and applying the changes
	for (unsigned i = 0; i < 2; i++)
	{
		RigidBody *body = NULL;
		if (i == 0)
		{
			body = firstObject;
		}
		else
		{
			body = secondObject;
		}

		// The linear and angular movements required are in proportion to
		// the two inverse inertias.
		real sign = (i == 0) ? 1 : -1;
		angularMove[i] =
			sign * penetration * (angularInertia[i] / totalInertia);
		linearMove[i] =
			sign * penetration * (linearInertia[i] / totalInertia);

		// To avoid angular projections that are too great (when mass is large
		// but inertia tensor is small) limit the angular move.
		Vector3 projection = relativeContactPosition[i];
		projection.addScaledVector(
			contactNormal,
			-relativeContactPosition[i].scalarProduct(contactNormal)
			);

		// Use the small angle approximation for the sine of the angle (i.e.
		// the magnitude would be sine(angularLimit) * projection.magnitude
		// but we approximate sine(angularLimit) to angularLimit).
		real maxMagnitude = angularLimit * projection.magnitude();

		if (angularMove[i] < -maxMagnitude)
		{
			real totalMove = angularMove[i] + linearMove[i];
			angularMove[i] = -maxMagnitude;
			linearMove[i] = totalMove - angularMove[i];
		}
		else if (angularMove[i] > maxMagnitude)
		{
			real totalMove = angularMove[i] + linearMove[i];
			angularMove[i] = maxMagnitude;
			linearMove[i] = totalMove - angularMove[i];
		}

		// We have the linear amount of movement required by turning
		// the rigid body (in angularMove[i]). We now need to
		// calculate the desired rotation to achieve that.
		if (angularMove[i] == 0)
		{
			// Easy case - no angular movement means no rotation.
			angularChange[i].clear();
		}
		else
		{
			// Work out the direction we'd like to rotate in.
			Vector3 targetAngularDirection =
				relativeContactPosition[i].vectorProduct(contactNormal);

			Matrix3 inverseInertiaTensor;
			body->getInverseInertiaTensorWorld(&inverseInertiaTensor);

			// Work out the direction we'd need to rotate to achieve that
			angularChange[i] =
				inverseInertiaTensor.transform(targetAngularDirection) *
				(angularMove[i] / angularInertia[i]);
		}

		// Velocity change is easier - it is just the linear movement
		// along the contact normal.
		linearChange[i] = contactNormal * linearMove[i];

		// Now we can start to apply the values we've calculated.
		// Apply the linear movement
		Vector3 pos = body->getPosition();
		pos.addScaledVector(contactNormal, linearMove[i]);
		body->setPosition(pos);

		// And the change in orientation
		Quaternion q = body->getOrientation();
		q.addScaledVector(angularChange[i], ((real)1.0));
		body->setOrientation(q);

		// We need to calculate the derived data for any body that is
		// asleep, so that the changes are reflected in the object's
		// data. Otherwise the resolution will not change the position
		// of the object, and the next collision detection round will
		// have the same penetration.
		//if (!body[i]->getAwake()) body[i]->calculateDerivedData();
	}
}

inline Vector3 Collision::calculateFrictionlessImpulse(Matrix3 * inverseInertiaTensor)
{
	Vector3 impulseContact;

	// Build a vector that shows the change in velocity in
	// world space for a unit impulse in the direction of the contact
	// normal.
	Vector3 deltaVelWorld = relativeContactPosition[0] % contactNormal;
	deltaVelWorld = inverseInertiaTensor[0].transform(deltaVelWorld);
	deltaVelWorld = deltaVelWorld % relativeContactPosition[0];

	// Work out the change in velocity in contact coordiantes.
	real deltaVelocity = deltaVelWorld * contactNormal;

	// Add the linear component of velocity change
	deltaVelocity += firstObject->getInverseMass();

	// Check if we need to the second body's data
	if (secondObject)
	{
		// Go through the same transformation sequence again
		Vector3 deltaVelWorld = relativeContactPosition[1] % contactNormal;
		deltaVelWorld = inverseInertiaTensor[1].transform(deltaVelWorld);
		deltaVelWorld = deltaVelWorld % relativeContactPosition[1];

		// Add the change in velocity due to rotation
		deltaVelocity += deltaVelWorld * contactNormal;

		// Add the change in velocity due to linear motion
		deltaVelocity += secondObject->getInverseMass();
	}

	// Calculate the required size of the impulse
	impulseContact.x = desiredDeltaVelocity / deltaVelocity;
	impulseContact.y = 0;
	impulseContact.z = 0;
	return impulseContact;
}

inline Vector3 Collision::calculateFrictionImpulse(Matrix3 * inverseInertiaTensor)
{
	Vector3 impulseContact;
	real inverseMass = firstObject->getInverseMass();

	// The equivalent of a cross product in matrices is multiplication
	// by a skew symmetric matrix - we build the matrix for converting
	// between linear and angular quantities.
	Matrix3 impulseToTorque;
	impulseToTorque.setSkewSymmetric(relativeContactPosition[0]);

	// Build the matrix to convert contact impulse to change in velocity
	// in world coordinates.
	Matrix3 deltaVelWorld = impulseToTorque;
	deltaVelWorld *= inverseInertiaTensor[0];
	deltaVelWorld *= impulseToTorque;
	deltaVelWorld *= -1;

	// Check if we need to add body two's data
	if (secondObject)
	{
		// Set the cross product matrix
		impulseToTorque.setSkewSymmetric(relativeContactPosition[1]);

		// Calculate the velocity change matrix
		Matrix3 deltaVelWorld2 = impulseToTorque;
		deltaVelWorld2 *= inverseInertiaTensor[1];
		deltaVelWorld2 *= impulseToTorque;
		deltaVelWorld2 *= -1;

		// Add to the total delta velocity.
		deltaVelWorld += deltaVelWorld2;

		// Add to the inverse mass
		inverseMass += secondObject->getInverseMass();
	}

	// Do a change of basis to convert into contact coordinates.
	Matrix3 deltaVelocity = contactToWorld.transpose();
	deltaVelocity *= deltaVelWorld;
	deltaVelocity *= contactToWorld;

	// Add in the linear velocity change
	deltaVelocity.data[0] += inverseMass;
	deltaVelocity.data[4] += inverseMass;
	deltaVelocity.data[8] += inverseMass;

	// Invert to get the impulse needed per unit velocity
	Matrix3 impulseMatrix = deltaVelocity.inverse();

	// Find the target velocities to kill
	Vector3 velKill(desiredDeltaVelocity,
		-contactVelocity.y,
		-contactVelocity.z);

	// Find the impulse to kill target velocities
	impulseContact = impulseMatrix.transform(velKill);

	// Check for exceeding friction
	real planarImpulse = real_sqrt(
		impulseContact.y*impulseContact.y +
		impulseContact.z*impulseContact.z
		);
	if (planarImpulse > impulseContact.x * friction)
	{
		// We need to use dynamic friction
		impulseContact.y /= planarImpulse;
		impulseContact.z /= planarImpulse;

		impulseContact.x = deltaVelocity.data[0] +
			deltaVelocity.data[1] * friction*impulseContact.y +
			deltaVelocity.data[2] * friction*impulseContact.z;
		impulseContact.x = desiredDeltaVelocity / impulseContact.x;
		impulseContact.y *= friction * impulseContact.x;
		impulseContact.z *= friction * impulseContact.x;
	}
	return impulseContact;
}
