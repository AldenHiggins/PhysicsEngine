#pragma once
#ifndef PLAYER_CONTROLS_H
#define PLAYER_CONTROLS_H

#include "MathDataTypes.h"

#define PLAYER_DEFAULT_SPEED 1.0f
#define PLAYER_DEFAULT_POSITION PhysicsEngine::Vector3(0.0f, 4.0f, 0.0f)

namespace PhysicsDemo
{
	class PlayerController 
	{
	private:
		PhysicsEngine::Vector3 position;
		PhysicsEngine::real yaw;
		PhysicsEngine::real pitch;
		PhysicsEngine::real speed;
		
	public:
		PlayerController()
		{
			position = PLAYER_DEFAULT_POSITION;
			speed = PLAYER_DEFAULT_SPEED;
		}

		// Controls
		void keyCheck(unsigned char key);
		void keyUpCheck(unsigned char key);

		// Getters and setters
		PhysicsEngine::real getSpeed();
		void setSpeed(PhysicsEngine::real newSpeed);

		PhysicsEngine::Vector3 getPosition();
		void setPosition(PhysicsEngine::Vector3 newPosition);

		PhysicsEngine::real getYaw();
		void setYaw(PhysicsEngine::real newYaw);

		PhysicsEngine::real getPitch();
		void setPitch(PhysicsEngine::real newPitch);
	};
}

#endif //PLAYER_CONTROLS_H