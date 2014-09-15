#include "particle.h"
#include <iostream>

static GLfloat roomBound = WORLDBOUNDS;

Particle::Particle(GLfloat positionInput[3], GLfloat sizeInput, GLfloat colorInput[4])
{
	position[0] = positionInput[0];
	position[1] = positionInput[1];
	position[2] = positionInput[2];
	size = sizeInput;
	color[0] = colorInput[0];
	color[1] = colorInput[1];
	color[2] = colorInput[2];
	color[3] = colorInput[3];

	velocity[0] = 0;
	velocity[1] = 0;
	velocity[2] = 0;

	acceleration[0] = 0;
	acceleration[1] = 0;
	acceleration[2] = 0;
}

Particle::~Particle()
{

}

// Perform the necessary updates and then draw the particle
void Particle::draw()
{
	glPushMatrix();
	update();
	glColor3f(.2f, .2f, .2f);
	//glMaterialfv(GL_FRONT, GL_DIFFUSE, color);
	//std::cout << "Drawing a particle here: \n";
	//std::cout << position[0] << "\n";
	//std::cout << position[1] << "\n";
	//std::cout << position[2] << "\n";
	glTranslatef(position[0], position[1], position[2]);
	glutSolidSphere(size, 10, 10);
	//glutSolidCube(size);
	glPopMatrix();
}

void Particle::update()
{
	velocity[0] += acceleration[0];
	velocity[1] += acceleration[1];
	velocity[2] += acceleration[2];

	position[0] += velocity[0];
	position[1] += velocity[1];
	position[2] += velocity[2];


	// Keep the particle within the bounds of the room
	if (position[2] > roomBound)
	{
		position[2] = 0;
	}
	else if (position[2] < 0)
	{
		position[2] = roomBound;
	}

	if (position[1] > roomBound)
	{
		position[1] = -roomBound;
	}
	else if (position[1] < -roomBound)
	{
		position[1] = roomBound;
	}

	if (position[0] > roomBound)
	{
		position[0] = -roomBound;
	}
	else if (position[0] < -roomBound)
	{
		position[0] = roomBound;
	}
}
