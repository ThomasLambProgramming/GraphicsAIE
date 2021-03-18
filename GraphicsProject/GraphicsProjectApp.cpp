#include "GraphicsProjectApp.h"
#include "Gizmos.h"
#include "Input.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "imgui.h"

#include "Scene.h"
#include "Instance.h"

using glm::vec3;
using glm::vec4;
using glm::mat4;
using aie::Gizmos;

GraphicsProjectApp::GraphicsProjectApp() {

}

GraphicsProjectApp::~GraphicsProjectApp() {
	
}

bool GraphicsProjectApp::startup() {
	
	setBackgroundColour(0.25f, 0.25f, 0.25f);

	// initialise gizmo primitive counts
	Gizmos::create(10000, 10000, 10000, 10000);

	// create simple camera transforms
	m_viewMatrix = glm::lookAt(vec3(10), vec3(0), vec3(0, 1, 0));
	m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f, (float)getWindowWidth() / (float)getWindowHeight(), 0.1f, 1000.0f);

	Light light;
	light.m_color = { 1,1,1 };
	light.m_direction = { 1, -1,1 };
	
	return LoadShaderAndMeshLogic(light);
}

void GraphicsProjectApp::shutdown() {

	Gizmos::destroy();
	delete m_scene;
}

void GraphicsProjectApp::update(float deltaTime) {

	// wipe the gizmos clean for this frame
	Gizmos::clear();

	// draw a simple grid with gizmos
	vec4 white(1);
	vec4 black(0, 0, 0, 1);
	for (int i = 0; i < 21; ++i) {
		Gizmos::addLine(vec3(-10 + i, 0, 10),
						vec3(-10 + i, 0, -10),
						i == 10 ? white : black);
		Gizmos::addLine(vec3(10, 0, -10 + i),
						vec3(-10, 0, -10 + i),
						i == 10 ? white : black);
	}
	ImguiLogic();
	float time = getTime();
	
	// add a transform so that we can see the axis
	Gizmos::addTransform(mat4(1));


	m_scene->GetLight().m_direction = glm::normalize(glm::vec3(glm::cos(time * 2), glm::sin(time * 2), 0));
	
	m_camera.Update(deltaTime);
	
	aie::Input* input = aie::Input::getInstance();

	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void GraphicsProjectApp::draw() {

	// wipe the screen to the background colour
	clearScreen();
	glm::mat4 projectionMatrix = m_camera.GetProjectionMatrix(getWindowWidth(), getWindowHeight());
	glm::mat4 viewMatrix = m_camera.GetViewMatrix();

	// update perspective based on screen size
	//m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f, getWindowWidth() / (float)getWindowHeight(), 0.1f, 1000.0f);
	//DrawShaderAndMeshes(projectionMatrix, viewMatrix);
		
	Gizmos::draw(projectionMatrix * viewMatrix);
	m_scene->Draw();
}
bool GraphicsProjectApp::LoadShaderAndMeshLogic(Light a_light)
{
	/*
#pragma  region Quad

	//load the vertex shader from a file
	m_simpleShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/simple.vert");

	//load the fragment shader from a file
	m_simpleShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/simple.frag");

	if (!m_simpleShader.link())
	{
		printf("yeah the fragment or vertex failed good job this didnt load");
		return false;
	}
	//setting up all the positions for the 4 corners of the quad
	Mesh::Vertex vertices[4];
	vertices[0].position = { -0.5f, 0.f, 0.5f, 1.0f };
	vertices[1].position = { 0.5f, 0.f, 0.5f, 1.0f };
	vertices[2].position = { -0.5f, 0.f,-0.5f, 1.0f };
	vertices[3].position = { 0.5f, 0.f,-0.5f, 1.0f }; 
	//where each triangle should be drawn relative to the vertices
	unsigned int indices[6] = { 0,1,2,2,1,3 };

	
	m_quadMesh.InitialiseQuad();
	//set the scale of the quad to be 10 by 10 units
	m_quadTransform = {
		10,0,0,0,
		0,10,0,0,
		0,0,10,0,
		0,0,0,1
	};
	
#pragma  endregion 
#pragma region  Bunny
	if (m_bunnyMesh.load("./stanford/Bunny.obj") == false)
	{
		printf("REEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE bunny didnt load u idiot");
		return false;
	}
	m_bunnyTransform = {
		0.5f,0,0,0,
		0,0.5f,0,0,
		0,0,0.5f,0,
		3,0,-3,1
	};
#pragma endregion
#pragma region Dragon
	if (m_dragonMesh.load("./stanford/Dragon.obj") == false)
	{
		printf("REEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE dragon didnt load u idiot");
		return false;
	}
	m_dragonTransform = {
		0.5f,0,0,0,
		0,0.5f,0,0,
		0,0,0.5f,0,
		3,0,3,1
	};
#pragma endregion
#pragma region Lucy
	if (m_lucyMesh.load("./stanford/Lucy.obj") == false)
	{
		printf("REEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE Lucy didnt load u idiot");
		return false;
	}
	m_lucyTransform = {
		0.5f,0,0,0,
		0,0.5f,0,0,
		0,0,0.5f,0,
		-3,0,-3,1
	};
#pragma endregion
#pragma region Budda
	if (m_buddhaMesh.load("./stanford/Buddha.obj") == false)
	{
		printf("REEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE Buddha didnt load u idiot");
		return false;
	}
	m_buddhaTransform = {
		0.5f,0,0,0,
		0,0.5f,0,0,
		0,0,0.5f,0,
		-3,0,3,1
	};
#pragma endregion 
	*/
#pragma region Spear
	//give file name, does it have textures, do we want to flip v (if it returns false no load)
	if (m_spearMesh.load("./soulspear/soulspear.obj", true, true) == false)
	{
		printf("Soul spear Mesh Failed!\n");
		return false;
	}
	m_spearTransform = {
		0.5f,0,0,0,
		0,0.5f,0,0,
		0,0,0.5f,0,
		0,0,0,1
	};
#pragma endregion
#pragma region Phong
	
	m_phongShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/phong.vert");
	m_phongShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/phong.frag");
	if (m_phongShader.link() == false)
	{
		printf("Phong no load link fail \n");
		printf(m_phongShader.getLastError());
		return false;
	}
#pragma endregion 
#pragma region TextureShader
	m_textureShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/textured.vert");
	m_textureShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/textured.frag");
	if (m_textureShader.link() == false)
	{
		printf("TextureShader no load link fail \n");
		printf(m_textureShader.getLastError());
		return false;
	}
#pragma endregion
#pragma region NormalMapShader
	m_normalMapShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/normalMap.vert");
	m_normalMapShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/normalMap.frag");
	if (m_normalMapShader.link() == false)
	{
		printf("normalmap shader no load link fail \n");
		printf(m_normalMapShader.getLastError());
		return false;
	}
#pragma endregion
#pragma region GridTGA
	if (m_gridTexture.load("./textures/numbered_grid.tga") == false)
	{
		printf("Failed to load numbered grid.tga \n");
		return false;
	}
#pragma endregion


	m_scene = new Scene(&m_camera, glm::vec2(getWindowWidth(), getWindowHeight()), 
		a_light, glm::vec3(0.25f));

	for (int i = 0; i < 10; i++)
	{
		m_scene->AddInstances(new Instance(glm::vec3(i * 2, 0, 0), 
										   glm::vec3(0, i * 30, 0), 
										   glm::vec3(1), 
										   &m_spearMesh, 
										   &m_normalMapShader));
	}
	//add a red light on the left side and a green light on the right side
	m_scene->GetPointLights().push_back(Light(vec3(5, 3, 0), vec3(1, 0, 0), 50));
	m_scene->GetPointLights().push_back(Light(vec3(-5, 3, 0), vec3(0, 1, 0), 50));

		
	//if all pass without returning false then all has loaded properly
	return true;
}

/*void GraphicsProjectApp::DrawShaderAndMeshes(glm::mat4 a_projectionMatrix, glm::mat4 a_viewMatrix)
{
	auto pvm = a_projectionMatrix * a_viewMatrix * glm::mat4(0);

	//TEXTURESHADER
#pragma  region Quad
	//bind the shader 
	//m_simpleShader.bind();
	m_textureShader.bind();
	//bind the transform of the mesh
	pvm = a_projectionMatrix * a_viewMatrix * m_quadTransform;	
	m_textureShader.bindUniform("ProjectionViewModel",pvm);
	m_textureShader.bindUniform("diffuseTexture", 0);
	m_gridTexture.bind(0);

	m_quadMesh.Draw();
	
#pragma  endregion 
	//PHONGSHADER
#pragma region Phong
	m_phongShader.bind();
	//Bind the camera position
	m_phongShader.bindUniform("CameraPosition", vec3(glm::inverse(a_viewMatrix)[3]));
	m_phongShader.bindUniform("AmbientColor", m_ambientLight);
	m_phongShader.bindUniform("LightColor", m_light.color);
	m_phongShader.bindUniform("LightDirection", m_light.direction);
	
#pragma endregion 

#pragma region Bunny
	//bind the pvm
	pvm = a_projectionMatrix * a_viewMatrix * m_bunnyTransform;
	m_phongShader.bindUniform("ProjectionViewModel", pvm);
	m_phongShader.bindUniform("AmbientColor", bunnyColor);

	//bind the lighting transforms
	m_phongShader.bindUniform("ModelMatrix", m_bunnyTransform);
	m_bunnyMesh.draw();
#pragma endregion

#pragma region Dragon
	//bind the pvm
	pvm = a_projectionMatrix * a_viewMatrix * m_dragonTransform;
	m_phongShader.bindUniform("ProjectionViewModel", pvm);
	m_phongShader.bindUniform("AmbientColor", dragonColor);

	//bind the lighting transforms
	m_phongShader.bindUniform("ModelMatrix", m_dragonTransform);
	m_dragonMesh.draw();
#pragma endregion

#pragma region Lucy
	//bind the pvm
	pvm = a_projectionMatrix * a_viewMatrix * m_lucyTransform;
	m_phongShader.bindUniform("ProjectionViewModel", pvm);
	m_phongShader.bindUniform("AmbientColor", LucyColor);
	
	//bind the lighting transforms
	m_phongShader.bindUniform("ModelMatrix", m_lucyTransform);
	m_lucyMesh.draw();
#pragma endregion

#pragma region Buddha
	//bind the pvm
	pvm = a_projectionMatrix * a_viewMatrix * m_buddhaTransform;
	m_phongShader.bindUniform("ProjectionViewModel", pvm);
	m_phongShader.bindUniform("AmbientColor", BuddhaColor);

	//bind the lighting transforms
	m_phongShader.bindUniform("ModelMatrix", m_buddhaTransform);
	m_buddhaMesh.draw();
#pragma endregion 
	//NORMALMAPSHADER
#pragma region NormalShader
	

#pragma endregion
}*/

void GraphicsProjectApp::ImguiLogic()
{
	
	ImGui::Begin("Scene Light Settings");

	//scene lighting options
	ImGui::DragFloat3("Sunlight Direction", &m_scene->GetLight().m_direction[0], 0.1f, -1.0f, 1.0f);
	ImGui::DragFloat3("Sunlight Color", &m_scene->GetLight().m_color[0], 0.1f, 0.0f, 2.0f);

	//model colors
	//ImGui::DragFloat3("Bunny Color", &bunnyColor[0], 0.1f, 0, 2);
	//ImGui::DragFloat3("Dragon Color", &dragonColor[0], 0.1f, 0, 2);
	//ImGui::DragFloat3("Lucy Color", &LucyColor[0], 0.1f, 0, 2);
	//ImGui::DragFloat3("Buddha Color", &BuddhaColor[0], 0.1f, 0, 2);

	//model transforms the 0000 is to give them basic values not the 29547y29874 they start with 
	
	//ImGui::SliderFloat3("Bunny Transform",&imguiPos[0], -10, 10);
	
	/*if(ImGui::Button("Apply"))
	{
		m_bunnyTransform *= resetMatrix;
		m_bunnyTransform = glm::translate(m_bunnyTransform, imguiPos);
	}*/


	
	ImGui::End();
}
