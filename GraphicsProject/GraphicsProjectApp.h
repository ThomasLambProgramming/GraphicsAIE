#pragma once
#include "Shader.h"
#include "Application.h"
#include <glm/mat4x4.hpp>

#include <vector>
#include "Mesh.h"
#include "OBJMesh.h"
#include "Camera.h"

class GraphicsProjectApp : public aie::Application {
public:

	GraphicsProjectApp();
	virtual ~GraphicsProjectApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

	bool LoadShaderAndMeshLogic();

	void DrawShaderAndMeshes(glm::mat4, glm::mat4);
	void ImguiLogic();

protected:
	//camera
	Camera m_camera;

	aie::Texture m_gridTexture;
	
	//shader
	aie::ShaderProgram m_simpleShader;
	aie::ShaderProgram m_phongShader;
	aie::ShaderProgram m_textureShader;
	aie::ShaderProgram m_normalMapShader;
	//

	aie::OBJMesh m_spearMesh;
	glm::mat4 m_spearTransform;

	Mesh m_quadMesh;
	glm::mat4 m_quadTransform;
	glm::vec4 meshColor = {1,0,0,1};
	
	//Objects below
	glm::vec3 bunnyColor = { 1,1,1 };
	glm::vec3 dragonColor = { 0,0,0};
	glm::vec3 LucyColor = { 0,1,0 };
	glm::vec3 BuddhaColor = { 0,0,1 };
	
	aie::OBJMesh m_bunnyMesh;
	glm::mat4 m_bunnyTransform;

	aie::OBJMesh m_dragonMesh;
	glm::mat4 m_dragonTransform;

	aie::OBJMesh m_lucyMesh;
	glm::mat4 m_lucyTransform;

	aie::OBJMesh m_buddhaMesh;
	glm::mat4 m_buddhaTransform;

	//this is for the modifying of the matrix
	glm::vec3 imguiPos;
	//this is to reset the position of the model to then be able to apply a position to it
	glm::mat4 resetMatrix = {
		1,1,1,1,
		1,1,1,1,
		1,1,1,1,
		0,0,0,1
	};
	
	struct Light
	{
		glm::vec3 direction;
		glm::vec3 color;
	};
	
	Light m_light;

	//this is the intensity of each color in the scene as this applys to every model
	//its a global kind of thing and controlls the rgb thing
	//this seems to be the color of the ambient light 
	glm::vec3 m_ambientLight;

	// camera transforms
	glm::mat4	m_viewMatrix;
	glm::mat4	m_projectionMatrix;
};
