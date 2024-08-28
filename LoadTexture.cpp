#include "LoadTexture.h"
#include <vector>

LoadTexture::LoadTexture()
{
}

LoadTexture::~LoadTexture()
{
}

GLuint LoadTexture::Texture(const char* filename)
{
	GLuint textureID;

	// Generate texture ID
	glGenTextures(1, &textureID);

	glActiveTexture(GL_TEXTURE0 + textureID - 1);
	// Bind texture
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Load image
	int width, height;
	unsigned char* image = SOIL_load_image(filename, &width, &height, 0, SOIL_LOAD_RGB);

	// Set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Generate texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Free image data
	SOIL_free_image_data(image);

	// Unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);

	return textureID;
}

void LoadTexture::ChangeTextureOnGrass(std::vector<GameObject*> grassObjs)
{
	GLuint fungi      = Texture("Textures/funguy.png");
	GLuint green      = Texture("Textures/grass.png");
	GLuint fungicided = Texture("Textures/fungicided.png");

	for (int i = 0; i < grassObjs.size(); i++)
	{
		if (grassObjs[i]->HasFlag(alive))
		{
			grassObjs[i]->texID = fungi;
		}

		if (grassObjs[i]->HasFlag(dead))
		{
			grassObjs[i]->texID = green;
		}

		if (grassObjs[i]->HasFlag(infected))
		{
			grassObjs[i]->texID = fungicided;
		}
	}
}
