#ifndef RENDER_PARTICLES_H
#define RENDER_PARTICLES_H

#include "MathDataTypes.h"
#include "Particle.h"
#include "RenderableObjects.h"
#include "DataTypeRedefinition.h"

// Position
// Size
// Velocity
// Color
// Shape?

namespace PhysicsDemo
{
	class RenderingDemo;

	class RenderableParticle : public Renderable
	{
	protected:
		real size;
		Vector3 color;
		real lifeTime;
		// The amount of time the particle waits to be displayed
		real startupTime;
		bool isDead = false;
		real timeAliveSoFar;
	public:
		RenderableParticle()
		{
			physicsParticle = new Particle();
			timeAliveSoFar = 0;
		}
		// Display this particle
		void display();
		// Function called when this particle dies
		virtual void onDeath();

		void setColor(Vector3 colorInput)
		{
			color = colorInput;
		}

		void setSize(real sizeInput)
		{
			size = sizeInput;
		}

		Particle *physicsParticle;
	};

	class CircleParticle : public RenderableParticle
	{
	public:
		// Display this particle
		virtual void display();
	};

	class FireworkParticle : public CircleParticle
	{
	private:
		RenderableParticle *deathParticles[5];
		int lives;
	public:
		// Create a new firework
		FireworkParticle(int livesInput)
		{
			lives = livesInput;
		}

		// Display this particle
		virtual void display();
		// Call this when the particle dies
		virtual void onDeath();
	};

	struct CreateParticle
	{
		// Generate a new firework
		static RenderableParticle *createFireWorkParticle(real speed, real size, Vector3 position, Vector3 color, bool originalFirework, int lives);
		// Generate a new particle
		static RenderableParticle* createParticle(RenderingDemo *demo, Vector3 position, real speed, real size, Vector3 color);
		// Generate a new circular particle
		static RenderableParticle* createCircularParticle(real speed, real size, Vector3 color);
	private:
		// These are private to stop instances being created: use get().
		CreateParticle() {}
		CreateParticle(const CreateParticle &) {}
		CreateParticle& operator=(const CreateParticle &);
	};

	




}


#endif //RENDER_PARTICLES_H