#pragma once
#include <glm/glm.hpp>

class Particle
{
private:

public:
    glm::vec2 velocity = glm::vec2(0,0); //Three number data needed
    glm::vec2 position = glm::vec2(0,0); //Three number data needed
    float mass; //Normally One number, but can contain decimal place values.
    glm::vec2 acceleration; //Three number data needed
    glm::vec2 totalLinearForce; //Three number data needed
    float friction     = 0.3f; /* A NUMBER BETWEEN 0 and 1 */

    float time         = 0.0f; //current time
    float radius       = 0.5f;
    glm::vec2 size     = glm::vec2(1, 1);
    //bool hasGravity  = false;
    
    Particle();
    Particle(float m, float f, glm::vec2 pos);
    ~Particle();
    /////physics update function
    virtual void Update(float);
    float GetMass();
    float GetFriction();
    glm::vec2 GetAcceleration();
    glm::vec2 GetVelocity();
    glm::vec2 GetTotalForce();
    void ResetForce();
    void AddForce(glm::vec2);
};

