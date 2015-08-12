#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include "MathDataTypes.h"
#include "ApplicationSettings.h"
#include "Particle.h"
#include "Timing.h"
#include "RigidBody.h"
#include "Controls.h"
#include "BoundingVolumes.h"

using namespace PhysicsEngine;

// Draw the background of the scene
void drawBackground();

// Contains all of the particles in the scene
std::vector<Particle *> particles;
// Contains all the rigid bodies in the scene
std::vector<RigidBody *> rigidBodies;
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
	float duration = (float)TimingData::get().lastFrameDuration * 0.001f;
	// Integrate all of the particles
	for (int particleIndex = 0; particleIndex < particles.size(); particleIndex++)
	{
		particles[particleIndex]->integrate(duration);
	}

	if (rigidBodies.size() > 0)
	{
		// Build up a list of bounding spheres for broad phase collision detection
		BVHNode<BoundingSphere> *parent = NULL;
		BoundingSphere sphere(rigidBodies[0]->getPosition(), 1.0f);
		BVHNode<BoundingSphere> newNode(parent, sphere);
		newNode.body = rigidBodies[0];
		
		// Integrate all of the rigid bodies and add them to the bounding sphere heirarchy
		for (int rigidBodyIndex = 0; rigidBodyIndex < rigidBodies.size(); rigidBodyIndex++)
		{
			RigidBody *body = rigidBodies[rigidBodyIndex];
			body->integrate(duration);
			BoundingSphere boundSphere(body->getPosition(), 1.0f);
			if (rigidBodyIndex != 0)
			{
				newNode.insert(body, boundSphere);
			}
		}

		// Once all of the rigid bodies have been included check for collisions
		PotentialContact contacts[10];
		int contactsFound = newNode.getPotentialContacts(contacts, 10);

		if (contactsFound > 0)
		{
			std::cout << "Found " << contactsFound << " contacts!" << std::endl;
		}
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	// Look out towards the Z direction (eye, center, up)
	gluLookAt(0.0, 4.0, 0.0, 0.0, 4.0, 6.0, 0.0, 1.0, 0.0);
	// Rotate the camera based on mouse movements
	glRotatef(-phi, 1, 0, 0);
	glRotatef(theta, 0, 1, 0);

	// Now render the scene
	// Draw the background
	drawBackground();
	// Draw all of the particles
	for (int particleIndex = 0; particleIndex < particles.size(); particleIndex++)
	{
		particles[particleIndex]->display();
	}
	// Enable lighting stuff
	const static GLfloat lightPosition[] = { 0.7f, -1, 0.4f, 0 };
	const static GLfloat lightPositionMirror[] = { 0.7f, 1, 0.4f, 0 };

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	// Draw all rigid bodies
	for (int rigidBodyIndex = 0; rigidBodyIndex < rigidBodies.size(); rigidBodyIndex++)
	{
		rigidBodies[rigidBodyIndex]->display();
	}
	// Disable lighting stuff
	glDisable(GL_NORMALIZE);
	glDisable(GL_LIGHTING);
	glDisable(GL_COLOR_MATERIAL);
	glDisable(GL_DEPTH_TEST);

	glFlush();
	glutSwapBuffers();
}

// Draw the background of the scene
void drawBackground()
{
	// Draw the ground
	glBegin(GL_QUADS);
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(20, 0, 20);
	glVertex3f(20, 0, -20);
	glVertex3f(-20, 0, -20);
	glVertex3f(-20, 0, 20);
	// Draw the walls
	// Front wall
	glColor3f(0.9f, 0.9f, 0.9f);
	glVertex3f(20, 0, 20);
	glVertex3f(20, 20, 20);
	glVertex3f(-20, 20, 20);
	glVertex3f(-20, 0, 20);
	// Back Wall
	glColor3f(0.8f, 0.8f, 0.8f);
	glVertex3f(20, 0, -20);
	glVertex3f(20, 20, -20);
	glVertex3f(-20, 20, -20);
	glVertex3f(-20, 0, -20);
	// Left Wall
	glColor3f(0.7f, 0.7f, 0.7f);
	glVertex3f(20, 0, -20);
	glVertex3f(20, 20, -20);
	glVertex3f(20, 20, 20);
	glVertex3f(20, 0, 20);
	// Right Wall
	glColor3f(0.6f, 0.6f, 0.6f);
	glVertex3f(-20, 0, 20);
	glVertex3f(-20, 20, 20);
	glVertex3f(-20, 20, -20);
	glVertex3f(-20, 0, -20);
	// Draw the lines of the axes
	glColor3f(0.0f, 0.0f, 0.0f);
	// x axis
	glVertex3f(20, .01, AXES_WIDTH/2);
	glVertex3f(20, .01, -1 * AXES_WIDTH/2);
	glVertex3f(-20, .01, -1 * AXES_WIDTH/2);
	glVertex3f(-20, .01, AXES_WIDTH/2);
	// z axis
	glVertex3f(AXES_WIDTH/2, .01, 20);
	glVertex3f(-1 * AXES_WIDTH/2, .01, 20);
	glVertex3f(-1 * AXES_WIDTH/2, .01, -20);
	glVertex3f(AXES_WIDTH/2, .01, -20);
	glEnd();
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
	keyCheck(key, &particles, &rigidBodies, theta, phi);
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
	GLfloat lightAmbient[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	GLfloat lightDiffuse[] = { 0.9f, 0.95f, 1.0f, 1.0f };

	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);

	glEnable(GL_LIGHT0);

	glClearColor(0.9f, 0.95f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);

	// Now set the view
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (double)SCREEN_WIDTH / (double)SCREEN_HEIGHT, 1.0, 500.0);
	glMatrixMode(GL_MODELVIEW);

	// But override the clear color
	glClearColor(0.0f, 0.0f, 0.1f, 1.0f);
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