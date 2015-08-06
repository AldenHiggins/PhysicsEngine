#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include "MathDataTypes.h"

using namespace PhysicsEngine;

/**
* Creates a window in which to display the scene.
*/
void createWindow(const char* title)
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(640, 320);
	glutInitWindowPosition(0, 0);
	glutCreateWindow(title);
}

/**
* Called each frame to update the 3D scene. Delegates to
* the application.
*/
void update()
{

}

/**
* Called each frame to display the 3D scene. Delegates to
* the application.
*/
void display()
{
	// Update the displayed content.
	

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	// eye, center, up
	gluLookAt(0.0, 4.0, 0.0, 0.0, 4.0, 6.0, 0.0, 1.0, 0.0);
	glColor3f(1, 0.5f, 0.5f);

	real xPosition = 0;
	real yPosition = 4;
	real zPosition = 10;
	real size = 3;

	glBegin(GL_QUADS);
	glVertex3f(xPosition - size, yPosition - size, zPosition);
	glVertex3f(xPosition + size, yPosition - size, zPosition);
	glVertex3f(xPosition + size, yPosition + size, zPosition);
	glVertex3f(xPosition - size, yPosition + size, zPosition);

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
}

void initializeGraphics()
{
	glClearColor(0.9f, 0.95f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);

	// Now set the view
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (double)1920 / (double)1080, 1.0, 500.0);
	glMatrixMode(GL_MODELVIEW);

	// But override the clear color
	glClearColor(0.0f, 0.0f, 0.1f, 1.0f);
}

/**
* The main entry point. We pass arguments onto GLUT.
*/
int main(int argc, char** argv)
{
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