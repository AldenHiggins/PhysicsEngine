#include "RenderableObjects.h"
#include "PhysicsDemo.h"
#include "Rendering.h"

using namespace PhysicsDemo;

Axis::Axis
(
	RenderingDemo *demo,
	Vector3 colorInput,
	Vector3 firstPointInput,
	Vector3 secondPointInput,
	Vector3 thirdPointInput,
	Vector3 fourthPointInput
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
	Vector3 positionInput,
	Vector3 normalInput,
	Vector3 upInput,
	Vector3 colorInput,
	real halfSizeInput,
	real offset
)
{
	position = positionInput;
	normal = normalInput;
	up = upInput;
	color = colorInput;
	halfSize = halfSizeInput;

	plane = new CollisionPlane();
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


	Vector3 right = up.vectorProduct(normal);

	real newHalfSize = halfSize;
	Vector3 variableColor = color;
	for (int quadIndex = 0; quadIndex < 50; quadIndex++)
	{
		variableColor[0] += .1f;
		glColor3f((float)variableColor[0], (float)variableColor[1], (float)variableColor[2]);
		Vector3 position1 = position + (up * newHalfSize) + (right * newHalfSize) + (normal * quadIndex * .01f);
		Vector3 position2 = position + (up * newHalfSize) - (right * newHalfSize) + (normal * quadIndex * .01f);
		Vector3 position3 = position - (up * newHalfSize) - (right * newHalfSize) + (normal * quadIndex * .01f);
		Vector3 position4 = position - (up * newHalfSize) + (right * newHalfSize) + (normal * quadIndex * .01f);

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
	Vector3 position,
	Vector3 velocity,
	Vector3 acceleration,
	real mass,
	Vector3 halfSize,
	Vector3 colorInput,
	bool isStatic
)
{
	color = colorInput;
	boxPrimitive = new CollisionBox();

	boxPrimitive->body->setStatic(isStatic);
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
	// Render the box
	Rendering::drawBox(mat, color, this->boxPrimitive->halfSize);
}

Sphere::Sphere
(
	RenderingDemo *demo,
	Vector3 position,
	Vector3 velocity,
	Vector3 acceleration,
	real mass,
	real radius,
	Vector3 colorInput
)
{
	color = colorInput;

	spherePrimitive = new CollisionSphere();
	spherePrimitive->setState(position, velocity, acceleration, mass, radius);
	spherePrimitive->body->calculateDerivedData();

	demo->physicsEngine.createSphere(spherePrimitive);
	demo->sphereObjects.push_back(this);
	demo->renderableObjects.push_back(this);
}

// Display this sphere
void Sphere::display()
{
	Rendering::drawSphere(this->spherePrimitive->body->getPosition(), color, this->spherePrimitive->radius);
}


Capsule::Capsule
(
	RenderingDemo *demo,
	Vector3 position,
	Quaternion rotation,
	Vector3 velocity,
	Vector3 acceleration,
	real mass,
	real radius,
	real height,
	Vector3 colorInput
)
{
	color = colorInput;

	// Generate the quadric object
	quadricObject = gluNewQuadric();
	gluQuadricNormals(quadricObject, GLU_SMOOTH);
	gluQuadricDrawStyle(quadricObject, GLU_FILL);

	capsulePrimitive = new CollisionCapsule();
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

	Rendering::drawCapsule(mat, color, this->capsulePrimitive->height, this->capsulePrimitive->radius);
}
