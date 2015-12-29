#include "RenderableObjects.h"

using namespace PhysicsEngine;

// Constants used for the inertia tensor calculation
#define PI               3.141592654f
#define PI_TIMES2        6.283185307f

const float oneDiv3 = (float)(1.0 / 3.0);
const float oneDiv8 = (float)(1.0 / 8.0);
const float oneDiv12 = (float)(1.0 / 12.0);
const float density = 1.0f;

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
	
	glTranslatef(0.0f, .5f * height, 0.0f);
	glutSolidSphere(radius, 50, 50);
	glTranslatef(0.0f, -1.0f * height, 0.0f);
	glutSolidSphere(radius, 50, 50);
	glTranslatef(0.0f, height, 0.0f);
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	gluCylinder(quadricObject, radius, radius, height, 100, 100);
	
	glPopMatrix();
}

// Set all of the parameters for this rigid body/Collision Box
void CapsuleObject::setState(Vector3 position, Vector3 velocity, Vector3 acceleration, real mass, real radiusInput, real heightInput)
{
	radius = radiusInput;
	height = heightInput;
	body->setPosition(position);
	body->setVelocity(velocity);
	body->setAcceleration(acceleration);
	body->setMass(mass);
	body->setDamping(SQUARE_LINEAR_DAMPING, SQUARE_ANGULAR_DAMPING);

	// Set the inertia tensor for the capsule
	Matrix3 tensor;

	float cM; // cylinder mass
	float hsM; // mass of hemispheres
	float rSq = radiusInput * radiusInput;
	cM = PI * heightInput * rSq * density;
	hsM = PI_TIMES2 * oneDiv3 * rSq * radiusInput * density;

	// from cylinder
	tensor.data[4] = rSq * cM * 0.5f;
	tensor.data[0] = tensor.data[8] = tensor.data[4] * 0.5f + cM * heightInput * heightInput * oneDiv12;
	// from hemispheres
	float temp0 = hsM * 2.0f * rSq / 5.0f;
	tensor.data[4] += temp0 * 2.0f;
	float temp1 = heightInput * 0.5f;
	float temp2 = temp0 + hsM * (temp1 * temp1 + 3.0f * oneDiv8 * heightInput * heightInput);
	tensor.data[0] += temp2 * 2.0f;
	tensor.data[8] += temp2 * 2.0f;
	tensor.data[1] = tensor.data[2] = tensor.data[3] = tensor.data[5] = tensor.data[6] = tensor.data[7] = 0.0f;

	body->setInertiaTensor(tensor);
	
	body->addForceAtBodyPoint(Vector3(200.0f, 0.0f, 0.0f), Vector3(0.0f, 3.0f, 0.0f));
}
