#pragma once
#ifndef PLAYER_CONTROLS_H
#define PLAYER_CONTROLS_H

class PhysicsEngine::Vector3;
PhysicsEngine::real;

#define PLAYER_DEFAULT_SPEED 1.0f

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
		PlayerController(PhysicsEngine::Vector3 initialPosition)
		{
			position = initialPosition;
			speed = PLAYER_DEFAULT_SPEED;
		}

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