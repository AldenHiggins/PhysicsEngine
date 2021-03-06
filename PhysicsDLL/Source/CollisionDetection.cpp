#include "CollisionDetection.h"
#include <assert.h>

#define CHECK_OVERLAP(axis, index) \
    if (!tryAxis(firstRect, otherRect, (axis), toCentre, (index), pen, best)) return 0;

using namespace PhysicsEngine;

// Determine if there is a collision between two cubic rigid bodies
unsigned int CollisionDetection::cubeCubeCollisionDetect
(
	std::vector<Collision> *collisionList,
	CollisionBox *firstRect,
	CollisionBox *otherRect
)
{
	//if (!IntersectionTests::boxAndBox(one, two)) return 0;

	// Find the vector between the two centres
	//Vector3 toCentre = two.getAxis(3) - one.getAxis(3);
	RigidBody *first = firstRect->body;
	RigidBody *other = otherRect->body;
	Vector3 toCentre = other->getTransformMatrix().getAxisVector(3) - first->getTransformMatrix().getAxisVector(3);

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
		fillPointFaceBoxBox(firstRect, otherRect, toCentre, collisionList, best, pen);
		return 1;
	}
	else if (best < 6)
	{
		// We've got a vertex of box one on a face of box two.
		// We use the same algorithm as above, but swap around
		// one and two (and therefore also the vector between their
		// centres).
		fillPointFaceBoxBox(otherRect, firstRect, toCentre*-1.0f, collisionList, best - 3, pen);
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
		Vector3 ptOnOneEdge = firstRect->halfSize;
		Vector3 ptOnTwoEdge = otherRect->halfSize;
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
		Vector3 vertex = contactPointCalculate
		(
			ptOnOneEdge, oneAxis, firstRect->halfSize[oneAxisIndex],
			ptOnTwoEdge, twoAxis, otherRect->halfSize[twoAxisIndex],
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

// Find collisions between a cube and a plane
unsigned CollisionDetection::boxAndHalfSpaceCollisionDetect
(
	CollisionBox *box,
	CollisionPlane *plane,
	std::vector<Collision> *collisionList
)
{
	Vector3 planeDirection = plane->normal;
	real planeOffset = plane->offset;
	//// Make sure we have contacts
	//if (data->contactsLeft <= 0) return 0

	// Check for intersection
	if (!boxAndHalfSpaceIntersect(box, planeDirection, planeOffset))
	{
		return 0;
	}

	// We have an intersection, so find the intersection points. We can make
	// do with only checking vertices. If the box is resting on a plane
	// or on an edge, it will be reported as four or two contact points.

	// Go through each combination of + and - for each half-size
	static real mults[8][3] = { { 1, 1, 1 }, { -1, 1, 1 }, { 1, -1, 1 }, { -1, -1, 1 },
	{ 1, 1, -1 }, { -1, 1, -1 }, { 1, -1, -1 }, { -1, -1, -1 } };

	unsigned contactsUsed = 0;
	for (unsigned i = 0; i < 8; i++)
	{
		// Calculate the position of each vertex
		Vector3 vertexPos(mults[i][0], mults[i][1], mults[i][2]);
		vertexPos.componentProductUpdate(box->halfSize);
		vertexPos = box->body->getTransformMatrix().transform(vertexPos);

		// Calculate the distance from the plane
		real vertexDistance = vertexPos * planeDirection;

		// Compare this to the plane's distance
		if (vertexDistance <= planeOffset)
		{
			// Create the contact data.
			// The contact point is halfway between the vertex and the
			// plane - we multiply the direction by half the separation
			// distance and add the vertex location.
			Collision newCollision;
			newCollision.contactPoint = planeDirection;
			newCollision.contactPoint *= (vertexDistance - planeOffset);
			newCollision.contactPoint += vertexPos;
			newCollision.contactNormal = planeDirection;
			newCollision.penetration = planeOffset - vertexDistance;
			newCollision.firstObject = box->body;
			newCollision.secondObject = NULL;
			newCollision.friction = 0.9f;

			// Add the new collision to the list
			contactsUsed++;
			collisionList->push_back(newCollision);
		}
	}
	return contactsUsed;
}

// Determine if there is a collision between two spheres
unsigned CollisionDetection::sphereAndHalfSpaceCollisionDetect
(
	CollisionSphere *sphere,
	CollisionPlane *plane,
	std::vector<Collision> *collisionList
)
{
	Vector3 planeDirection = plane->normal;
	real planeOffset = plane->offset;

	real sphereDistance = sphere->body->getPosition() * planeDirection - sphere->radius - planeOffset;
	// If the distance is greater than zero we don't have a collision
	if (sphereDistance >= 0)
	{
		return 0;
	}

	Collision newCollision;
	newCollision.contactPoint = sphere->body->getPosition() - planeDirection * (sphereDistance + sphere->radius);
	newCollision.contactNormal = planeDirection;
	newCollision.penetration = -sphereDistance;
	newCollision.firstObject = sphere->body;
	newCollision.secondObject = NULL;
	newCollision.friction = 0.9f;

	collisionList->push_back(newCollision);
	return 1;
}

// Determine if there is a collision between two spheres
unsigned CollisionDetection::capsuleHalfSpaceCollisionDetect
(
	CollisionCapsule *capsule,
	CollisionPlane *plane,
	std::vector<Collision> *collisionList
)
{
	Vector3 planeDirection = plane->normal;
	real planeOffset = plane->offset;

	// Consider the capsule as the two spheres at either end of the capsule
	// Test the sphere at the top of the capsule first
	Vector3 topSpherePosition = capsule->body->getPointInWorldSpace(Vector3(0, capsule->height, 0));
	real sphereDistance = topSpherePosition * planeDirection - capsule->radius - planeOffset;
	// If the distance is greater than zero we don't have a collision
	if (sphereDistance < 0)
	{
		Collision newCollision;
		newCollision.contactPoint = topSpherePosition - planeDirection * (sphereDistance + capsule->radius);
		newCollision.contactNormal = planeDirection;
		newCollision.penetration = -sphereDistance;
		newCollision.firstObject = capsule->body;
		newCollision.secondObject = NULL;
		newCollision.friction = 0.9f;

		collisionList->push_back(newCollision);
	}

	// Consider the capsule as the two spheres at either end of the capsule
	// Test the sphere at the top of the capsule first
	Vector3 bottomSpherePosition = capsule->body->getPointInWorldSpace(Vector3(0, -1 * capsule->height, 0));
	sphereDistance = bottomSpherePosition * planeDirection - capsule->radius - planeOffset;
	// If the distance is greater than zero we don't have a collision
	if (sphereDistance < 0)
	{
		Collision newCollision;
		newCollision.contactPoint = bottomSpherePosition - planeDirection * (sphereDistance + capsule->radius);
		newCollision.contactNormal = planeDirection;
		newCollision.penetration = -sphereDistance;
		newCollision.firstObject = capsule->body;
		newCollision.secondObject = NULL;
		newCollision.friction = 0.9f;

		collisionList->push_back(newCollision);
	}
	
	return 1;
}

// Determine if a capsule has collided with a sphere
unsigned int CollisionDetection::capsuleSphereCollisionDetect
(
	CollisionCapsule *capsule,
	CollisionSphere *sphere,
	std::vector<Collision> *collisionList
)
{
	// Transform the sphere's center into the capsule's coordinate system
	Vector3 capsuleCoordSphereCenter = sphere->body->getPosition();
	capsuleCoordSphereCenter = capsule->body->getTransformMatrix().transformInverse(capsuleCoordSphereCenter);

	// First check if the sphere is within the capsule's y coordinates, if not discard
	real newCapsuleHeight = capsule->height * .5f + sphere->radius;
	if (capsuleCoordSphereCenter.y > newCapsuleHeight || capsuleCoordSphereCenter.y < -1 * newCapsuleHeight)
	{
		return 0;
	}

	Vector3 capsuleCoordSphereCenterNoHeight = capsuleCoordSphereCenter;
	// Remove the y component of the vector
	capsuleCoordSphereCenterNoHeight.y = 0;

	// Check if the sphere is close enough to the capsule to collide
	real penetration = capsuleCoordSphereCenterNoHeight.magnitude() - sphere->radius - capsule->radius;
	if (penetration >= 0)
	{
		return 0;
	}

	Collision newCollision;
	// Really problematic and error prone solution, pretty much scale the vector from teh capsule to the sphere such that the sphere's radius is no longer a factor
	// This neither gets the correct contact point nor will it prevent errors from occuring
	Vector3 contactPoint = capsuleCoordSphereCenter;
	contactPoint = contactPoint * ((capsuleCoordSphereCenter.magnitude() - sphere->radius) / capsuleCoordSphereCenter.magnitude());
	newCollision.contactPoint = capsule->body->getPointInWorldSpace(contactPoint);

	// Contact normal is the vector from the capsule center to the contact point (bad implementation)
	Vector3 contactNormal = capsule->body->getPointInWorldSpace(capsuleCoordSphereCenter);
	contactNormal = contactNormal - capsule->body->getPosition();
	contactNormal.normalise();
	// Reverse the contact normal
	contactNormal[0] *= -1;
	contactNormal[1] *= -1;
	contactNormal[2] *= -1;
	newCollision.contactNormal = contactNormal;
	newCollision.penetration = -penetration;
	newCollision.firstObject = capsule->body;
	newCollision.secondObject = sphere->body;
	newCollision.friction = 0.9f;

	collisionList->push_back(newCollision);
	return 1;
}

// Determine if a capsule has collided with another capsule
unsigned int CollisionDetection::capsuleCapsuleCollisionDetect
(
	CollisionCapsule *first,
	CollisionCapsule *second,
	std::vector<Collision> *collisionList
)
{
	Vector3 firstPoint1 = first->body->getPointInWorldSpace(Vector3(0, (first->height / 2), 0));
	Vector3 firstPoint2 = first->body->getPointInWorldSpace(Vector3(0, (-1 * first->height / 2), 0));

	Vector3 u = firstPoint2 - firstPoint1;
	u.normalise();

	Vector3 secondPoint1 = second->body->getPointInWorldSpace(Vector3(0, (second->height / 2), 0));
	Vector3 secondPoint2 = second->body->getPointInWorldSpace(Vector3(0, (-1 * second->height / 2), 0));

	Vector3 v = secondPoint2 - secondPoint1;
	v.normalise();

	real a = u * u;
	real b = u * v;
	real c = v * v;
	real d = u * (firstPoint1 - secondPoint1);
	real e = v * (firstPoint1 - secondPoint1);

	real denominator = a * c - b * b;
	real minimumSValue = 0;
	real minimumTValue = 0;
	// Check if the lines are parallel beforehand
	if (denominator == 0)
	{
		minimumSValue = .5f;
		minimumTValue = .5f;
	}
	else
	{
		minimumSValue = (b * e - c * d) / denominator;
		minimumTValue = (a * e - b * d) / denominator;
	}

	// Prevent the S and T values from being less than zero
	if (minimumSValue < 0)
	{
		minimumSValue = 0;
	}

	if (minimumTValue < 0)
	{
		minimumTValue = 0;
	}

	// Prevent the S and T values from being greater than 1
	if (minimumSValue > 1)
	{
		minimumSValue = 1;
	}

	if (minimumTValue > 1)
	{
		minimumTValue = 1;
	}


	Vector3 firstCapsulePosition = ((firstPoint2 - firstPoint1) * minimumSValue) + firstPoint1;
	Vector3 secondCapsulePosition = ((secondPoint2 - secondPoint1) * minimumTValue) + secondPoint1;

	Vector3 vectorFromFirstToSecond = secondCapsulePosition - firstCapsulePosition;
	real capsuleDistance = vectorFromFirstToSecond.magnitude();
	capsuleDistance = capsuleDistance - first->radius - second->radius;

	// Check if the distance between the capsules is less than the sum of their radii
	if (capsuleDistance > 0)
	{
		return 0;
	}

	// Flip the contact normal
	vectorFromFirstToSecond.normalise();
	vectorFromFirstToSecond[0] *= -1.0f;
	vectorFromFirstToSecond[1] *= -1.0f;
	vectorFromFirstToSecond[2] *= -1.0f;

	Collision newCollision;
	newCollision.contactPoint = firstCapsulePosition + (vectorFromFirstToSecond * .5);
	newCollision.contactNormal = vectorFromFirstToSecond;
	newCollision.penetration = -1 * capsuleDistance;
	newCollision.firstObject = first->body;
	newCollision.secondObject = second->body;
	newCollision.friction = 0.9f;

	collisionList->push_back(newCollision);
	return 1;
}

#define PARALLEL_EPSILON 0.0001

// Determine if a capsule has collided with a square
unsigned int CollisionDetection::capsuleSquareCollisionDetect
(
	CollisionCapsule *first,
	CollisionBox *second,
	std::vector<Collision> *collisionList
)
{
	// Get the three points involved
	//Vector3 point1 = first->body->getPointInWorldSpace(Vector3(0.0, first->height / 2.0f, 0.0));
	//Vector3 point2 = first->body->getPointInWorldSpace(Vector3(0.0, -1 * first->height / 2.0f, 0.0f));
	//Vector3 point3 = second->body->getPosition();
	//Vector3 pointInCubeSpace = second->body->getTransformMatrix().transformInverse(closestPointOnCapsule);

	Vector3 boxCenter = second->body->getPosition();

	Vector3 capCenterBoxSpace = second->body->getTransformMatrix().transformInverse(first->body->getPosition());
	Vector3 capsuleStart = first->body->getPointInWorldSpace(Vector3(0.0, -1 * first->height / 2.0f, 0.0f));
	Vector3 capsuleStartBoxSpace = second->body->getTransformMatrix().transformInverse(first->body->getPointInWorldSpace(Vector3(0.0, -1 * first->height / 2.0f, 0.0f)));
	Vector3 capsuleEnd = first->body->getPointInWorldSpace(Vector3(0.0, first->height / 2.0f, 0.0f));
	Vector3 capsuleEndBoxSpace = second->body->getTransformMatrix().transformInverse(first->body->getPointInWorldSpace(Vector3(0.0, first->height / 2.0f, 0.0f)));
	Vector3 capsuleSegmentVector = capsuleEndBoxSpace - capsuleStartBoxSpace;
	real maxRayDistance = capsuleSegmentVector.magnitude();
	capsuleSegmentVector.normalise();

	// Expand the box half sizes by the capsules radius
	Vector3 boxExpandedHalfSizes;
	boxExpandedHalfSizes[0] = second->halfSize[0] + first->radius;
	boxExpandedHalfSizes[1] = second->halfSize[1] + first->radius;
	boxExpandedHalfSizes[2] = second->halfSize[2] + first->radius;

	real t = 0.0f;
	
	// Check the three axes
	for (int axisIndex = 0; axisIndex < 3; axisIndex++)
	{
		if (abs(capsuleSegmentVector[axisIndex]) < PARALLEL_EPSILON)
		{
			if (capsuleStartBoxSpace[axisIndex] < -1 * boxExpandedHalfSizes[axisIndex] || capsuleStartBoxSpace[axisIndex] > boxExpandedHalfSizes[axisIndex])
			{
				return 0;
			}
		}
		else
		{
			// Compute intersection t value of ray with near and far plane of box
			real ood = 1.0f / capsuleSegmentVector[axisIndex];
			real t1 = (-1 * boxExpandedHalfSizes[axisIndex] - capsuleStartBoxSpace[axisIndex]) * ood;
			real t2 = (boxExpandedHalfSizes[axisIndex] - capsuleStartBoxSpace[axisIndex]) * ood;
			// Make t1 be intersection with near and t2 with far
			if (t1 > t2)
			{
				real temp = t1;
				t1 = t2;
				t2 = temp;
			}
			// Compute the intersection of slab intersection intervals
			if (t1 > t)
			{
				t = t1;
			}
			if (t2 < maxRayDistance)
			{
				maxRayDistance = t2;
			}

			if (t > maxRayDistance)
			{
				return 0;
			}
		}
	}

	Vector3 intersectionPoint = capsuleStartBoxSpace + capsuleSegmentVector * t;
	// Get rid of the capsule's radius from the calculated intersection point
	for (int axisIndex = 0; axisIndex < 3; axisIndex++)
	{
		if (intersectionPoint[axisIndex] < -1 * first->radius)
		{
			intersectionPoint[axisIndex] += first->radius;
		}
		else if (intersectionPoint[axisIndex] > first->radius)
		{
			intersectionPoint[axisIndex] -= first->radius;
		}
	}

	Collision newCollision;
	newCollision.contactPoint = second->body->getPointInWorldSpace(intersectionPoint);

	// Contact the underpants gnomes for this one
	if (second->halfSize[0] > first->height || second->halfSize[1] > first->height || second->halfSize[2] > first->height)
	{
		Vector3 axes[] = 
		{
			Vector3(1, 0, 0), Vector3(-1, 0, 0),
			Vector3(0, 1, 0), Vector3(0, -1, 0),
			Vector3(0, 0, 1), Vector3(0, 0, -1)
		};

		real axesDistances[] =
		{
			boxExpandedHalfSizes[0] - intersectionPoint[0], abs(-1 * boxExpandedHalfSizes[0] - intersectionPoint[0]),
			boxExpandedHalfSizes[1] - intersectionPoint[1], abs(-1 * boxExpandedHalfSizes[1] - intersectionPoint[1]),
			boxExpandedHalfSizes[2] - intersectionPoint[2], abs(-1 * boxExpandedHalfSizes[2] - intersectionPoint[2])
		};

		int closestAxisIndex = 0;
		real lowestDistance = 10000;

		for (int axisIndex = 0; axisIndex < 6; axisIndex++)
		{
			if (axesDistances[axisIndex] < lowestDistance)
			{
				closestAxisIndex = axisIndex;
				lowestDistance = axesDistances[axisIndex];
			}
		}

		newCollision.contactNormal = axes[closestAxisIndex];
		newCollision.penetration = abs(first->radius - lowestDistance);
	}
	// Case where the box is small enough that we don't have to get fancy with the contact normal
	else
	{
		Vector3 contactNormal = intersectionPoint;
		contactNormal.normalise();
		newCollision.contactNormal = contactNormal;
		newCollision.penetration = .1f;
	}
	
	newCollision.firstObject = first->body;
	newCollision.secondObject = second->body;
	newCollision.friction = 0.9f;

	collisionList->push_back(newCollision);

	return 1;
}


// Determine if two spheres have collided
unsigned CollisionDetection::sphereSphereCollisionDetect
(
	CollisionSphere *first,
	CollisionSphere *other,
	std::vector<Collision> *collisionList
)
{
	Vector3 toCenter = first->body->getPosition() - other->body->getPosition();
	// If the distance between the two spheres is greater than their radii there isn't a collision
	real centerMagnitude = toCenter.magnitude();
	real penetration = (first->radius + other->radius) - centerMagnitude;
	if (penetration < 0)
	{
		return 0;
	}

	Collision newCollision;
	newCollision.contactPoint = other->body->getPosition() + toCenter * .5f;
	Vector3 contactNormal = toCenter;
	contactNormal.normalise();
	newCollision.contactNormal = contactNormal;
	newCollision.penetration = penetration;
	newCollision.firstObject = first->body;
	newCollision.secondObject = other->body;
	newCollision.friction = 0.9f;

	collisionList->push_back(newCollision);
	return 1;
}

// Determine if a sphere has collided with a cube
unsigned CollisionDetection::sphereCubeCollisionDetect
(
	CollisionSphere *sphere,
	CollisionBox *cube,
	std::vector<Collision> *collisionList
)
{
	// Transform the centre of the sphere into box coordinates
	Vector3 centre = sphere->body->getTransformMatrix().getAxisVector(3);
	Vector3 relCentre = cube->body->getTransformMatrix().transformInverse(centre);

	// Early out check to see if we can exclude the contact
	if (real_abs(relCentre.x) - sphere->radius > cube->halfSize.x ||
		real_abs(relCentre.y) - sphere->radius > cube->halfSize.y ||
		real_abs(relCentre.z) - sphere->radius >  cube->halfSize.z)
	{
		return 0;
	}

	Vector3 closestPt(0, 0, 0);
	real dist;

	// Clamp each coordinate to the box.
	dist = relCentre.x;
	if (dist > cube->halfSize.x) dist = cube->halfSize.x;
	if (dist < -cube->halfSize.x) dist = -cube->halfSize.x;
	closestPt.x = dist;

	dist = relCentre.y;
	if (dist > cube->halfSize.y) dist = cube->halfSize.y;
	if (dist < -cube->halfSize.y) dist = -cube->halfSize.y;
	closestPt.y = dist;

	dist = relCentre.z;
	if (dist > cube->halfSize.z) dist = cube->halfSize.z;
	if (dist < -cube->halfSize.z) dist = -cube->halfSize.z;
	closestPt.z = dist;

	// Check we're in contact
	dist = (closestPt - relCentre).squareMagnitude();
	if (dist > sphere->radius * sphere->radius) return 0;

	// Compile the contact
	Vector3 closestPtWorld = cube->body->getTransformMatrix().transform(closestPt);

	Collision newCollision;
	newCollision.contactPoint = closestPtWorld;
	newCollision.contactNormal = (closestPtWorld - centre);
	newCollision.contactNormal.normalise();
	newCollision.penetration = sphere->radius - real_sqrt(dist);
	newCollision.firstObject = cube->body;
	newCollision.secondObject = sphere->body;
	newCollision.friction = 0.9f;

	collisionList->push_back(newCollision);
	return 1;
}

bool CollisionDetection::boxAndHalfSpaceIntersect
(
	const CollisionBox *box,
	Vector3 planeDirection,
	real planeOffset
)
{
	// Work out the projected radius of the box onto the plane direction
	real projectedRadius = transformToAxis(box, planeDirection);

	// Work out how far the box is from the origin
	real boxDistance =
		planeDirection *
		box->body->getPosition() -
		projectedRadius;

	// Check for the intersection
	return boxDistance <= planeOffset;
}

inline real CollisionDetection::transformToAxis
(
	const CollisionBox *box,
	const Vector3 &axis
)
{
	return
		box->halfSize[0] * real_abs(axis * box->body->getTransformMatrix().getAxisVector(0)) +
		box->halfSize[1] * real_abs(axis * box->body->getTransformMatrix().getAxisVector(1)) +
		box->halfSize[2] * real_abs(axis * box->body->getTransformMatrix().getAxisVector(2));
}

inline bool CollisionDetection::tryAxis
(
	const CollisionBox *one,
	const CollisionBox *two,
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
inline real CollisionDetection::penetrationOnAxis
(
	const CollisionBox *one,
	const CollisionBox *two,
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

void CollisionDetection::fillPointFaceBoxBox
(
	CollisionBox *one,
	CollisionBox *two,
	const Vector3 &toCentre,
	std::vector<Collision> *data,
	unsigned best,
	real pen
)
{
	// This method is called when we know that a vertex from
	// box two is in contact with box one.

	//Contact* contact = data->contacts;

	// We know which axis the collision is on (i.e. best),
	// but we need to work out which of the two faces on
	// this axis.
	Vector3 normal = one->body->getTransformMatrix().getAxisVector(best);
	if (one->body->getTransformMatrix().getAxisVector(best) * toCentre > 0)
	{
		normal = normal * -1.0f;
	}

	// Work out which vertex of box two we're colliding with.
	// Using toCentre doesn't work!
	Vector3 vertex = two->halfSize;
	if (two->body->getTransformMatrix().getAxisVector(0) * normal < 0) vertex.x = -vertex.x;
	if (two->body->getTransformMatrix().getAxisVector(1) * normal < 0) vertex.y = -vertex.y;
	if (two->body->getTransformMatrix().getAxisVector(2) * normal < 0) vertex.z = -vertex.z;

	// Create the contact data
	Collision newCollision;
	newCollision.contactNormal = normal;
	newCollision.penetration = pen;
	newCollision.contactPoint = two->body->getTransformMatrix() * vertex;
	newCollision.firstObject = one->body;
	newCollision.secondObject = two->body;
	newCollision.friction = 0.9f;
	// Now add the new contact to the list
	data->push_back(newCollision);
}

inline Vector3 CollisionDetection::contactPointCalculate
(
	const Vector3 &pOne,
	const Vector3 &dOne,
	real oneSize,
	const Vector3 &pTwo,
	const Vector3 &dTwo,
	real twoSize,

	// If this is true, and the contact point is outside
	// the edge (in the case of an edge-face contact) then
	// we use one's midpoint, otherwise we use two's.
	bool useOne
)
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

	// Zero denominator indicates parallel lines
	if (real_abs(denom) < 0.0001f)
	{
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
