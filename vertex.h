#ifndef VERTEX_H
#define VERTEX_H

#include <glm/glm.hpp>

struct Vertex
{
   glm::vec4 coords;
   glm::vec2 texCoords;
};

struct VertexWtihNormal
{
	glm::vec4 coords;
	glm::vec3 normals;
	glm::vec2 texcoords;
};

struct  VertexWithAll
{
	glm::vec3 position;
	glm::vec3 color;
	glm::vec2 textcoord;
	glm::vec3 normal;
};

//struct  VertexForFungus
//{
//	glm::vec3 position;
//	glm::vec4 colour;
//	glm::vec3 normal;
//	glm::vec2 textcoord;
//};
#endif
