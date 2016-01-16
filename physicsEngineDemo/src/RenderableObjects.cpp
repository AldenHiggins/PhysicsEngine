#include "RenderableObjects.h"
#include "PhysicsDemo.h"

using namespace PhysicsDemo;

Axis::Axis
(
	RenderingDemo *demo,
	PhysicsEngine::Vector3 colorInput,
	PhysicsEngine::Vector3 firstPointInput,
	PhysicsEngine::Vector3 secondPointInput,
	PhysicsEngine::Vector3 thirdPointInput,
	PhysicsEngine::Vector3 fourthPointInput
)
{
	color = colorInput;
	firstPoint = firstPointInput;
	secondPoint = secondPointInput;
	thirdPoint = thirdPointInput;
	fourthPoint = fourthPointInput;

	// Add the axis to the renderable list
	demo->renderableObjects.push_back(this);
}

void Axis::display()
{
	glBegin(GL_QUADS);
	// Draw the lines of the axes
	glColor3f((float)color[0], (float)color[1], (float)color[2]);
	glVertex3f((float)firstPoint[0], (float)firstPoint[1], (float)firstPoint[2]);
	glVertex3f((float)secondPoint[0], (float)secondPoint[1], (float)secondPoint[2]);
	glVertex3f((float)thirdPoint[0], (float)thirdPoint[1], (float)thirdPoint[2]);
	glVertex3f((float)fourthPoint[0], (float)fourthPoint[1], (float)fourthPoint[2]);
	glEnd();
}

Plane::Plane
(
	RenderingDemo *demo,
	PhysicsEngine::Vector3 positionInput,
	PhysicsEngine::Vector3 normalInput,
	PhysicsEngine::Vector3 upInput,
	PhysicsEngine::Vector3 colorInput,
	PhysicsEngine::real halfSizeInput,
	PhysicsEngine::real offset
)
{
	position = positionInput;
	normal = normalInput;
	up = upInput;
	color = colorInput;
	halfSize = halfSizeInput;

	plane = new PhysicsEngine::CollisionPlane();
	plane->normal = normalInput;
	plane->offset = offset;

	// Add the newly created plane to the demo's lists and the physics engine
	demo->planes.push_back(this);
	demo->renderableObjects.push_back(this);
	demo->physicsEngine.createPlane(plane);
}

// Display this plane
void Plane::display()
{
	glBegin(GL_QUADS);
	glColor3f((float)color[0], (float)color[1], (float)color[2]);
	glNormal3f((float)normal[0], (float)normal[1], (float)normal[2]);


	PhysicsEngine::Vector3 right = up.vectorProduct(normal);

	PhysicsEngine::real newHalfSize = halfSize;
	PhysicsEngine::Vector3 variableColor = color;
	for (int quadIndex = 0; quadIndex < 50; quadIndex++)
	{
		variableColor[0] += .1f;
		glColor3f((float)variableColor[0], (float)variableColor[1], (float)variableColor[2]);
		PhysicsEngine::Vector3 position1 = position + (up * newHalfSize) + (right * newHalfSize) + (normal * quadIndex * .01f);
		PhysicsEngine::Vector3 position2 = position + (up * newHalfSize) - (right * newHalfSize) + (normal * quadIndex * .01f);
		PhysicsEngine::Vector3 position3 = position - (up * newHalfSize) - (right * newHalfSize) + (normal * quadIndex * .01f);
		PhysicsEngine::Vector3 position4 = position - (up * newHalfSize) + (right * newHalfSize) + (normal * quadIndex * .01f);

		glVertex3f((float)position1[0], (float)position1[1], (float)position1[2]);
		glVertex3f((float)position2[0], (float)position2[1], (float)position2[2]);
		glVertex3f((float)position3[0], (float)position3[1], (float)position3[2]);
		glVertex3f((float)position4[0], (float)position4[1], (float)position4[2]);

		newHalfSize = newHalfSize / 1.1;
	}
	glEnd();
}

Box::Box
(
	RenderingDemo *demo,
	PhysicsEngine::Vector3 position,
	PhysicsEngine::Vector3 velocity,
	PhysicsEngine::Vector3 acceleration,
	PhysicsEngine::real mass,
	PhysicsEngine::Vector3 halfSize
)
{
	boxPrimitive = new PhysicsEngine::CollisionBox();

	boxPrimitive->body->setStatic(true);
	boxPrimitive->setState(position, velocity, acceleration, mass, halfSize);
	boxPrimitive->body->calculateDerivedData();

	demo->physicsEngine.createBox(boxPrimitive);
	demo->rectangleObjects.push_back(this);
	demo->renderableObjects.push_back(this);
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
	glScalef((float)(this->boxPrimitive->halfSize[0] * 2), (float)(this->boxPrimitive->halfSize[1] * 2), (float)(this->boxPrimitive->halfSize[2] * 2));
	glutSolidCube(1.0f);
	glPopMatrix();
}

Sphere::Sphere
(
	RenderingDemo *demo,
	PhysicsEngine::Vector3 position,
	PhysicsEngine::Vector3 velocity,
	PhysicsEngine::Vector3 acceleration,
	PhysicsEngine::real mass,
	PhysicsEngine::real radius
)
{
	spherePrimitive = new PhysicsEngine::CollisionSphere();
	spherePrimitive->setState(position, velocity, acceleration, mass, radius);
	spherePrimitive->body->calculateDerivedData();

	demo->physicsEngine.createSphere(spherePrimitive);
	demo->sphereObjects.push_back(this);
	demo->renderableObjects.push_back(this);
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


Capsule::Capsule
(
	RenderingDemo *demo,
	PhysicsEngine::Vector3 position,
	PhysicsEngine::Quaternion rotation,
	PhysicsEngine::Vector3 velocity,
	PhysicsEngine::Vector3 acceleration,
	PhysicsEngine::real mass,
	PhysicsEngine::real radius,
	PhysicsEngine::real height
)
{
	// Generate the quadric object
	quadricObject = gluNewQuadric();
	gluQuadricNormals(quadricObject, GLU_SMOOTH);
	gluQuadricDrawStyle(quadricObject, GLU_FILL);

	capsulePrimitive = new PhysicsEngine::CollisionCapsule();
	capsulePrimitive->setState(position, rotation, velocity, acceleration, mass, radius, height);
	capsulePrimitive->body->calculateDerivedData();

	demo->physicsEngine.createCapsule(capsulePrimitive);
	demo->capsuleObjects.push_back(this);
	demo->renderableObjects.push_back(this);	
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
	glColor3f(.9f, .3f, .1f);

	glPushMatrix();
	glMultMatrixf(mat);
	
	glTranslatef(0.0f, (float)(.5f * this->capsulePrimitive->height), 0.0f);
	glutSolidSphere(this->capsulePrimitive->radius, 50, 50);
	glTranslatef(0.0f, (float)(-1.0f * this->capsulePrimitive->height), 0.0f);
	glutSolidSphere((float)(this->capsulePrimitive->radius), 50, 50);
	glTranslatef(0.0f, (float)(this->capsulePrimitive->height), 0.0f);
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	gluCylinder(quadricObject, this->capsulePrimitive->radius, this->capsulePrimitive->radius, this->capsulePrimitive->height, 100, 100);
	
	glPopMatrix();
}
