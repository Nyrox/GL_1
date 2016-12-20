#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <Camera.h>
#include <Shader.h>
#include <Cube.h>

#include <iostream>

bool keys[1024];
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (action == GLFW_PRESS) {
		keys[key] = true;
	}
	else if (action == GLFW_RELEASE) {
		keys[key] = false;
	}
}

int main() {
	// GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	
	// Context
	GLFWwindow* window = glfwCreateWindow(800, 600, "Hi", nullptr, nullptr);
	if (window == nullptr) {
		std::cout << "Failed to create window." << std::endl;
		std::cin.get();
	}
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);

	glewExperimental = true;
	GLenum err = glewInit();
	if (err != GLEW_OK) {
		std::cout << glewGetErrorString(err) << std::endl;
		std::cin.get();
	}

	glEnable(GL_DEPTH_TEST);

	glViewport(0, 0, 800, 600);

	Camera camera;
	camera.position.z = 5;

	glm::mat4 view = glm::translate(glm::mat4(), { 0, 0, -3 });
	glm::mat4 projection = glm::perspective(45.0f, 800.f / 600.f, 0.1f, 100.f);

	Shader basicShader;
	basicShader.loadFromFile("shaders/basic.vert", "shaders/basic.frag");
	basicShader.bind();

	
	Cube light;
	light.position.x = 0.25;
	light.position.z = -3;
	light.initRenderData();

	glm::vec3 lightPos = light.position;

	Cube cube;
	cube.position.x -= 1.25;
	cube.position.z = 0;
	cube.color = {0.8, 0.4f, 0.31f};
	cube.initRenderData();



	basicShader.setUniform("lightColor", glm::vec3(1.0, 1, 1));
	basicShader.setUniform("lightPos", light.position);
	basicShader.setUniform("cameraPos", { 0, 0, 3 });

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		// Physics
		GLfloat cameraSpeed = 0.03f;
		if (keys[GLFW_KEY_W]) {
			camera.position += camera.forwards() * cameraSpeed;
		}
		if (keys[GLFW_KEY_S]) {
			camera.position -= camera.forwards() * cameraSpeed;
		}
		if (keys[GLFW_KEY_D]) {
			camera.position += camera.right() * cameraSpeed;
		}
		if (keys[GLFW_KEY_A]) {
			camera.position -= camera.right() * cameraSpeed;
		}


		// Render
		light.position.z = sin(glfwGetTime());
		basicShader.setUniform("lightPos", light.position);


		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		basicShader.bind();
		basicShader.setUniform("view", camera.getViewMatrix());
		basicShader.setUniform("projection", projection);
		
		cube.draw(basicShader);
		light.draw(basicShader);
		
		glfwSwapBuffers(window);
	}


	glfwTerminate();
}