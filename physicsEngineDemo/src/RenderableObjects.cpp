#include "RenderableObjects.h"

using namespace PhysicsDemo;

// Display this rectangle
void Box::display()
{
	// Get the OpenGL transformation
	GLfloat mat[16];
	this->boxPrimitive->body->getGLTransform(mat);
	glColor3f(.5f, .1f, .9f);

	glPushMatrix();
	glMultMatrixf(mat);
	glScalef(this->boxPrimitive->halfSize[0] * 2, this->boxPrimitive->halfSize[1] * 2, this->boxPrimitive->halfSize[2] * 2);
	glutSolidCube(1.0f);
	glPopMatrix();
}

// Display this sphere
void Sphere::display()
{
	// Get the OpenGL transformation
	GLfloat mat[16];
	this->spherePrimitive->body->getGLTransform(mat);
	glColor3f(.7f, .5f, .1f);

	glPushMatrix();
	glMultMatrixf(mat);
	glutSolidSphere(this->spherePrimitive->radius, SPHERE_SLICES, SPHERE_STACKS);
	glPopMatrix();
}

// Generate a new capsule object
Capsule::Capsule()
{
	// Generate the rigid body
	capsulePrimitive = new PhysicsEngine::CollisionCapsule();
	quadricObject = gluNewQuadric();
	gluQuadricNormals(quadricObject, GLU_SMOOTH);
	gluQuadricDrawStyle(quadricObject, GLU_FILL);
}

Capsule::~Capsule()
{
	gluDeleteQuadric(quadricObject);
	delete capsulePrimitive;
}

// Display the capsule
void Capsule::display()
{
	// Get the OpenGL transformation
	GLfloat mat[16];
	this->capsulePrimitive->body->getGLTransform(mat);
	glColor3f(.5f, .1f, .9f);

	glPushMatrix();
	glMultMatrixf(mat);
	
	glTranslatef(0.0f, .5f * this->capsulePrimitive->height, 0.0f);
	glutSolidSphere(this->capsulePrimitive->radius, 50, 50);
	glTranslatef(0.0f, -1.0f * this->capsulePrimitive->height, 0.0f);
	glutSolidSphere(this->capsulePrimitive->radius, 50, 50);
	glTranslatef(0.0f, this->capsulePrimitive->height, 0.0f);
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	gluCylinder(quadricObject, this->capsulePrimitive->radius, this->capsulePrimitive->radius, this->capsulePrimitive->height, 100, 100);
	
	glPopMatrix();
}
