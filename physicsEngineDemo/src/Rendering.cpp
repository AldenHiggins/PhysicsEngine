#include "PhysicsDemo.h"
#include "Rendering.h"

using namespace PhysicsDemo;

void Rendering::drawBox(GLfloat transform[16], Vector3 color, Vector3 halfSizes)
{
	// Get the OpenGL transformation
	glColor3f((float)color[0], (float)color[1], (float)color[2]);

	glPushMatrix();
	glMultMatrixf(transform);
	glScalef((float)(halfSizes[0] * 2), (float)(halfSizes[1] * 2), (float)(halfSizes[2] * 2));
	glutSolidCube(1.0f);
	glPopMatrix();
}

void Rendering::drawSphere(Vector3 position, Vector3 color, real radius)
{
	// Render the collision position
	glColor3f((float)color[0], (float)color[1], (float)color[2]);
	glPushMatrix();
	glTranslatef((float)position[0], (float)position[1], (float)position[2]);
	glutSolidSphere(radius, SPHERE_SLICES, SPHERE_STACKS);
	glPopMatrix();
}

void Rendering::drawCapsule(GLfloat transform[16], Vector3 color, real height, real radius)
{
	// TODO: Get rid of this, this is terrible
	GLUquadric *quadricObject = gluNewQuadric();
	gluQuadricNormals(quadricObject, GLU_SMOOTH);
	gluQuadricDrawStyle(quadricObject, GLU_FILL);

	glColor3f((float)color[0], (float)color[1], (float)color[2]);

	glPushMatrix();
	glMultMatrixf(transform);

	glTranslatef(0.0f, (float)(.5f * height), 0.0f);
	glutSolidSphere(radius, 50, 50);
	glTranslatef(0.0f, (float)(-1.0f * height), 0.0f);
	glutSolidSphere((float)(radius), 50, 50);
	glTranslatef(0.0f, (float)(height), 0.0f);
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	gluCylinder(quadricObject, radius, radius, height, 100, 100);

	glPopMatrix();

	gluDeleteQuadric(quadricObject);
}
	