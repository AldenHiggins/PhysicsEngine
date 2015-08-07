#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "MathDataTypes.h"
#include "ApplicationSettings.h"
#include "Particle.h"
#include "Timing.h"

using namespace PhysicsEngine;

// Generate a new particle
void createParticle(real speed, real size, Vector3 color);

// Contains all of the particles in the scene
Particle particles[MAX_PARTICLE_COUNT];
int currentParticles;
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
	for (int particleIndex = 0; particleIndex < currentParticles; particleIndex++)
	{
		particles[particleIndex].integrate(duration);
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	// Look out towards the Z direction
	// eye, center, up
	gluLookAt(0.0, 4.0, 0.0, 0.0, 4.0, 6.0, 0.0, 1.0, 0.0);
	// Rotate the camera based on mouse movements
	glRotatef(-phi, 1, 0, 0);
	glRotatef(theta, 0, 1, 0);

	// Now render all the particles
	glBegin(GL_QUADS);
	for (int particleIndex = 0; particleIndex < currentParticles; particleIndex++)
	{
		Particle particle = particles[particleIndex];
		particle.display();
	}
	glEnd();

	glFlush();
	glutSwapBuffers();
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
	// Note we omit passing on the x and y: they are rarely needed.
	switch (key)
	{
	// Create a slow particle
	case '1': createParticle(.3f, .1f, Vector3(.5f, 0.0f, 1.0f)); break;
	// Create a fast particle
	case '2': createParticle(.8f, .1f, Vector3(0.0f, 0.5f, 0.5f)); break;
	// Create a big particle
	case '3': createParticle(.3f, 0.3f, Vector3(.75f, 0.23f, 0.68f)); break;
	}
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

// Generate a new particle
void createParticle(real speed, real size, Vector3 color)
{
	if (currentParticles > MAX_PARTICLE_COUNT)
	{
		std::cout << "No more particles can be created!" << std::endl;
		return;
	}
	Particle newParticle;
	real xVelocity = ((real)((rand() % 200) - 100)) / 100;
	real yVelocity = ((real)((rand() % 200) - 100)) / 100;
	real zVelocity = ((real)((rand() % 200) - 100)) / 100;
	xVelocity *= speed;
	yVelocity *= speed;
	zVelocity *= speed;
	newParticle.setVelocity(Vector3(xVelocity, yVelocity, zVelocity));
	newParticle.setPosition(Vector3(0.0f, 4.0f, 6.0f));
	newParticle.setColor(color);
	newParticle.setSize(size);
	newParticle.setLifeTime(3.0f);
	// Add the newly created particle to the list of particles
	particles[currentParticles] = newParticle;
	// Increment the current number of particles
	currentParticles++;
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