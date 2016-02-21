#include "PhysicsDemo.h"
#include "Rendering.h"

using namespace PhysicsDemo;

void Rendering::drawBox(GLfloat mat[16], Vector3 color, Vector3 halfSizes)
{
	// Get the OpenGL transformation
	glColor3f((float)color[0], (float)color[1], (float)color[2]);

	glPushMatrix();
	glMultMatrixf(mat);
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
	

//// Display this rectangle
//void Box::display()
//{
//	// Get the OpenGL transformation
//	GLfloat mat[16];
//	this->boxPrimitive->body->getGLTransform(mat);
//	glColor3f((float)color[0], (float)color[1], (float)color[2]);

//	glPushMatrix();
//	glMultMatrixf(mat);
//	glScalef((float)(this->boxPrimitive->halfSize[0] * 2), (float)(this->boxPrimitive->halfSize[1] * 2), (float)(this->boxPrimitive->halfSize[2] * 2));
//	glutSolidCube(1.0f);
//	glPopMatrix();
//}