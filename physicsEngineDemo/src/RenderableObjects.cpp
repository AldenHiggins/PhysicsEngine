#include "RenderableObjects.h"

using namespace PhysicsDemo;

void Axis::display()
{
	glBegin(GL_QUADS);
	// Draw the lines of the axes
	glColor3f(color[0], color[1], color[2]);
	glVertex3f(firstPoint[0], firstPoint[1], firstPoint[2]);
	glVertex3f(secondPoint[0], secondPoint[1], secondPoint[2]);
	glVertex3f(thirdPoint[0], thirdPoint[1], thirdPoint[2]);
	glVertex3f(fourthPoint[0], fourthPoint[1], fourthPoint[2]);
	glEnd();
}

// Display this plane
void Plane::display()
{
	glBegin(GL_QUADS);
	glColor3f(color[0], color[1], color[2]);
	glNormal3f(normal[0], normal[1], normal[2]);


	PhysicsEngine::Vector3 right = up.vectorProduct(normal);

	PhysicsEngine::real newHalfSize = halfSize;
	PhysicsEngine::Vector3 variableColor = color;
	for (int quadIndex = 0; quadIndex < 50; quadIndex++)
	{
		variableColor[0] += .1f;
		glColor3f(variableColor[0], variableColor[1], variableColor[2]);
		PhysicsEngine::Vector3 position1 = position + (up * newHalfSize) + (right * newHalfSize) + (normal * quadIndex * .01f);
		PhysicsEngine::Vector3 position2 = position + (up * newHalfSize) - (right * newHalfSize) + (normal * quadIndex * .01f);
		PhysicsEngine::Vector3 position3 = position - (up * newHalfSize) - (right * newHalfSize) + (normal * quadIndex * .01f);
		PhysicsEngine::Vector3 position4 = position - (up * newHalfSize) + (right * newHalfSize) + (normal * quadIndex * .01f);

		glVertex3f(position1[0], position1[1], position1[2]);
		glVertex3f(position2[0], position2[1], position2[2]);
		glVertex3f(position3[0], position3[1], position3[2]);
		glVertex3f(position4[0], position4[1], position4[2]);

		newHalfSize = newHalfSize / 1.1;
	}
	glEnd();
}

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
