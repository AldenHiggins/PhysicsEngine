#include "MathDataTypes.h"
#include "RenderParticles.h"
#include <GL/glut.h>
#include <iostream>
#include "PhysicsDemo.h"

using namespace PhysicsDemo;

// Display this particle
void RenderableParticle::display()
{
	// Don't display a dead particle
	if (isDead)
	{
		return;
	}

	PhysicsEngine::Vector3 position = physicsParticle->getPosition();

	glBegin(GL_QUADS);
	// Set the color for the particle
	glColor3f((float)color[0], (float)color[1], (float)color[2]);
	// Now render the particle
	glVertex3f((float)(position[0] - size), (float)(position[1] - size), (float)position[2]);
	glVertex3f((float)(position[0] + size), (float)(position[1] - size), (float)position[2]);
	glVertex3f((float)(position[0] + size), (float)(position[1] + size), (float)position[2]);
	glVertex3f((float)(position[0] - size), (float)(position[1] + size), (float)position[2]);
	glEnd();

	glColor4f(1, 1, 1, 1);
}

// Empty on death function
void RenderableParticle::onDeath(){}


// Display this circular particle
void CircleParticle::display()
{
	// Don't display the particle before's it's done getting ready
	if (timeAliveSoFar < startupTime)
	{
		return;
	}

	// Don't display a dead particle
	if (timeAliveSoFar > lifeTime)
	{
		return;
	}

	PhysicsEngine::Vector3 position = physicsParticle->getPosition();

	glColor3f((float)color[0], (float)color[1], (float)color[2]);
	glPushMatrix();
	glTranslatef((float)position[0], (float)position[1], (float)position[2]);
	glutSolidSphere((float)size, 20, 20);
	glPopMatrix();
}

// Show the firework
void FireworkParticle::display()
{
	// If the firework is dead display it's child particles if it has them
	if (isDead)
	{
		// Check to see if this firework has any more lives
		if (lives <= 0)
		{
			return;
		}
		for (int particleIndex = 0; particleIndex < 5; particleIndex++)
		{
			deathParticles[particleIndex]->display();
		}
	}
	else
	{
		CircleParticle::display();
	}
}

// Generate child particles of this firework
void FireworkParticle::onDeath()
{
	// Don't generate any more particles if this firework is out of lives
	if (lives <= 0)
	{
		return;
	}
	// Create child particles for this firework when it "detonates"
	for (int particleIndex = 0; particleIndex < 5; particleIndex++)
	{
		deathParticles[particleIndex] = CreateParticle::createFireWorkParticle(0.4f, .1f, physicsParticle->getPosition(), color, false, lives - 1);
	}
}

// Generate a new circular particle
RenderableParticle* CreateParticle::createFireWorkParticle(PhysicsEngine::real speed, PhysicsEngine::real size, PhysicsEngine::Vector3 position, PhysicsEngine::Vector3 color, bool originalFirework, int lives)
{
	FireworkParticle *newParticle = new FireworkParticle(lives);
	PhysicsEngine::real xVelocity = ((PhysicsEngine::real)((rand() % 800) - 400)) / 100;
	PhysicsEngine::real yVelocity = ((PhysicsEngine::real)((rand() % 800) - 0)) / 100;
	PhysicsEngine::real zVelocity = ((PhysicsEngine::real)((rand() % 800) - 400)) / 100;

	xVelocity *= speed;
	yVelocity *= speed * 2;
	zVelocity *= speed;

	// Make the original firework follow a predictable path
	if (originalFirework)
	{
		xVelocity = 0;
		yVelocity = 8.0f * speed;
		zVelocity = 0;
		// Give the particle a startup time for GIF recording purposes
		newParticle->physicsParticle->setStartupTime(5.0f);
	}

	newParticle->physicsParticle->setVelocity(PhysicsEngine::Vector3(xVelocity, yVelocity, zVelocity));
	// Add gravity onto this circular particle
	newParticle->physicsParticle->setAcceleration(PhysicsEngine::Vector3(0.0f, -3.81f, 0.0f));
	newParticle->physicsParticle->setPosition(position);
	newParticle->setColor(color);
	newParticle->setSize(size);
	newParticle->physicsParticle->setLifeTime(1.0f);
	return newParticle;
}

// Generate a new circular particle
RenderableParticle* CreateParticle::createCircularParticle(PhysicsEngine::real speed, PhysicsEngine::real size, PhysicsEngine::Vector3 color)
{
	CircleParticle *newParticle = new CircleParticle();
	PhysicsEngine::real xVelocity = ((PhysicsEngine::real)((rand() % 200) - 100)) / 100;
	PhysicsEngine::real yVelocity = 30.0f;
	PhysicsEngine::real zVelocity = ((PhysicsEngine::real)((rand() % 200) - 100)) / 100;
	xVelocity *= speed;
	yVelocity *= speed;
	zVelocity *= speed;
	newParticle->physicsParticle->setVelocity(PhysicsEngine::Vector3(xVelocity, yVelocity, zVelocity));
	// Add gravity onto this circular particle
	newParticle->physicsParticle->setAcceleration(PhysicsEngine::Vector3(0.0f, -9.81f, 0.0f));
	newParticle->physicsParticle->setPosition(PhysicsEngine::Vector3(0.0f, 4.0f, 6.0f));
	newParticle->setColor(color);
	newParticle->setSize(size);
	newParticle->physicsParticle->setLifeTime(2.0f);
	return newParticle;
}

// Generate a new particle
RenderableParticle* CreateParticle::createParticle(RenderingDemo *demo, PhysicsEngine::Vector3 position, PhysicsEngine::real speed, PhysicsEngine::real size, PhysicsEngine::Vector3 color)
{
	RenderableParticle *newParticle = new RenderableParticle();
	PhysicsEngine::real xVelocity = ((PhysicsEngine::real)((rand() % 200) - 100)) / 100;
	PhysicsEngine::real yVelocity = ((PhysicsEngine::real)((rand() % 200) - 100)) / 100;
	PhysicsEngine::real zVelocity = ((PhysicsEngine::real)((rand() % 200) - 100)) / 100;
	xVelocity *= speed;
	yVelocity *= speed;
	zVelocity *= speed;
	newParticle->physicsParticle->setVelocity(PhysicsEngine::Vector3(xVelocity, yVelocity, zVelocity));
	newParticle->physicsParticle->setPosition(position);
	// Give the particle a ramp up time to make a GIF
	newParticle->physicsParticle->setStartupTime(0.1f);
	newParticle->setColor(color);
	newParticle->setSize(size);
	newParticle->physicsParticle->setLifeTime(3.0f);

	// Add the new particle to the physics and rendering engines
	demo->particles.push_back(newParticle);
	demo->physicsEngine.createParticle(newParticle->physicsParticle);
	demo->renderableObjects.push_back(newParticle);

	return newParticle;
}