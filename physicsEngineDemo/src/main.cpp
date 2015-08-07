#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include "MathDataTypes.h"
#include "ApplicationSettings.h"
#include "Particle.h"
#include "Timing.h"

using namespace PhysicsEngine;

// Contains all of the particles in the scene
Particle particles[PARTICLE_COUNT];
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
	for (int particleIndex = 0; particleIndex < PARTICLE_COUNT; particleIndex++)
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
	for (int particleIndex = 0; particleIndex < PARTICLE_COUNT; particleIndex++)
	{
		Particle particle = particles[particleIndex];
		// Set the color for the particle
		Vector3 particleColor = particle.getColor();
		glColor3f(particleColor[0], particleColor[1], particleColor[2]);
		// Get the particle's position
		Vector3 particlePosition = particle.getPosition();
		// Get the particle's size
		real size = particle.getSize();
		// Now render the particle
		glVertex3f(particlePosition[0] - size, particlePosition[1] - size, particlePosition[2]);
		glVertex3f(particlePosition[0] + size, particlePosition[1] - size, particlePosition[2]);
		glVertex3f(particlePosition[0] + size, particlePosition[1] + size, particlePosition[2]);
		glVertex3f(particlePosition[0] - size, particlePosition[1] + size, particlePosition[2]);
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
	// Generate all of the particles
	for (int particleIndex = 0; particleIndex < PARTICLE_COUNT; particleIndex++)
	{
		Particle newParticle;
		real xVelocity = ((real)((rand() % 200) - 100)) / 100;
		real yVelocity = ((real)((rand() % 200) - 100)) / 100;
		real zVelocity = ((real)((rand() % 200) - 100)) / 100;
		xVelocity *= .5;
		yVelocity *= .5;
		zVelocity *= .5;
		newParticle.setVelocity(Vector3(xVelocity, yVelocity, zVelocity));
		newParticle.setPosition(Vector3(0.0f, 4.0f, 6.0f));
		newParticle.setColor(Vector3(1, 0.5f, 0.5f));
		newParticle.setSize(0.1f);
		particles[particleIndex] = newParticle;;
	}
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