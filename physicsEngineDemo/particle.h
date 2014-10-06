#ifndef PARTICLE_H
#define PARTICLE_H

#include <GL/glut.h>

#define WORLDBOUNDS 22.0f

class Particle{
public:
	Particle(GLfloat positionInput[3], GLfloat sizeInput, GLfloat colorInput[4]);
	~Particle();

	void draw();
	void update();

	GLfloat position[3];
	GLfloat velocity[3];
	GLfloat acceleration[3];

	GLfloat color[4];
	
	GLfloat size;
private:
};

#endif // PARTICLE_H