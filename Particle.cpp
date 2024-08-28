#include "Particle.h"

void Particle::Update(float deltaTime)
{

	acceleration = totalLinearForce / mass; //force / mass = acceleration

	//friction = 0.2f; //changed this to a global variable.
	velocity = velocity + ((acceleration)*deltaTime);
	velocity *= pow(friction, deltaTime); //works like wind or drag or smth  // The lower it is the stronger it is?
	//velocity *= (1 - friction * deltaTime); //works like wind or drag or smth  //new func

	position = position + ((velocity)*deltaTime);	
	//ResetForce();

}

Particle::Particle()
{
	mass = 0.0f;
	glm::vec2 acceleration; //Three number data needed
	glm::vec2 velocity; //Three number data needed
	glm::vec2 totalLinearForce; //Three number data needed
	float friction = 0.0f; /* A NUMBER BETWEEN 0 and 1 */

	glm::vec2 position; //Three number data needed
	float time = 0.0f; //current time
	bool hasGravity = false;

	//bool isOutOfPlay = false; //This ball always needs to be in play
	//bool isPotted = false; //Take the correct ball out of comission
}

Particle::Particle(float m, float f, glm::vec2 pos)
{
	//All play apart in Newtons Laws
	mass = m;
	friction = f;
	velocity = glm::vec2(0, 0);
	acceleration = glm::vec2(0, 0);
	position = pos;
	totalLinearForce = glm::vec2(0, 0);
}

Particle::~Particle()
{
}

float Particle::GetMass()
{
	return mass;
}

glm::vec2 Particle::GetAcceleration()
{
	return acceleration;
}

float Particle::GetFriction()
{
	return friction; /* A NUMBER BETWEEN 0 and 1 */
}

glm::vec2 Particle::GetVelocity()
{
	return velocity;
}

glm::vec2 Particle::GetTotalForce()
{
	return totalLinearForce;
}

void Particle::ResetForce()
{
	totalLinearForce = glm::vec2(0, 0);
}

void Particle::AddForce(glm::vec2 force)
{
	totalLinearForce += force;
}