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

using namespace PhysicsDemo;

// Draw the background of the scene
void drawBackground();
// Draw the scene
void drawScene();

// The player controller
PlayerController player;

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
// Contains all of the planes in the scene
std::vector<Plane *> planes;

// Camera control variables
//float theta;
//float phi;
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
	gluLookAt(0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0);
	// Move the scene based on where the player is
	PhysicsEngine::real pitch = player.getPitch();
	PhysicsEngine::real yaw = player.getYaw();
	PhysicsEngine::Vector3 position = player.getPosition();
	glRotatef(-1 * pitch, 1, 0, 0);
	glRotatef(yaw, 0, 1, 0);
	glTranslatef(-1 * position[0], -1 * position[1], -1 * position[2]);
	
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

	// Draw the scene
	drawScene();

	glFlush();
	glutSwapBuffers();
}

// Draw the background of the scene
void drawBackground()
{
	glBegin(GL_QUADS);	
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

// Draw the scene
void drawScene()
{
	// Draw all the planes
	for (unsigned int planeIndex = 0; planeIndex < planes.size(); planeIndex++)
	{
		planes[planeIndex]->display();
	}

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
	Controls::keyCheck(key, &physicsEngine, &particles, &rectangleObjects, &sphereObjects, &capsuleObjects, &player);
	player.keyCheck(key);
}

/**
* Called when a key is pressed.
*/
void keyboardUp(unsigned char key, int x, int y)
{
	player.keyUpCheck(key);
}

/**
* Called when the mouse is dragged.
*/
void motion(int x, int y)
{
	// Update the camera
	PhysicsEngine::real yaw = player.getYaw();
	PhysicsEngine::real pitch = player.getPitch();

	player.setYaw(yaw + (x - lastX) * .25f);
	player.setPitch(pitch + (y - lastY) * .25f);

	// Clamp the pitch to prevent gimbal lock and other bad things
	if (pitch < -20.0f)
	{
		player.setPitch(-20.0f);
	}
	else if (pitch > 80.0f)
	{
		player.setPitch(80.0f);
	}

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
	Plane *plane1 = new Plane
	(
		PhysicsEngine::Vector3(),
		PhysicsEngine::Vector3(0.0f, 1.0f, 0.0f),
		PhysicsEngine::Vector3(1.0f, 0.0f, 0.0f),
		PhysicsEngine::Vector3(0.3f, 0.3f, 0.7f),
		20.0f,
		0.0f
	);

	Plane *plane2 = new Plane
	(
		PhysicsEngine::Vector3(0.0f, 0.0f, 20.0f),
		PhysicsEngine::Vector3(0.0f, 0.0f, -1.0f),
		PhysicsEngine::Vector3(1.0f, 0.0f, 0.0f),
		PhysicsEngine::Vector3(0.9f, 0.9f, 0.9f),
		20.0f,
		-20.0f
	);

	Plane *plane3 = new Plane
	(
		PhysicsEngine::Vector3(0.0f, 0.0f, -20.0f),
		PhysicsEngine::Vector3(0.0f, 0.0f, 1.0f),
		PhysicsEngine::Vector3(1.0f, 0.0f, 0.0f),
		PhysicsEngine::Vector3(0.0f, 0.8f, 0.0f),
		20.0f,
		-20.0f
	);

	Plane *plane4 = new Plane
	(
		PhysicsEngine::Vector3(20.0f, 0.0f, 0.0f),
		PhysicsEngine::Vector3(-1.0f, 0.0f, 0.0f),
		PhysicsEngine::Vector3(0.0f, 1.0f, 0.0f),
		PhysicsEngine::Vector3(0.4f, 0.8f, 0.1f),
		20.0f,
		-20.0f
	);

	Plane *plane5 = new Plane
	(
		PhysicsEngine::Vector3(-20.0f, 0.0f, 0.0f),
		PhysicsEngine::Vector3(1.0f, 0.0f, 0.0f),
		PhysicsEngine::Vector3(0.0f, 1.0f, 0.0f),
		PhysicsEngine::Vector3(1.0f, 0.0f, 0.0f),
		20.0f,
		-20.0f
	);

	// Initialize the planes
	planes.push_back(plane1);
	planes.push_back(plane2);
	planes.push_back(plane3);
	planes.push_back(plane4);
	planes.push_back(plane5);

	// Register them with the physics engine
	physicsEngine.createPlane(plane1->plane);
	physicsEngine.createPlane(plane2->plane);
	physicsEngine.createPlane(plane3->plane);
	physicsEngine.createPlane(plane4->plane);
	physicsEngine.createPlane(plane5->plane);
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
	glutKeyboardUpFunc(keyboardUp);
	glutDisplayFunc(display);
	glutIdleFunc(update);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);

	// Initialize the graphics
	initializeGraphics();

	// Run the application
	glutMainLoop();
}