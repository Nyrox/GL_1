#pragma once
#include <Framebuffer.h>
#include <Core/Camera.h>
#include <Shader.h>
#include <Mesh.h>
#include <Light.h>

#include <list>
#include <vector>
#include <Core\Skybox.h>

class Renderer {
public:
	Renderer(float backbuffer_width, float backbuffer_height);
	
	void render();

	void addPointLight(PointLight light);
	void addDirectionalLight(DirectionalLight light);

	std::list<Mesh> meshes;
	std::list<Mesh> transparent;

	Camera* camera;
	glm::mat4 projection;
private:
	Skybox skybox;
	Framebuffer postProcessBuffer;

	Shader post_process_shader;
	Shader shadow_pass_shader;
	Shader dirLightShadowPassShader;

	Shader forward_render_shader;	

	std::vector<PointLight> point_lights;
	std::vector<Framebuffer> shadow_maps;
	
	std::vector<DirectionalLight> directional_lights;
	std::vector<Framebuffer> directional_shadow_maps;

};