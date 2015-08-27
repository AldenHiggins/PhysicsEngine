#include "RenderableObjects.h"
#include <gl/glut.h>

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