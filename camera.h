#pragma once
#include <iostream>
#include <fstream>

#include <GL/glew.h>
#include <GL/freeglut.h>
#pragma comment(lib, "glew32.lib") 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <vector>
#include <map>
#include "shader.h"
#include "vertex.h"

using namespace std;
using namespace glm;

class Camera
{
protected:

public:

	glm::vec3 cameraPos = glm::vec3(17.5, -25.0, 17.5);
	glm::vec3 cameraFront = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 cameraUp = glm::vec3(0.0, 0.0, 1.0);

	glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f),glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	double zoom = 5.0;
	const float cameraSpeed = 0.1f; // adjust accordingly
	#pragma endregion 

	Camera();
	~Camera();
    glm::mat4 WhichCamera();
	void CameraMovement(glm::vec3, GLuint programID);
    void ViewMatCalc(GLuint);
    void ProjectionCamera(GLuint);
    void OrthoCam(GLuint);
};

