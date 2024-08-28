#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include "vertex.h"

using namespace std;
using namespace glm;

Camera::Camera()
{
}

Camera::~Camera()
{

}

glm::mat4 Camera::WhichCamera() // (eye_x, eye_y, eye_z) (center_x, center_y, center_z) (up_dx, up_dy, up_dz) //position //location //direction up
{
	mat4 viewMatrix;

	viewMatrix = lookAt(vec3(17.5, -25.0, 17.5), vec3(17.5, 0.0, 17.5), vec3(0.0, 0.0, 1.0));
		
	return viewMatrix;
}

void Camera::CameraMovement(glm::vec3 direction, GLuint programID)
{
	cameraPos += direction * cameraSpeed;
	ViewMatCalc(programID);
}

void Camera::ViewMatCalc(GLuint programID)
{
	glm::mat4 view = glm::lookAt( cameraPos, cameraPos + cameraFront, cameraUp);
	//cout << cameraPos.x << endl;
	glUniformMatrix4fv(programID, 1, GL_FALSE, value_ptr(view));

}

void Camera::ProjectionCamera(GLuint programID)
{
	glm::mat4 projMat;
	projMat = frustum(-zoom, zoom, -zoom, zoom, 5.0, 1000.0); /*perspective(radians(60.0), 1.0, 0.1, 1000.0); *///
	glUniformMatrix4fv(programID, 1, GL_FALSE, value_ptr(projMat));
}

void Camera::OrthoCam(GLuint programID)
{
	glm::mat4 projMat;
	projMat = glm::ortho(0.0f, 500.f, 500.f, 0.0f, 0.1f, 1.0f); //glm::perspective(60.0f, 500.f / 500.f, 0.1f, 100.0f); /*perspective(radians(60.0), 1.0, 0.1, 1000.0); *///
	glUniformMatrix4fv(programID, 1, GL_FALSE, value_ptr(projMat));	

}