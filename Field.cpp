#include "Field.h"

Field::Field()
{
}

Field::~Field()
{
}

void Field::InitialiseField(unsigned int vao, unsigned int vbo)
{
	static Vertex fieldVertices[] =       //START SCREEN
	{
		{vec4(100.0, -5.0, 100.0, 1.0), vec2(8.0, 0.0)},  //bottom right
		{vec4(100.0, -5.0, -100.0, 1.0), vec2(8.0, 8.0)}, //top right
		{vec4(-100.0, -5.0, 100.0, 1.0), vec2(0.0, 0.0)}, //bottom left
		{vec4(-100.0, -5.0, -100.0, 1.0), vec2(0.0, 8.0)} //top left
	};
	fieldVAO = vao;
	fieldVBO = vbo;
	glBindVertexArray(fieldVAO);
	glBindBuffer(GL_ARRAY_BUFFER, fieldVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(fieldVertices), fieldVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(fieldVertices[0]), 0);  //layout(location=0) in vec4 fieldCoords;
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(fieldVertices[0]), (void*)(sizeof(fieldVertices[0].coords)));  //layout(location=1) in vec2 fieldTexCoords;
	glEnableVertexAttribArray(1);
}

void Field::Initialise(unsigned int programId, unsigned int textureID)
{
	std::string fieldTextures[] =
	{
		"Textures/grass.bmp",
	};

	glActiveTexture(GL_TEXTURE0 + textureID - 1);
	glBindTexture(GL_TEXTURE_2D, textureID);

	int width, height;

	unsigned char* data = SOIL_load_image(fieldTextures[0].c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	SOIL_free_image_data(data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	unsigned int grassTexLoc = glGetUniformLocation(programId, "grassTex");
	glUniform1i(grassTexLoc, 0); //send texture to shader
}

void Field::Draw(unsigned int programId)
{
	glBindVertexArray(fieldVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void Field::SetViewMatrix(unsigned int modelViewMatLoc, glm::mat4 modelViewMat)
{
	glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));
}
