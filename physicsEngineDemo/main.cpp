#include <iostream>
#include <vector>
#include <GL/glut.h>
#include "particle.h"
#include <windows.h>

#define initialWindowX 500
#define initialWindowY 100
#define windowSizeX 800
#define windowSizeY 800



GLfloat light_diffuse[] = { 0.3, 0.3, 0.3, 1.0 };  /* Red diffuse light. */
GLfloat light_position[] = { 0.0, 0.0, 7.0, 0.0 };  /* Infinite light location. */
GLfloat light_direction[] = { 0.0, 0.0, -1.0 };
GLfloat light_direction2[] = { 0.0, 0.0, 1.0 };

// Object variables
GLfloat translateX = 0;
GLfloat translateY = 0;
GLfloat floorColor[4] = { 1.0, 0.0, 1.0, 1.0 };
GLfloat sphereColor[4] = { 0.8, 0.5, 0.3, 1.0 };

// Camera variables
GLfloat cameraX = 0;
GLfloat cameraY = -15;
GLfloat cameraZ = 5;

GLfloat cameraPosition[] = {0.0,-15.0,5.0};
GLfloat cameraRotation[] = { 0.0, 0.0, 0.0 };

// Particles array
std::vector<Particle> particles;

void init()
{
	// Initialize all the particles
	GLfloat part1Pos[3] = { 0.0, 0.0, 3.0 };
	GLfloat part1Color[4] = { .5, .5, .5, 1.0 };
	Particle part1(part1Pos, .5f, part1Color);
	Particle part2(part1Pos, .5f, part1Color);
	Particle part3(part1Pos, .5f, part1Color);
	Particle part4(part1Pos, .5f, part1Color);
	Particle part5(part1Pos, .5f, part1Color);
	Particle part6(part1Pos, .5f, part1Color);

	part1.velocity[0] = .0f;
	part1.velocity[1] = .0f;
	part1.velocity[2] = .2f;


	//part2.velocity[0] = .0f;
	//part2.velocity[1] = .1f;
	//part2.velocity[2] = .0f;

	//part3.velocity[0] = -.1f;
	//part3.velocity[1] = .0f;
	//part3.velocity[2] = .0f;

	//part4.velocity[0] = .0f;
	//part4.velocity[1] = -.1f;
	//part4.velocity[2] = .0f;

	//part5.velocity[0] = .0f;
	//part5.velocity[1] = .0f;
	//part5.velocity[2] = .1f;

	//part6.velocity[0] = .0f;
	//part6.velocity[1] = .0f;
	//part6.velocity[2] = -.1f;

	particles.push_back(part1);
	//particles.push_back(part2);
	//particles.push_back(part3);
	//particles.push_back(part4);
	//particles.push_back(part5);
	//particles.push_back(part6);

	/* Enable a single OpenGL light. */
	/*glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light_direction);

	glEnable(GL_LIGHT1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT1, GL_POSITION, light_position);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, light_direction2);
	glEnable(GL_LIGHTING);*/

	glClearColor(0.0, 0.0, 0.0, 0.0);
	/* Use depth buffering for hidden surface elimination. */
	glEnable(GL_DEPTH_TEST);

	/* Setup the view of the cube. */
	glMatrixMode(GL_PROJECTION);
	gluPerspective( /* field of view in degree */ 40.0,
		/* aspect ratio */ 1.0,
		/* Z near */ 1.0, /* Z far */ 50.0);
	glMatrixMode(GL_MODELVIEW);
	//gluLookAt(0.0, 5.0, 0.0,  /* eye is at (0,0,5) */
	//	0.0, 0.0, 0.0,      /* center is at (0,0,0) */
	//	0.0, 0.0, 1.0);      /* up is in positive Y direction */
	//gluLookAt(cameraX, cameraY, cameraZ,  /* eye is at (0,0,5) */
	//	cameraX, 0.0, cameraZ,      /* center is at (0,0,0) */
	//	0.0, 0.0, 1.0);      /* up is in positive Y direction */
	gluLookAt(0, -32, 7,  /* eye is at (0,0,5) */
		0, 0.0, 5,      /* center is at (0,0,0) */
		0.0, 0.0, 1.0);
}

void moveCamera()
{
	//gluLookAt(cameraX, cameraY, cameraZ,  /* eye is at (0,0,5) */
	//	cameraX, 0.0, cameraZ,      /* center is at (0,0,0) */
	//	0.0, 0.0, 1.0);      /* up is in positive Y direction */
	gluLookAt(0, -32, 7,  /* eye is at (0,0,5) */
		0, 0.0, 5,      /* center is at (0,0,0) */
		0.0, 0.0, 1.0);      /* up is in positive Y direction */
}

void drawGround()
{
	//glPushMatrix();
	//glBegin(GL_POLYGON);
	//glMaterialfv(GL_FRONT, GL_DIFFUSE, floorColor);

	//glVertex3f(-WORLDBOUNDS.0f,-WORLDBOUNDS.0f, 0.0f);
	//glVertex3f(-WORLDBOUNDS.0f, WORLDBOUNDS.0f, 0.0f);
	//glVertex3f(WORLDBOUNDS.0f, WORLDBOUNDS.0f, 0.0f);
	//glVertex3f(WORLDBOUNDS.0f, -WORLDBOUNDS.0f, 0.0f);

	//// Additional vertices for lighting purposes
	//glVertex3f(2.5f, 2.5f, 0.0f);
	//glVertex3f(2.5f, -2.5f, 0.0f);
	//glVertex3f(-2.5f, -2.5f, 0.0f);
	//glVertex3f(-2.5f, 2.5f, 0.0f);
	//glVertex3f(12.5f, 12.5f, 0.0f);
	//glVertex3f(12.5f, -12.5f, 0.0f);
	//glVertex3f(-12.5f, -12.5f, 0.0f);
	//glVertex3f(-12.5f, 12.5f, 0.0f);

	//glEnd();
	glPushMatrix();
	glColor3f(.6f, .7f, .2f);
	glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, floorColor);
	glTranslatef(0.f, 0.f, -14.0f);
	glutSolidCube(WORLDBOUNDS);
	glPopMatrix();

	glPushMatrix();
	glColor3f(.6f, .7f, .2f);
	glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, floorColor);
	glTranslatef(0.f, 0.f, 35.5f);
	glutSolidCube(WORLDBOUNDS);
	glPopMatrix();

	glPushMatrix();
	glColor3f(.6f, .2f, .2f);
	glTranslatef(WORLDBOUNDS, 0.f, 0.f);
	glutSolidCube(WORLDBOUNDS);
	glPopMatrix();

	glPushMatrix();
	glColor3f(.6f, .2f, .5f);
	glTranslatef(-WORLDBOUNDS, 0.f, 0.f);
	glutSolidCube(WORLDBOUNDS);
	glPopMatrix();

	glPushMatrix();
	glColor3f(.6f, .8f, .5f);
	glTranslatef(0.f, 22.f, 0.f);
	glutSolidCube(WORLDBOUNDS);
	glPopMatrix();

	glPushMatrix();
	glColor3f(.2f, .7f, .5f);
	glTranslatef(0.f, -35.f, 0.f);
	glutSolidCube(WORLDBOUNDS);
	glPopMatrix();

	glPushMatrix();
	glColor3f(.2f, .7f, .5f);
	glTranslatef(0.f, 0.f, 32.f);
	glutSolidCube(WORLDBOUNDS);
	glPopMatrix();
}

void drawLight()
{
	glPushMatrix();
	glColor3f(.4f, .4f, .4f);
	glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
	glTranslatef(light_position[0], light_position[1], light_position[2] - 2);
	glutSolidCube(2.f);
	glPopMatrix();
}
#define xCenter initialWindowX + (.5 * windowSizeX)
#define yCenter initialWindowY + (.5 * windowSizeY)
void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// do mouse calculations
	POINT cursor;

	GetCursorPos(&cursor);
	std::cout << "Cursor x: " << cursor.x << "\n";
	std::cout << "Cursor y: " << cursor.y << "\n";

	if (cursor.x > xCenter + 2)
	{
		cameraRotation[2] += 1;
	}
	else if (cursor.x < xCenter - 2)
	{
		cameraRotation[2] -= 1;
	}

	if (cursor.y > yCenter + 2)
	{
		cameraRotation[0] += 1;
	}
	else if (cursor.y < yCenter)
	{
		cameraRotation[0] -= 1; 
	}



	SetCursorPos(initialWindowX + (.5 * windowSizeX), initialWindowY + (.5 * windowSizeY));
	//glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glMatrixMode(GL_PROJECTION);
	moveCamera();

	glMatrixMode(GL_MODELVIEW);
	glTranslatef(cameraX, cameraY, cameraZ);
	glRotatef(cameraRotation[0], cameraX, 0.0, 0.0);
	glRotatef(cameraRotation[1], 0.0, cameraY - 32, 0.0);
	//glRotatef(cameraRotation[2], 0.0, 0.0, cameraZ);
	
	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
	//
	//glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);
	//glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	//glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light_direction);
	//glLightfv(GL_LIGHT1, GL_POSITION, light_position);
	//glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, light_direction2);



	//glEnable(GL_LIGHTING);
	
	//glLoadIdentity();
	//std::cout << particles.size() << "\n";
	for (int i = 0; i < particles.size(); ++i)
	{
		particles[i].draw();
	}
	//drawLight();
	drawGround();
	

	

	glutSwapBuffers();
	//glFlush();
}

void idle()
{
	glutPostRedisplay();
}

void specialFunc(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		translateY += .1;
		break;
	case GLUT_KEY_RIGHT:
		translateX -= .1;
		break;
	case GLUT_KEY_LEFT:
		translateX += .1;
		break;
	case GLUT_KEY_DOWN:
		translateY -= .1;
		break;
	}
}

void keyFunc(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w':
		cameraY += .1;
		break;
	case 's':
		cameraY -= .1;
		break;
	case 'a':
		cameraX -= .1;
		break;
	case 'd':
		cameraX += .1;
		break;
	case 'q':
		cameraZ -= .1;
		break;
	case 'e':
		cameraZ += .1;
		break;
	// Exit by pressing 'esc'
	case 27:
		exit(0);
		break;
	}
}
//SetCursorPos(initialWindowX + (.5 * windowSizeX)


void mouseMoveFunction(int x, int y)
{
	std::cout << "X: " << x << "\n";
	std::cout << "Y: " << y << "\n";
	std::cout << "XCenter: " << xCenter << "\n";
	std::cout << "YCenter: " << yCenter << "\n";
	if (x > xCenter + 20)
	{
		cameraRotation[2] += 1;
	}
	else if (x < xCenter - 20)
	{
		cameraRotation[2] -= 1;
	}

	//if (y > yCenter)
	//{
	//	cameraRotation[2] -= .1;
	//}
	//else if (y < yCenter)
	//{
	//	cameraRotation[2] += .1;
	//}

	//xCenter = initialWindowX + (.5 * windowSizeX);
	//yCenter = initialWindowY + (.5 * windowSizeY);
	/*previousX = x;
	previousY = y;*/
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(windowSizeX, windowSizeY);
	glutInitWindowPosition(initialWindowX, initialWindowY);
	glutCreateWindow("Alden's Physics Demo");
	glutSpecialFunc(specialFunc);
	//glutPassiveMotionFunc(mouseMoveFunction);
	glutKeyboardFunc(keyFunc);
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	init();
	glutMainLoop();
	return 0;
}
