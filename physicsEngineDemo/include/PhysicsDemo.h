#ifndef PHYSICS_DEMO_H__
#define PHYSICS_DEMO_H__

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
#include "PhysicsDemo.h"

namespace PhysicsDemo
{
	class RenderingDemo
	{
	public:
		// The player controller
		PlayerController player;

		// Duration of the previous frame
		float duration;

		// Used to pause the game
		bool isPaused;

		// The instance of the physics engine that this game will make use of
		PhysicsEngine::Physics physicsEngine;
		// The list of all renderable objects in the demo
		std::vector<Renderable *> renderableObjects;
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
		int lastX;
		int lastY;

		void initializeScene();
		void initializeGraphics();
		void motion(int x, int y);
		void keyboardUp(unsigned char key, int x, int y);
		void keyboard(unsigned char key, int x, int y);
		void reshape(int width, int height);
		void mouse(int button, int state, int x, int y);
		void display();
		void update();
		void createWindow(const char* title);

		// Draw the background of the scene
		void drawBackground();
		// Draw the scene
		void drawScene();
	};
}

#endif //PHYSICS_DEMO_H__
