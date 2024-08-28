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


using namespace std;
using namespace glm;

class StartScreen
{
private:
	unsigned int startVAO, startVBO;

public:
	StartScreen();
	~StartScreen();

	void InitialiseStartScreen(unsigned int vao, unsigned int vbo);
	void Initialise(unsigned int programId, unsigned int textureID);

	void Draw(unsigned int programId);
	void SetViewMatrix(unsigned int modelViewMatLoc, glm::mat4 modelViewMat);
	
};

