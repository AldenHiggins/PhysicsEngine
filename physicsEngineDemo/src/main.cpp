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

using namespace PhysicsDemo;

// Draw the background of the scene
void drawBackground();
// Draw all of the rigid bodies
void drawBodies();

// The instance of the physics engine that this game will make use of
PhysicsEngine::Physics physicsEngine;

// Contains all of the particles in the scene
std::vector<RenderableParticle *> particles;
// Contains all the rectangular objects in the scene
std::vector<Box *> rectangleObjects;
// Contains all of the spherical objects in the scene
std::vector<Sphere *> sphereObjects;
// Contains all of the capsules in the scene
std::vector<Capsule *> capsuleObjects;

// Camera control variables
float theta;
float phi;
int lastX;
int lastY;

/**
* Creates a window in which to display the scene.
*/
void createWindow(const char* title)
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
void update()
{
	TimingData::get().update();
	glutPostRedisplay();
}

/**
* Called each frame to display the 3D scene. Delegates to
* the application.
*/
void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Light up the scene
	GLfloat light_position[] = { -4.0, 50.0, -10.0, 0.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glLoadIdentity();
	// Look out towards the Z direction (eye, center, up)
	gluLookAt(0.0, 4.0, 0.0, 0.0, 4.0, 6.0, 0.0, 1.0, 0.0);
	// Rotate the camera based on mouse movements
	glRotatef(-phi, 1, 0, 0);
	glRotatef(theta, 0, 1, 0);

	// Draw the background
	drawBackground();

	float duration = (float)TimingData::get().lastFrameDuration * 0.001f;
	if (duration <= 0.0f)
	{
		return;
	}
	else if (duration > 0.05f)
	{
		duration = 0.05f;
	}

	// Update the world physics based on the timestep of the frame
	physicsEngine.updatePhysics(duration);

	// Draw all of the rigid bodies
	drawBodies();

	glFlush();
	glutSwapBuffers();
}

// Draw the background of the scene
void drawBackground()
{
	// Draw the ground
	glBegin(GL_QUADS);
	glColor3f(1.0f, 1.0f, 1.0f);
	glNormal3f(0, 1, 0);
	glVertex3f(20, 0, 20);
	glVertex3f(20, 0, -20);
	glVertex3f(-20, 0, -20);
	glVertex3f(-20, 0, 20);
	// Draw the walls
	// Front wall
	glColor3f(0.9f, 0.9f, 0.9f);
	glNormal3f(0, 0, -1);
	glVertex3f(20, 0, 20);
	glVertex3f(20, 20, 20);
	glVertex3f(-20, 20, 20);
	glVertex3f(-20, 0, 20);
	// Back Wall
	glColor3f(0.8f, 0.8f, 0.8f);
	glNormal3f(0, 0, 1);
	glVertex3f(20, 0, -20);
	glVertex3f(20, 20, -20);
	glVertex3f(-20, 20, -20);
	glVertex3f(-20, 0, -20);
	// Left Wall
	glColor3f(0.7f, 0.7f, 0.7f);
	glNormal3f(-1, 0, 0);
	glVertex3f(20, 0, -20);
	glVertex3f(20, 20, -20);
	glVertex3f(20, 20, 20);
	glVertex3f(20, 0, 20);
	// Right Wall
	glColor3f(0.6f, 0.6f, 0.6f);
	glNormal3f(1, 0, 0);
	glVertex3f(-20, 0, 20);
	glVertex3f(-20, 20, 20);
	glVertex3f(-20, 20, -20);
	glVertex3f(-20, 0, -20);
	// Draw the lines of the axes
	glColor3f(0.0f, 0.0f, 0.0f);
	// x axis
	glVertex3f(20, .01f, AXES_WIDTH/2);
	glVertex3f(20, .01f, -1 * AXES_WIDTH/2);
	glVertex3f(-20, .01f, -1 * AXES_WIDTH/2);
	glVertex3f(-20, .01f, AXES_WIDTH/2);
	// z axis
	glVertex3f(AXES_WIDTH/2, .01f, 20);
	glVertex3f(-1 * AXES_WIDTH/2, .01f, 20);
	glVertex3f(-1 * AXES_WIDTH/2, .01f, -20);
	glVertex3f(AXES_WIDTH/2, .01f, -20);
	glEnd();
}

// Draw all of the rigid bodies
void drawBodies()
{
	// Draw all of the particles
	for (unsigned int particleIndex = 0; particleIndex < particles.size(); particleIndex++)
	{
		particles[particleIndex]->display();
	}

	// Draw all cubes
	for (unsigned int rigidBodyIndex = 0; rigidBodyIndex < rectangleObjects.size(); rigidBodyIndex++)
	{
		rectangleObjects[rigidBodyIndex]->display();
	}

	// Draw all spheres
	for (unsigned int rigidBodyIndex = 0; rigidBodyIndex < sphereObjects.size(); rigidBodyIndex++)
	{
		sphereObjects[rigidBodyIndex]->display();
	}

	// Draw all the capsules
	for (unsigned int rigidBodyIndex = 0; rigidBodyIndex < capsuleObjects.size(); rigidBodyIndex++)
	{
		capsuleObjects[rigidBodyIndex]->display();
	}
}

/**
* Called when a mouse button is pressed. Delegates to the
* application.
*/
void mouse(int button, int state, int x, int y)
{
	lastX = x;
	lastY = y;
}

/**
* Called when the display window changes size.
*/
void reshape(int width, int height)
{
}

/**
* Called when a key is pressed.
*/
void keyboard(unsigned char key, int x, int y)
{
	Controls::keyCheck(key, &physicsEngine, &particles, &rectangleObjects, &sphereObjects, &capsuleObjects, theta, phi);
}


/**
* Called when the mouse is dragged.
*/
void motion(int x, int y)
{
	// Update the camera
	theta += (x - lastX)*0.25f;
	phi += (y - lastY)*0.25f;

	// Keep it in bounds
	if (phi < -20.0f) phi = -20.0f;
	else if (phi > 80.0f) phi = 80.0f;

	// Remember the position
	lastX = x;
	lastY = y;
}

void initializeGraphics()
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

void initializeScene()
{
	// Initialize the timers
	TimingData::init();
}

/**
* The main entry point. We pass arguments onto GLUT.
*/
int main(int argc, char** argv)
{
	// Initialize everything needed for the current scene
	initializeScene();
	// Set up GLUT and the timers
	glutInit(&argc, argv);

	// Create the application and its window
	createWindow("PhysicsEngine");

	// Set up the appropriate handler functions
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutDisplayFunc(display);
	glutIdleFunc(update);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);

	// Initialize the graphics
	initializeGraphics();

	// Run the application
	glutMainLoop();
}