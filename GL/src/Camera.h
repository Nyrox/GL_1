#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:

	glm::mat4 getViewMatrix();

	glm::vec3 position;


	glm::vec3 forwards();
	glm::vec3 right();
};