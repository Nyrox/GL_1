#include "Sprite.h"
#include <glm/gtc/matrix_transform.hpp>

Sprite::Sprite(uint32_t _width, uint32_t _height, Texture* texture) : width(width), height(height), texture(texture), yaw(0) {

	initRenderData();
}

void Sprite::draw(Shader& shader) {
	shader.bind();

	shader.setUniform("model", glm::rotate(glm::translate(glm::mat4(), position), yaw, { 0, 1, 0 }));
	shader.setUniform("diffuse", 0);

	texture->bind(0);

	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

void Sprite::initRenderData() {
	GLuint vbo;

	GLfloat vertices[] = {
		0, 0, 0,			0, 0, 1,	0, 0,
		width, 0, 0,		0, 0, 1,	1, 0,
		0, height, 0,		0, 0, 1,	0, 1,
		width, height, 0,	0, 0, 1,	1, 1
	};

	glGenVertexArrays(1, &this->vao);
	glGenBuffers(1, &vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(this->vao);
	
	// Positions
	glVertexAttribPointer(0, 3, GL_FLOAT, 0, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Normals
	glVertexAttribPointer(0, 3, GL_FLOAT, 0, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);

	// UVS
	glVertexAttribPointer(0, 2, GL_FLOAT, 0, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
}