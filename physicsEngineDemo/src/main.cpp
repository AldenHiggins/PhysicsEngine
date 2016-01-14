#include <GL/glut.h>
#include "PhysicsDemo.h"

PhysicsDemo::RenderingDemo demo;

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
	demo.update();
}

/**
* Called each frame to display the 3D scene. Delegates to
* the application.
*/
void display()
{
	demo.display();
}

/**
* Called when a mouse button is pressed. Delegates to the
* application.
*/
void mouse(int button, int state, int x, int y)
{
	demo.mouse(button, state, x, y);
}

/**
* Called when the display window changes size.
*/
void reshape(int width, int height)
{
	demo.reshape(width, height);
}

/**
* Called when a key is pressed.
*/
void keyboard(unsigned char key, int x, int y)
{
	demo.keyboard(key, x, y);
}
//
/**
* Called when a key is pressed.
*/
void keyboardUp(unsigned char key, int x, int y)
{
	demo.keyboardUp(key, x, y);
}

///**
//* Called when the mouse is dragged.
//*/
void motion(int x, int y)
{
	demo.motion(x, y);
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
	glutKeyboardUpFunc(keyboardUp);
	glutDisplayFunc(display);
	glutIdleFunc(update);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);

	// Initialize the graphics
	demo.initializeGraphics();

	// Initialize the scene
	demo.initializeScene();

	// Run the application
	glutMainLoop();
}