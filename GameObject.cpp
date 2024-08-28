#include "GameObject.h"
#include <fstream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_inverse.hpp>

std::map<char, bool> GameObject::keys;
std::map<char, bool> GameObject::specialKeys;

GameObject::GameObject()
{
	
}

GameObject::GameObject(float m, float f, unsigned int flag, Particle* P) //need to change it so that the particle passes through the constructor instead of being made within.
{    
    this->flag = flag;
	particlePTR = P;
}

GameObject::~GameObject()
{
    delete particlePTR;
}

void GameObject::AddFlag(gameObjectflags newFlags)
{
	flag = flag | newFlags;
}

void GameObject::RemoveFlag(gameObjectflags flagsToRemove)
{
	flag = flag & ~flagsToRemove;
}

void GameObject::ClearFlags()
{
	flag = 0;
}

void GameObject::ApplyFungicide()
{
	this->RemoveFlag(alive);
	this->AddFlag(dead);
	this->AddFlag(infected);
	this->timeStepCounter = 5;
}

void GameObject::HandleInfection() {
	if (HasFlag(infected)) {
		timeStepCounter--;

		if (timeStepCounter <= 0) {
			RemoveFlag(infected);
		}
	}
}

bool GameObject::HasFlag(gameObjectflags checkFlag) const
{
	return (flag & checkFlag) != 0;
}

void GameObject::Draw(int modelMatLoc, int colourLoc, int texLoc)
{	
	glm::mat4 modelMat = glm::mat4(1.0);
	//modelMat = glm::scale(modelMat, vec3(3, 3, 3)); //changes the scale

	pos3d = glm::vec3(particlePTR->position.x, 0, particlePTR->position.y);
	modelMat = glm::translate(modelMat, pos3d);

	glUniformMatrix4fv(modelMatLoc, 1, GL_FALSE, value_ptr(modelMat));
	glUniform4fv(colourLoc, 1, glm::value_ptr(Colour));

	// Do texture uniform using texID	
	glUniform1i(texLoc, texID - 1); //send texture to shader
	glActiveTexture(GL_TEXTURE0 + (texID - 1));
	glBindTexture(GL_TEXTURE_2D, texID);

	glBindVertexArray(vao);

	glDrawElements(GL_TRIANGLES, indCount, GL_UNSIGNED_INT, (void*)0);  ///use index array to control triangle drawing
}

void GameObject::SendToGPU()
{
	//Create VAO and VBO
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vertices);
	glGenBuffers(1, &indices);

	//Binding VAO 
	glBindVertexArray(vao);
	//Binding VBO
	glBindBuffer(GL_ARRAY_BUFFER, vertices);
	//creates a new data store for the buffer object currently bound to GL_ARRAY_BUFFER
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexWtihNormal) * vertCount, verticesNor, GL_STATIC_DRAW);
	//Binding Vertex array indices

	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(verticesNor[0]), 0);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(verticesNor[0]), (GLvoid*)sizeof(verticesNor[0].normals));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(verticesNor[0]), (GLvoid*)offsetof(VertexWtihNormal, texcoords));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices);
	//creates a new data store for the buffer object currently bound to GL_ELEMENT_ARRAY_BUFFER
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indCount, indicesPTR, GL_STATIC_DRAW);

}

Particle* GameObject::GetParticle()
{
    return particlePTR;
}

void GameObject::Update(float deltaTime)
{

}

