#include "PhysicsDemo.h"
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include "MathDataTypes.h"
#include "ApplicationSettings.h"
#include "Particle.h"
#include "Timing.h"
#include "Controls.h"
#include "RenderableObjects.h"
#include "PlayerControls.h"
#include "DataTypeRedefinition.h"

using namespace PhysicsDemo;

/**
* Creates a window in which to display the scene.
*/
void RenderingDemo::createWindow(const char* title)
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	glutInitWindowPosition(0, 0);
	glutCreateWindow(title);
}

/**
* Called each frame to update the 3D scene. Delegates to
* the application.
*/
void RenderingDemo::update()
{
	// Update timing data and the duration variable
	TimingData::get().update();
	duration = (float)TimingData::get().lastFrameDuration * 0.001f;
	if (duration <= 0.0f)
	{
		return;
	}
	else if (duration > 0.05f)
	{
		duration = 0.05f;
	}

	// Update the player's position
	player.update(duration);

	// Update the world physics based on the timestep of the frame
	if (!isPaused)
	{
		physicsEngine.updatePhysics(duration);
	}
	
	glutPostRedisplay();
}

/**
* Called each frame to display the 3D scene. Delegates to
* the application.
*/
void RenderingDemo::display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Light up the scene
	GLfloat light_position[] = { -4.0, 10.0, -10.0, 0.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	// Determine the new camera forward
	Matrix3 rotMatrix;
	rotMatrix.setOrientation(player.getRotation());
	Vector3 rotatedForward = rotMatrix.transform(Vector3(0.0f, 0.0f, 1.0f));
	glLoadIdentity();
	// Look out towards the forward direction (eye, forward, up)
	gluLookAt
	(
		0.0, 0.0, 0.0,
		rotatedForward[0], rotatedForward[1], rotatedForward[2],
		0.0, 1.0, 0.0
	);
	// Move the camera to the player's current location
	Vector3 position = player.getPosition();
	glTranslatef((float)(-1 * position[0]), (float)(-1 * position[1]), (float)(-1 * position[2]));

	// Draw the scene
	drawScene();

	glFlush();
	glutSwapBuffers();
}

// Draw the scene
void RenderingDemo::drawScene()
{
	// Draw all of the renderable objects
	for (unsigned int renderableIndex = 0; renderableIndex < renderableObjects.size(); renderableIndex++)
	{
		renderableObjects[renderableIndex]->display();
	}
}

/**
* Called when a mouse button is pressed. Delegates to the
* application.
*/
void RenderingDemo::mouse(int button, int state, int x, int y)
{
	lastX = x;
	lastY = y;
}

/**
* Called when the display window changes size.
*/
void RenderingDemo::reshape(int width, int height)
{
}

/**
* Called when a key is pressed.
*/
void RenderingDemo::keyboard(unsigned char key, int x, int y)
{
	Controls::keyCheck(key, this);
	player.keyCheck(key);
}

/**
* Called when a key is pressed.
*/
void RenderingDemo::keyboardUp(unsigned char key, int x, int y)
{
	player.keyUpCheck(key);
}

/**
* Called when the mouse is dragged.
*/
void RenderingDemo::motion(int x, int y)
{
	Quaternion playerRotation = player.getRotation();

	real angle = (float)(x - lastX);
	angle = angle * PI / 180.0f;
	Quaternion yawQuat(cos(angle / 2), 0.0f, sin(angle / 2), 0.0f);

	playerRotation *= yawQuat;

	angle = playerRotation.toEuler()[1];
	angle = angle * PI / 180.0f;
	Quaternion newQuat(cos(angle / 2), 0.0f, sin(angle / 2), 0.0f);

	Matrix3 rotMatrix;
	rotMatrix.setOrientation(newQuat);

	Vector3 rotatedForward = rotMatrix.transform(Vector3(0.0f, 0.0f, 1.0f));

	Vector3 right = Vector3(0.0f, 1.0f, 0.0f).vectorProduct(rotatedForward);

	angle = (y - lastY);
	angle = angle * PI / 180.0f;
	angle *= -1;
	Quaternion pitchQuat(cos(angle / 2), right[0] * sin(angle / 2), right[1] * sin(angle / 2), right[2] * sin(angle / 2));

	playerRotation *= pitchQuat;
	
	player.setRotation(playerRotation);

	// Remember the position
	lastX = x;
	lastY = y;
}

void RenderingDemo::initializeGraphics()
{
	// Create a light
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 50.0 };
	GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);

	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);

	// Enable color
	glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

	// Now set the view
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (double)SCREEN_WIDTH / (double)SCREEN_HEIGHT, 1.0, 500.0);
	glMatrixMode(GL_MODELVIEW);
}

void RenderingDemo::initializeScene()
{
	// Initialize the timers
	TimingData::init();
	//Plane *plane1 = new Plane
	//(
	//	this,
	//	Vector3(),
	//	Vector3(0.0f, 1.0f, 0.0f),
	//	Vector3(1.0f, 0.0f, 0.0f),
	//	Vector3(0.3f, 0.3f, 0.7f),
	//	10.0f,
	//	0.0f
	//);

	//Plane *plane2 = new Plane
	//(
	//	this,
	//	Vector3(0.0f, 10.0f, 10.0f),
	//	Vector3(0.0f, 0.0f, -1.0f),
	//	Vector3(1.0f, 0.0f, 0.0f),
	//	Vector3(0.9f, 0.9f, 0.9f),
	//	10.0f,
	//	-10.0f
	//);

	//Plane *plane3 = new Plane
	//(
	//	this,
	//	Vector3(0.0f, 10.0f, -10.0f),
	//	Vector3(0.0f, 0.0f, 1.0f),
	//	Vector3(1.0f, 0.0f, 0.0f),
	//	Vector3(0.0f, 0.8f, 0.0f),
	//	10.0f,
	//	-10.0f
	//);

	//Plane *plane4 = new Plane
	//(
	//	this,
	//	Vector3(10.0f, 10.0f, 0.0f),
	//	Vector3(-1.0f, 0.0f, 0.0f),
	//	Vector3(0.0f, 1.0f, 0.0f),
	//	Vector3(0.4f, 0.8f, 0.1f),
	//	10.0f,
	//	-10.0f
	//);

	//Plane *plane5 = new Plane
	//(
	//	this,
	//	Vector3(-10.0f, 10.0f, 0.0f),
	//	Vector3(1.0f, 0.0f, 0.0f),
	//	Vector3(0.0f, 1.0f, 0.0f),
	//	Vector3(1.0f, 0.0f, 0.0f),
	//	10.0f,
	//	-10.0f
	//);

	//// Create the axes and add them to renderableObjects
	//Axis *firstAxis = new Axis
	//(
	//	this,
	//	Vector3(0.0f, 0.0f, 0.0f),
	//	Vector3(20, .01f, AXES_WIDTH / 2),
	//	Vector3(20, .01f, -1 * AXES_WIDTH / 2),
	//	Vector3(-20, .01f, -1 * AXES_WIDTH / 2),
	//	Vector3(-20, .01f, AXES_WIDTH / 2)
	//);

	//Axis *secondAxis = new Axis
	//(
	//	this,
	//	Vector3(0.0f, 0.0f, 0.0f),
	//	Vector3(AXES_WIDTH / 2, .01f, 20),
	//	Vector3(-1 * AXES_WIDTH / 2, .01f, 20),
	//	Vector3(-1 * AXES_WIDTH / 2, .01f, -20),
	//	Vector3(AXES_WIDTH / 2, .01f, -20)
	//); 
}

