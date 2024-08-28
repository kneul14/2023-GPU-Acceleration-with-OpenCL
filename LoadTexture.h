#pragma once
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "soil/SOIL.h"
#include <vector>

#include "Shader.h"
#include "vertex.h"

#include "GameObject.h"

class LoadTexture
{

public:
	LoadTexture();
	~LoadTexture();

	GLuint Texture(const char* filename);
	void   ChangeTextureOnGrass(std::vector<GameObject*> grassObjs);
};

