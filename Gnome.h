#pragma once
#include <iostream>
#include <fstream>

#include <GL/glew.h>
#include <GL/freeglut.h>
#pragma comment(lib, "glew32.lib") 

#include "soil/SOIL.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

#include "Shader.h"
#include "vertex.h"

#include "Particle.h"
#include "GameObject.h"
#include <glm/gtx/string_cast.hpp>

#include "LoadTexture.h"

using namespace std;
using namespace glm;

class Gnome : public GameObject
{
private:
	unsigned int GnomeVAO, GnomeVBO;
	float speed;
	float mass;
	float friction;

public:

	int location;
	int colourLoc;

	std::string GnomeTexture = "Textures/funguy.png";
	std::string gnomeTexture = "Textures/nome.png";
	std::string grassTexture = "Textures/grass.png";
	std::string fungiTexture = "Textures/fungi.png";

	Gnome();
	Gnome(float m, float f, vec3 col, Particle* particlePTR);
	~Gnome();

	void InitialiseGnome();
	void Initialise(unsigned int programId, unsigned int textureID);
	void SetViewMatrix(unsigned int modelViewMatLoc, glm::mat4 modelViewMat);

};