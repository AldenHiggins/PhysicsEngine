#include <iostream>
#include <conio.h>
#include <vector>
#include <GL/glut.h>
#include "particle.h"
#include "Drawing.h"
#include <windows.h>

// Include the OculusVR SDK
#include "OVR_CAPI.h"
#include "Kernel\OVR_Math.h"

using namespace OVR;
using namespace std;

// Choose whether the SDK performs rendering/distortion, or the application. 
#define          SDK_RENDER 1  //Do NOT switch until you have viewed and understood the Health and Safety message.
//Disabling this makes it a non-compliant app, and not suitable for demonstration. In place for development only.
const bool       FullScreen = true; //Should be true for correct timing.  Use false for debug only.




void moveCamera()
{
	// do mouse calculations
	float camMoveFraction = 0.3f;
	POINT cursor;

	GetCursorPos(&cursor);

	if (cursor.x > xCenter + 1)
	{
		cameraRotation[2] += 1;
	}
	else if (cursor.x < xCenter - 1)
	{
		cameraRotation[2] -= 1;
	}

	if (cursor.y > yCenter + 1)
	{
		cameraRotation[0] += 1;
	}
	else if (cursor.y < yCenter - 1)
	{
		cameraRotation[0] -= 1;
	}

	SetCursorPos(initialWindowX + (.5 * windowSizeX), initialWindowY + (.5 * windowSizeY));

	std::cout << "cameraRotation[2]: " << cameraRotation[2] << "\n";
	std::cout << "cameraRotation[0]: " << cameraRotation[0] << "\n";

	glRotatef(cameraRotation[0], 1, 0.0, 0.0);
	glRotatef(cameraRotation[2], 0.0, 0.0, 1.0);
	glTranslatef(-cameraX, -cameraY, -cameraZ);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	



	
	//glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glMatrixMode(GL_PROJECTION);
	moveCamera();

	glMatrixMode(GL_MODELVIEW);
	//glTranslatef(cameraX, cameraY, cameraZ);
	
	//glRotatef(cameraRotation[0], cameraX + 1, cameraY, cameraZ);
	//glRotatef(cameraRotation[1], cameraX, cameraY + 1, cameraZ);
	//glRotatef(cameraRotation[2], cameraX, cameraY, cameraZ + 5);
	//glRotatef(cameraRotation[1], 0.0, cameraY - 32, 0.0);
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


void Init(ovrHmd &hmd)
{
	ovr_Initialize();
	hmd = ovrHmd_Create(0);
	if (!hmd) return;

	//ovrHmd_ConfigureTracking(hmd, 1, 0);
	ovrHmd_ConfigureTracking(hmd, ovrTrackingCap_Orientation | ovrTrackingCap_Position, 0);
}


void Output(const ovrHmd &hmd)
{
	// Optional: we can overwrite the previous console to more
	// easily see the values change
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
	GetConsoleScreenBufferInfo(h, &bufferInfo);

	ovrFrameTiming frameTiming;

	while (hmd) {
		frameTiming = ovrHmd_BeginFrameTiming(hmd, 0);
		ovrTrackingState ts = ovrHmd_GetTrackingState(hmd, frameTiming.ScanoutMidpointSeconds);

		if (ts.StatusFlags & (ovrStatus_OrientationTracked | ovrStatus_PositionTracked))
		{
			// The cpp compatibility layer is used to convert ovrPosef to Posef (see OVR_Math.h)
			Posef pose = ts.HeadPose.ThePose;
			float yaw, pitch, roll;
			pose.Rotation.GetEulerAngles<Axis_Y, Axis_X, Axis_Z>(&yaw, &pitch, &roll);

			// Optional: move cursor back to starting position and print values
			SetConsoleCursorPosition(h, bufferInfo.dwCursorPosition);
			cout << "yaw: " << RadToDegree(yaw) << endl;
			cout << "pitch: " << RadToDegree(pitch) << endl;
			cout << "roll: " << RadToDegree(roll) << endl;

			float x, y, z;
			x = pose.Translation.x;
			y = pose.Translation.y;
			z = pose.Translation.z;
			cout << endl;

			cout << "x: " << x << endl;
			cout << "y: " << y << endl;
			cout << "z: " << z << endl;
			Sleep(50);

			ovrHmd_EndFrameTiming(hmd);

			if (_kbhit()) exit(0);
		}
		else
		{
			// Make sure to endFrameTiming no matter what
			ovrHmd_EndFrameTiming(hmd);
		}
	}
}


void Clear(const ovrHmd &hmd)
{
	ovrHmd_Destroy(hmd);
	ovr_Shutdown();
}


int main(int argc, char **argv)
{
	// Oculus initialization code
	ovrHmd hmd;
	Init(hmd);
	

	//// OPENGL setup code
	//glutInit(&argc, argv);
	//glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	//glutInitWindowSize(windowSizeX, windowSizeY);
	//glutInitWindowPosition(initialWindowX, initialWindowY);
	//glutCreateWindow("Alden's Physics Demo");
	//glutSpecialFunc(specialFunc);
	////glutPassiveMotionFunc(mouseMoveFunction);
	//glutKeyboardFunc(keyFunc);
	//glutDisplayFunc(display);
	//glutIdleFunc(idle);
	//init();
	//glutMainLoop();

	Output(hmd);
	// Clean up Oculus
	Clear(hmd);
	return 0;
}
