#ifndef PARTICLE_H
#define PARTICLE_H

#include "MathDataTypes.h"

// Position
// Size
// Velocity
// Color
// Shape?

namespace PhysicsEngine
{
	class Particle
	{
	protected:
		Vector3 position;
		Vector3 velocity;
		real size;
		Vector3 color;
		real lifeTime;
		bool isDead;
		real timeAliveSoFar;
	public:
		Particle()
		{
			timeAliveSoFar = 0;
		}
		// Integrate the particle forward in time by the timestep
		void integrate(real timeStep);
		// Display this particle
		void display();
		// Getters and setters for particle data
		Vector3 getPosition();
		void setPosition(Vector3 newPosition);
		Vector3 getVelocity();
		void setVelocity(Vector3 newVelocity);
		real getSize();
		void setSize(real newSize);
		Vector3 getColor();
		void setColor(Vector3 newColor);
		real getLifeTime();
		void setLifeTime(real newLifeTime);
		bool getIsDead();
	};





}


#endif //PARTICLE_H