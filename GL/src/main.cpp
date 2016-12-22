#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <Camera.h>
#include <Shader.h>
#include <Texture.h>
#include <Cube.h>
#include <Plane.h>
#include <iostream>
#include <Mesh.h>
#include <2D/Sprite.h>

bool keys[1024];
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (action == GLFW_PRESS) {
		keys[key] = true;
		
		if (key == GLFW_KEY_ESCAPE) {
			exit(EXIT_SUCCESS);
		}
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
	camera.position.y = 2;

	glm::mat4 view = glm::translate(glm::mat4(), { 0, 0, -3 });
	glm::mat4 projection = glm::perspective(45.0f, 800.f / 600.f, 0.1f, 1000.f);

	Shader basicShader;
	basicShader.loadFromFile("shaders/basic.vert", "shaders/basic.frag");

	Shader flatShader;
	flatShader.loadFromFile("shaders/flat.vert", "shaders/flat.frag");


	Plane floor;
	floor.scale = 2;
	floor.material = Material(glm::vec3(0.8, 0.4, 0.31));
	floor.initRenderData();

	Cube light;
	light.position.x = 3.5;
	light.position.z = -3;
	light.position.y = 2;
	light.scale = 0.1f;
	light.initRenderData();

	glm::vec3 lightPos = light.position;

	Texture lampTexture;
	lampTexture.loadFromFile("assets/lamp_icon.png", GL_RGBA);
	Sprite lampIcon(1, 1, &lampTexture);

	Cube cube;
	cube.position.x -= 1.25;
	cube.position.z = 0;
	cube.position.y = 2;
	cube.material = Material(glm::vec3(0.8, 0.4, 0.31));
	cube.initRenderData();

	Mesh betterCube;
	betterCube.position.x = -1.25;
	betterCube.position.z = 3;
	betterCube.position.y = 2;

	betterCube.material = Material(glm::vec3(0.8, 0.4, 0.31));
	betterCube.loadFromFile("assets/cube.ply");

	Mesh cross;
	cross.position.x = -1.25;
	cross.position.z = 6;
	cross.position.y = 1;
	cross.material = Material(glm::vec3(0.8, 0.4, 0.31));
	cross.loadFromFile("assets/cross.ply");

	Texture texture;
	texture.loadFromFile("assets/container2.png", GL_RGBA);

	Texture specular;
	specular.loadFromFile("assets/container2_specular.png", GL_RGBA);

	Light lightMaterial = Light({}, { 0.2, 0.2, 0.2 }, { 0.5, 0.5, 0.5 }, { 0.5, 0.5, 0.5 });

	GLfloat deltaTime = 0;
	GLfloat lastFrame = 0;

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		/*
			Check for OpenGL errors
		*/
		for (GLenum err; (err = glGetError()) != GL_NO_ERROR;) {
			std::cout << err;
		}

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;


		// Physics
		GLfloat cameraSpeed = 5.f * deltaTime;
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
		if (keys[GLFW_KEY_E]) {
			camera.yaw += cameraSpeed * 25;
		}
		if (keys[GLFW_KEY_Q]) {
			camera.yaw -= cameraSpeed * 25;
		}

		// Globals
		basicShader.bind();
		basicShader.setUniform("cameraPos", camera.position);
		basicShader.setUniform("view", camera.getViewMatrix());
		basicShader.setUniform("projection", projection);
			
		
		
		// Bind Textures
		texture.bind(0);
		specular.bind(1);

		// Render
		light.position.z = sin(glfwGetTime()) * 2;
		lightMaterial.position = light.position;
		basicShader.setUniform("light", lightMaterial);


		glClearColor(0.05, 0.05, 0.05, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		floor.draw(basicShader);
		cube.draw(basicShader);
		betterCube.draw(basicShader);
		cross.draw(basicShader);

		lampIcon.position = light.position;


		flatShader.bind();
		flatShader.setUniform("view", camera.getViewMatrix());
		flatShader.setUniform("projection", projection);


		lampIcon.draw(flatShader);

		glfwSwapBuffers(window);
	}


	glfwTerminate();
}