#include "Fungus.h"

Fungus::Fungus()
{

}

Fungus::Fungus(float m, float f, vec3 col, Particle* particlePTR) : GameObject(1.0f, 0.5f, flag, particlePTR)
{
	mass = m;
	friction = f;
	Colour = col;	
	//flag = destroyable;
	particlePTR->size = glm::vec2(1, 1);
	InitialiseFungus();
	SendToGPU();
}

Fungus::~Fungus()
{
	delete particlePTR;
}

void Fungus::InitialiseFungus()  //create the 3d object
{
#if 0
    static VertexForFungus FungusVertices[] =       //Fungus
    {
	
		//{vec3(-1, -1, -1),vec4(1,1,1,1),normalize(vec3(-1,-1,-1)),vec2(0,0)}, //0
		//{vec3(1, -1, -1),vec4(1,1,1,1),normalize(vec3(1,-1,-1)),vec2(0,0)}, //1
		//{vec3(-1, 1, -1),vec4(1,1,1,1),normalize(vec3(-1,1,-1)),vec2(0,0)}, //2
		//{vec3(1, 1, -1),vec4(1,1,1,1),normalize(vec3(1,1,-1)),vec2(0,0)}, //3
		//{vec3(-1, -1, 1),vec4(1,1,1,1),normalize(vec3(-1,-1,1)),vec2(0,0)}, //4
		//{vec3(1, -1, 1),vec4(1,1,1,1),normalize(vec3(1,-1,1)),vec2(0,0)}, //5
		//{vec3(-1, 1, 1),vec4(1,1,1,1),normalize(vec3(-1,1,1)),vec2(0,0)}, //6
		//{vec3(1, 1, 1),vec4(1,1,1,1),normalize(vec3(1,1,1)),vec2(0,0)}, //7		
		

		{vec3(1, 1, -1),vec4(1,1,1,1),normalize(vec3(1,1,-1)),vec2(0,0)}, //3
		{vec3(1, -1, -1),vec4(1,1,1,1),normalize(vec3(1,-1,-1)),vec2(0,0)}, //1
		{vec3(1, -1, 1),vec4(1,1,1,1),normalize(vec3(1,-1,1)),vec2(0,0)}, //5

		{vec3(1, 1, -1),vec4(1,1,1,1),normalize(vec3(1,1,-1)),vec2(0,0)}, //3
		{vec3(1, -1, 1),vec4(1,1,1,1),normalize(vec3(1,-1,1)),vec2(0,0)}, //5
		{vec3(1, 1, 1),vec4(1,1,1,1),normalize(vec3(1,1,1)),vec2(0,0)}, //7

		{vec3(-1, -1, -1),vec4(1,1,1,1),normalize(vec3(-1,-1,-1)),vec2(0,0)}, //0
		{vec3(-1, 1, -1),vec4(1,1,1,1),normalize(vec3(-1,1,-1)),vec2(0,0)}, //2
		{vec3(-1, 1, 1),vec4(1,1,1,1),normalize(vec3(-1,1,1)),vec2(0,0)}, //6

		{vec3(-1, -1, 1),vec4(1,1,1,1),normalize(vec3(-1,-1,1)),vec2(0,0)}, //4
		{vec3(-1, -1, -1),vec4(1,1,1,1),normalize(vec3(-1,-1,-1)),vec2(0,0)}, //0
		{vec3(-1, 1, 1),vec4(1,1,1,1),normalize(vec3(-1,1,1)),vec2(0,0)}, //6

		{vec3(-1, 1, -1),vec4(1,1,1,1),normalize(vec3(-1,1,-1)),vec2(0,0)}, //2
		{vec3(1, 1, -1),vec4(1,1,1,1),normalize(vec3(1,1,-1)),vec2(0,0)}, //3
		{vec3(-1, 1, 1),vec4(1,1,1,1),normalize(vec3(-1,1,1)),vec2(0,0)}, //6

		{vec3(-1, 1, 1),vec4(1,1,1,1),normalize(vec3(-1,1,1)),vec2(0,0)}, //6
		{vec3(1, 1, -1),vec4(1,1,1,1),normalize(vec3(1,1,-1)),vec2(0,0)}, //3
		{vec3(1, 1, 1),vec4(1,1,1,1),normalize(vec3(1,1,1)),vec2(0,0)}, //7

		{vec3(1, -1, -1),vec4(1,1,1,1),normalize(vec3(1,-1,-1)),vec2(0,0)}, //1
		{vec3(-1, -1, 1),vec4(1,1,1,1),normalize(vec3(-1,-1,1)),vec2(0,0)}, //4
		{vec3(1, -1, 1),vec4(1,1,1,1),normalize(vec3(1,-1,1)),vec2(0,0)}, //5

		{vec3(1, -1, -1),vec4(1,1,1,1),normalize(vec3(1,-1,-1)),vec2(0,0)}, //1
		{vec3(-1, -1, 1),vec4(1,1,1,1),normalize(vec3(-1,-1,1)),vec2(0,0)}, //4
		{vec3(-1, -1, -1),vec4(1,1,1,1),normalize(vec3(-1,-1,-1)),vec2(0,0)}, //0

		{vec3(-1, -1, -1),vec4(1,1,1,1),normalize(vec3(-1,-1,-1)),vec2(0,0)}, //0
		{vec3(1, -1, -1),vec4(1,1,1,1),normalize(vec3(1,-1,-1)),vec2(0,0)}, //1
		{vec3(-1, 1, -1),vec4(1,1,1,1),normalize(vec3(-1,1,-1)),vec2(0,0)}, //2

		{vec3(-1, 1, -1),vec4(1,1,1,1),normalize(vec3(-1,1,-1)),vec2(0,0)}, //2
		{vec3(1, -1, -1),vec4(1,1,1,1),normalize(vec3(1,-1,-1)),vec2(0,0)}, //1
		{vec3(1, 1, -1),vec4(1,1,1,1),normalize(vec3(1,1,-1)),vec2(0,0)}, //3

		{vec3(1, -1, 1),vec4(1,1,1,1),normalize(vec3(1,-1,1)),vec2(0,0)}, //5
		{vec3(-1, -1, 1),vec4(1,1,1,1),normalize(vec3(-1,-1,1)),vec2(0,0)}, //4
		{vec3(-1, 1, 1),vec4(1,1,1,1),normalize(vec3(-1,1,1)),vec2(0,0)}, //6

		{vec3(1, -1, 1),vec4(1,1,1,1),normalize(vec3(1,-1,1)),vec2(0,0)}, //5
		{vec3(-1, 1, 1),vec4(1,1,1,1),normalize(vec3(-1,1,1)),vec2(0,0)}, //6
		{vec3(1, 1, 1),vec4(1,1,1,1),normalize(vec3(1,1,1)),vec2(0,0)}, //7

	}; 
	vertCount = 4;
	indCount = 6;

	//glGenVertexArrays(1, &FungusVAO);
	//glGenBuffers(1, &FungusVBO);

	////FungusVAO = vao;
 //   //FungusVBO = vbo;
	//glBindVertexArray(FungusVAO);
	//glBindBuffer(GL_ARRAY_BUFFER, FungusVBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(VertexForFungus) * 36, FungusVertices, GL_STATIC_DRAW);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(FungusVertices[0]), 0);  //layout(location=0) in vec4 fieldCoords;
	//glEnableVertexAttribArray(0);
	//glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(FungusVertices[0]), (void*)(sizeof(GLfloat)*3));  //layout(location=1) in vec2 fieldTexCoords;
	//glEnableVertexAttribArray(1);
	//glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(FungusVertices[0]), (void*)(sizeof(GLfloat) * 7));  //layout(location=1) in vec2 fieldTexCoords;
	//glEnableVertexAttribArray(2); 
	//glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(FungusVertices[0]), (void*)(sizeof(GLfloat) * 10)); //layout(location=1) in vec2 fieldTexCoords;
	//glEnableVertexAttribArray(3);
#else
	//{
	//	{ vec3(1, 1, -1), vec4(1, 1, 1, 1), normalize(vec3(1, 1, -1)), vec2(0, 0)}, //3
	//	{ vec3(1, -1, -1),vec4(1,1,1,1),normalize(vec3(1,-1,-1)),vec2(0,0) }, //1
	//	{ vec3(1, -1, 1),vec4(1,1,1,1),normalize(vec3(1,-1,1)),vec2(0,0) }, //5
	//};

	verticesNor = (VertexWtihNormal*)malloc(sizeof(VertexWtihNormal) * 4 * 6);  //allocating memory for verticies 4 verts times 6 faces
	indicesPTR = (unsigned int*)malloc(sizeof(unsigned int) * 2 * 3 * 6);		//allocating memory for indicies

	// this points the normals the right way which is up
	for (int i = 0; i < 4; i++)
	{
		verticesNor[i+0].normals  = vec3(0, 1, 0);   //top
		verticesNor[i+4].normals  = vec3(0, -1, 0);  //bottom

		verticesNor[i+8].normals  = vec3(1, 0, 0);   //right
		verticesNor[i+12].normals = vec3(-1, 0, 0); //left
		
		verticesNor[i+20].normals = vec3(0, 0, 1); //front
		verticesNor[i+16].normals = vec3(0, 0, -1);  //back
	}

	// verticies setup for the first face
	//top
	verticesNor[0].coords = vec4( -particlePTR->size.x,   particlePTR->size.y,  1, 2) / 2.0f;	   //top left
	verticesNor[1].coords = vec4(  particlePTR->size.x,   particlePTR->size.y,  1, 2) / 2.0f;	   //top right
	verticesNor[2].coords = vec4(  particlePTR->size.x,   particlePTR->size.y, -1, 2) / 2.0f;	   //bottom right
	verticesNor[3].coords = vec4( -particlePTR->size.x,   particlePTR->size.y, -1, 2) / 2.0f;	   //bottom left
	//bottom										
	verticesNor[4].coords = vec4( -particlePTR->size.x,  -particlePTR->size.y,  1, 2) / 2.0f;	   //top left
	verticesNor[5].coords = vec4(  particlePTR->size.x,  -particlePTR->size.y,  1, 2) / 2.0f;	   //top right
	verticesNor[6].coords = vec4(  particlePTR->size.x,  -particlePTR->size.y, -1, 2)/ 2.0f;	   //bottom right
	verticesNor[7].coords = vec4( -particlePTR->size.x,  -particlePTR->size.y, -1, 2) / 2.0f;	   //bottom left
	//right											
	verticesNor[8].coords =  vec4( particlePTR->size.x,   particlePTR->size.y, -1, 2) / 2.0f;	   //top left
	verticesNor[9].coords =  vec4( particlePTR->size.x,   particlePTR->size.y,  1, 2) / 2.0f;	   //top right
	verticesNor[10].coords = vec4( particlePTR->size.x,  -particlePTR->size.y,  1, 2) / 2.0f;	   //bottom right
	verticesNor[11].coords = vec4( particlePTR->size.x,  -particlePTR->size.y, -1, 2) / 2.0f;	   //bottom left
	//left											
	verticesNor[12].coords = vec4(-particlePTR->size.x,   particlePTR->size.y, -1, 2) / 2.0f;	   //top left
	verticesNor[13].coords = vec4(-particlePTR->size.x,   particlePTR->size.y,  1, 2) / 2.0f;	   //top right
	verticesNor[14].coords = vec4(-particlePTR->size.x,  -particlePTR->size.y,  1, 2) / 2.0f;	   //bottom right
	verticesNor[15].coords = vec4(-particlePTR->size.x,  -particlePTR->size.y, -1, 2) / 2.0f;	   //bottom left
	//front						   					
	verticesNor[16].coords = vec4(-particlePTR->size.x,   particlePTR->size.y,  1, 2) / 2.0f;	   //top left
	verticesNor[17].coords = vec4( particlePTR->size.x,   particlePTR->size.y,  1, 2) / 2.0f;	   //top right
	verticesNor[18].coords = vec4( particlePTR->size.x,  -particlePTR->size.y,  1, 2) / 2.0f;	   //bottom right
	verticesNor[19].coords = vec4(-particlePTR->size.x,  -particlePTR->size.y,  1, 2) / 2.0f;	   //bottom left
	//back						   					
	verticesNor[20].coords = vec4(-particlePTR->size.x,   particlePTR->size.y, -1, 2) / 2.0f;	   //top left
	verticesNor[21].coords = vec4( particlePTR->size.x,   particlePTR->size.y, -1, 2) / 2.0f;	   //top right
	verticesNor[22].coords = vec4( particlePTR->size.x,  -particlePTR->size.y, -1, 2) / 2.0f;	   //bottom right
	verticesNor[23].coords = vec4(-particlePTR->size.x,  -particlePTR->size.y, -1, 2) / 2.0f;	   //bottom left

	for (int i = 0; i < 6; i++)
	{
		verticesNor[i * 4].texcoords     = vec2(0, 0);
		verticesNor[i * 4 + 1].texcoords = vec2(1, 0);
		verticesNor[i * 4 + 2].texcoords = vec2(1, 1);
		verticesNor[i * 4 + 3].texcoords = vec2(0, 1);

	}
		//verticesNor[4].texcoords = vec2(0, 0);
		//verticesNor[5].texcoords = vec2(1, 0);
		//verticesNor[6].texcoords = vec2(1, 1);
		//verticesNor[7].texcoords = vec2(0, 1);

	for (int i = 0; i < 6; i++)
	{
		int indOffset = i * 6;
		int vertOffset = i * 4;

		// Indices setup
		indicesPTR[0+indOffset] = 0+vertOffset;
		indicesPTR[1+indOffset] = 1+vertOffset;
		indicesPTR[2+indOffset] = 2+vertOffset;
					
		indicesPTR[3+indOffset] = 3+vertOffset;
		indicesPTR[4+indOffset] = 2+vertOffset;
		indicesPTR[5+indOffset] = 0+vertOffset;

	}
	vertCount = 4 * 6;
	indCount = 2 * 3 * 6;

#endif
}

void Fungus::Initialise(unsigned int programId, unsigned int textureID) 
{
	glActiveTexture(GL_TEXTURE0 + textureID - 1);
	glBindTexture(GL_TEXTURE_2D, textureID);

	int width, height;

	unsigned char* data = SOIL_load_image(/*fungusTextures[colourState]*/grassTexture.c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	SOIL_free_image_data(data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

}

void Fungus::SetViewMatrix(unsigned int modelViewMatLoc, glm::mat4 modelViewMat)
{
	glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));
}
