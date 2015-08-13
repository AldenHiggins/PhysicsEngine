#include "Collisions.h"
#include <assert.h>

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
    tryAxis(first, other, (axis), toCentre, (index), pen, best);

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
	Vector3 toCentre = first->getPosition() - other->getPosition();

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
	if (two->getTransformMatrix().getAxisVector(0) * normal < 0) vertex.x = -vertex.x;
	if (two->getTransformMatrix().getAxisVector(1) * normal < 0) vertex.y = -vertex.y;
	if (two->getTransformMatrix().getAxisVector(2) * normal < 0) vertex.z = -vertex.z;

	// Create the contact data
	newCollision.contactNormal = normal;
	newCollision.penetration = pen;
	newCollision.contactPoint = two->getTransformMatrix() * vertex;
	newCollision.firstObject = one;
	newCollision.secondObject = two;
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
