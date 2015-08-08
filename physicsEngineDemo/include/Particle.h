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
		Vector3 acceleration;
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
		virtual void display();
		// Getters and setters for particle data
		Vector3 getPosition();
		void setPosition(Vector3 newPosition);
		Vector3 getVelocity();
		void setVelocity(Vector3 newVelocity);
		Vector3 getAcceleration();
		void setAcceleration(Vector3 newAcceleration);
		real getSize();
		void setSize(real newSize);
		Vector3 getColor();
		void setColor(Vector3 newColor);
		real getLifeTime();
		void setLifeTime(real newLifeTime);
		bool getIsDead();
	};

	class CircleParticle : public Particle
	{
	public:
		// Display this particle
		virtual void display();
	};





}


#endif //PARTICLE_H