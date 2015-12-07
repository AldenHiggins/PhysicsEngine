#include "RenderableObjects.h"

using namespace PhysicsEngine;

// Display this rectangle
void RectangleObject::display()
{
	// Get the OpenGL transformation
	GLfloat mat[16];
	this->body->getGLTransform(mat);
	glColor3f(.5f, .1f, .9f);

	glPushMatrix();
	glMultMatrixf(mat);
	glScalef(halfSize[0] * 2, halfSize[1] * 2, halfSize[2] * 2);
	glutSolidCube(1.0f);
	glPopMatrix();
}

// Display this sphere
void SphereObject::display()
{
	// Get the OpenGL transformation
	GLfloat mat[16];
	this->body->getGLTransform(mat);
	glColor3f(.7f, .5f, .1f);

	glPushMatrix();
	glMultMatrixf(mat);
	glutSolidSphere(radius, SPHERE_SLICES, SPHERE_STACKS);
	glPopMatrix();
}

// Generate a new capsule object
CapsuleObject::CapsuleObject()
{
	// Generate the rigid body
	body = new RigidBody();

	quadricObject = gluNewQuadric();
	gluQuadricNormals(quadricObject, GLU_SMOOTH);
	gluQuadricDrawStyle(quadricObject, GLU_FILL);
}

CapsuleObject::~CapsuleObject()
{
	gluDeleteQuadric(quadricObject);
	delete body;
}

// Display the capsule
void CapsuleObject::display()
{
	// Get the OpenGL transformation
	GLfloat mat[16];
	this->body->getGLTransform(mat);
	glColor3f(.5f, .1f, .9f);

	glPushMatrix();
	glMultMatrixf(mat);
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	gluCylinder(quadricObject, 1.0, 1.0, 1.0, 10, 16);
	glPopMatrix();
}