#pragma once
#include "Shader.h"
#include "Application.h"
#include <glm/mat4x4.hpp>

#include <vector>
#include "Mesh.h"
#include "OBJMesh.h"
#include "Camera.h"

#include "Instance.h"
#include "Scene.h"
#include "ParticleEmitter.h"

class GraphicsProjectApp : public aie::Application {
protected:
	//camera
	Camera* m_camera;

	//since we are only displaying the models we dont need
	//multiple scenes
	Scene* m_scene;

	ParticleEmitter* m_emitter;

	// camera transforms
	glm::mat4	m_viewMatrix;
	glm::mat4	m_projectionMatrix;
	
	//shader
	aie::ShaderProgram m_simpleShader;
	aie::ShaderProgram m_phongShader;
	aie::ShaderProgram m_textureShader;
	aie::ShaderProgram m_normalMapShader;
	aie::ShaderProgram m_particleShader;
	//meshes
	aie::OBJMesh m_dragonMesh;
	aie::OBJMesh m_spearMesh;
	aie::OBJMesh m_gunMesh;

public:

	GraphicsProjectApp();
	virtual ~GraphicsProjectApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

	bool LoadShaderAndMeshLogic(Light a_light);

	//void DrawShaderAndMeshes(glm::mat4, glm::mat4);
	void ImguiLogic();

};

