#pragma once
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include "vertex.h"
#include "Particle.h"
#include <map>

enum gameObjectflags {	
						base              = 1 << 0, 
					    alive             = 1 << 1,
						dead	          = 1 << 2,
						infected          = 1 << 3,
						gnome             = 1 << 4,
						infectedAndDead   = 1 << 5

					 };

class GameObject
{
private:


public:

	Particle* particlePTR;

	unsigned int flag;

	//keyboard input
	static std::map<char, bool> keys;
	static std::map<char, bool> specialKeys;

	GLuint buffer, vao;
	unsigned int indices, vertices;    // VBO for the sphere object
	VertexWtihNormal* verticesNor;     //Sphere vertices data with normals
	unsigned int* indicesPTR;          //Sphere triangle indices 
	int vertCount, indCount;
	glm::vec3 Colour;
	int timeStepCounter = 0;

	glm::vec3 pos3d = glm::vec3(0, 0, 0);


	int colourState = 0;
	GLuint texID;

	//OpenGL drawing function
	virtual void Draw(int modelMatLoc, int colourLoc, int texLoc);

	virtual void SendToGPU();

	///physics update function
	virtual void Update(float deltaTime);

	//Get particle collider 
	Particle* GetParticle();

	void AddFlag(gameObjectflags);
	void RemoveFlag(gameObjectflags flagsToRemove);
	bool HasFlag(gameObjectflags checkFlag) const;
	void ClearFlags();
	void ApplyFungicide();
	void HandleInfection();

	GameObject();
	GameObject(float m, float f, unsigned int flag, Particle* particlePTR);
	//GameObject(float m, float f, vec2 pos, string tag); //normal collider constructor
	~GameObject(); //this might need to be changed to a virtual function like draw and update
};

