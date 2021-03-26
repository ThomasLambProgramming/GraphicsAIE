#include "GraphicsProjectApp.h"
#include "Gizmos.h"
#include "Input.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "imgui.h"
#include <string>
#include "Scene.h"
#include "Instance.h"
#include <sstream>
#include <iostream>


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

	m_camera = new Camera();
	m_camera->SetID(1);
	
	m_emitter = new ParticleEmitter();
	m_emitter->Initalise(1000, 50, 
		0.1f, 1.0f, 
		1.0f, 5.0f, 
		0.3f, 0.1f, 
		glm::vec4(1, 0, 0, 1), glm::vec4(1, 1, 0, 1));

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
		Gizmos::addLine(vec3(-10 + i, 0, 10), vec3(-10 + i, 0, -10), i == 10 ? white : black);
		Gizmos::addLine(vec3(10, 0, -10 + i), vec3(-10, 0, -10 + i), i == 10 ? white : black);
	}
	ImguiLogic();

	float time = getTime();
	m_scene->GetLight().m_direction = glm::normalize(glm::vec3(glm::cos(time * 2), glm::sin(time * 2), 0));

	// add a transform so that we can see the axis
	Gizmos::addTransform(mat4(1));
	
	m_camera->Update(deltaTime);
	m_emitter->Update(deltaTime, m_camera->GetPosition());
	
	aie::Input* input = aie::Input::getInstance();


	if (input->isKeyDown(aie::INPUT_KEY_1))
		m_scene->ChangeCamera(1);
	else if (input->isKeyDown(aie::INPUT_KEY_2))
	 	m_scene->ChangeCamera(2);
	else if (input->isKeyDown(aie::INPUT_KEY_3))
		m_scene->ChangeCamera(3);
	else if (input->isKeyDown(aie::INPUT_KEY_4))
		m_scene->ChangeCamera(4);

	m_camera = m_scene->GetCamera();

	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void GraphicsProjectApp::draw() {

	// wipe the screen to the background colour
	clearScreen();
	m_projectionMatrix = m_camera->GetProjectionMatrix(getWindowWidth(), getWindowHeight());
	m_viewMatrix = m_camera->GetViewMatrix();

	for (auto i : m_scene->GetPointLights())
	{
		glm::vec4 temp(i.m_color.r, i.m_color.g, i.m_color.b, 1);
		Gizmos::addSphere(i.m_direction, 1.0f, 10, 10, temp);
	}
	//DRAWING PARTICLE EFFECTS
	m_particleShader.bind();
	glm::mat4 temp(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	);
	auto pvm = m_projectionMatrix * m_viewMatrix * temp;
	m_particleShader.bindUniform("ProjectionViewModel", pvm);
	m_emitter->Draw();

	Light sunLight = m_scene->GetLight();
	glm::vec4 tempColor(sunLight.m_color.r, sunLight.m_color.g, sunLight.m_color.b, 1);
	Gizmos::addLine({ 0,0,0 }, sunLight.m_direction * 4.0f, tempColor);

	Gizmos::draw(m_projectionMatrix * m_viewMatrix);
	m_scene->Draw();
}
bool GraphicsProjectApp::LoadShaderAndMeshLogic(Light a_light)
{
#pragma region Shaders

	m_particleShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/ParticleShader.vert");
	m_particleShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/ParticleShader.frag");
	if (m_particleShader.link() == false)
	{
		printf("ParticleShader no load link fail \n");
		printf(m_particleShader.getLastError());
		return false;
	}

	m_phongShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/phong.vert");
	m_phongShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/phong.frag");
	if (m_phongShader.link() == false)
	{
		printf("Phong no load link fail \n");
		printf(m_phongShader.getLastError());
		return false;
	}

	m_textureShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/textured.vert");
	m_textureShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/textured.frag");
	if (m_textureShader.link() == false)
	{
		printf("TextureShader no load link fail \n");
		printf(m_textureShader.getLastError());
		return false;
	}

	m_normalMapShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/normalMap.vert");
	m_normalMapShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/normalMap.frag");
	if (m_normalMapShader.link() == false)
	{
		printf("normalmap shader no load link fail \n");
		printf(m_normalMapShader.getLastError());
		return false;
	}
#pragma endregion
#pragma region Meshes
	if (m_dragonMesh.load("./stanford/Dragon.obj") == false)
	{
		printf("Dragon didnt load");
		return false;
	}
	
	if (m_spearMesh.load("./soulspear/soulspear.obj", true, true) == false)
	{
		printf("Spear Didnt load");
		return false;
	}
	if (m_gunMesh.load("./M1_carbine/M1_carbine.obj", true, true) == false)
	{
		printf("Gun Didnt load");
		return false;
	}
#pragma endregion

	m_scene = new Scene(m_camera, glm::vec2(getWindowWidth(), getWindowHeight()), 
		a_light, glm::vec3(0.25f));

	//Three stationary lights
	Camera* ZAxis = new Camera();
	ZAxis->SetStationary(true);
	ZAxis->SetID(2);
	ZAxis->SetPosition({ 0.0f,4.0f,20.0f });
	ZAxis->SetRotation(-90.0f, 0.0f);
	m_scene->AddCamera(ZAxis);

	Camera* YAxis = new Camera();
	YAxis->SetStationary(true);
	YAxis->SetID(3);
	YAxis->SetPosition({ 0.0f,20.0f,0.0f });
	YAxis->SetRotation(0.0f, -90.0f);
	m_scene->AddCamera(YAxis);

	Camera* XAxis = new Camera();
	XAxis->SetStationary(true);
	XAxis->SetID(4);
	XAxis->SetPosition({-20.0f, 4.0f, 0.0f });
	XAxis->SetRotation();
	m_scene->AddCamera(XAxis);

	//couldnt add the camera in startup due to the scenes instance not being created yet
	m_scene->AddCamera(m_camera);

	//Create an instance out of dragon mesh and add to scene
	m_scene->AddInstances(new Instance(glm::vec3(5, 0, 5),
		glm::vec3(0, 0, 0),
		glm::vec3(0.5f, 0.5f, 0.5f),
		&m_dragonMesh,
		&m_phongShader, "Dragon"));

	m_scene->AddInstances(new Instance(glm::vec3(-3, 4.8f, 0.6f),
		glm::vec3(-90.0f, 0, 0),
		glm::vec3(0.1f, 0.1f, 0.1f),
		&m_gunMesh,
		&m_normalMapShader, "Gun"));


	for (int i = 0; i < 10; i++)
	{
		std::string name = "Spear :" + std::to_string(i);
		
		if (i % 2 == 0)
		{
			m_scene->AddInstances(new Instance(glm::vec3(i * 2, 0, 0), glm::vec3(0, 90, 0),
				glm::vec3(1), &m_spearMesh, &m_normalMapShader, name));
		}
		else
		{	
			m_scene->AddInstances(new Instance(glm::vec3(i * 2, 0, 0), glm::vec3(0, 0, 0),
				glm::vec3(1), &m_spearMesh, &m_normalMapShader, name));
		}
	}
	//add a red light on the left side and a green light on the right side
	m_scene->GetPointLights().push_back(Light(vec3(5, 4, 5), vec3(1, 0, 0), 50));
	m_scene->GetPointLights().push_back(Light(vec3(-5, 3, 5), vec3(0, 1, 0), 50));

	//add a third light to be the dynamic light
	m_scene->GetPointLights().push_back(Light(vec3(0, 5, 0), vec3(1, 1, 1), 30));

		
	//if all pass without returning false then all has loaded properly
	return true;
}


void GraphicsProjectApp::ImguiLogic()
{
	ImGui::Begin("Scene Settings");
	//This is seperate so only the logic of each scene needs to be called
	//in case of multiple scenes the current scene can be loaded and have its own logic called
	m_scene->ImGuiScene();

	//PARTICLE SETTINGS
	if (ImGui::CollapsingHeader("Particle Settings"))
	{
		ImGui::Indent();
		ImGui::SliderFloat3("Position", &m_emitter->GetPosition()[0], -20.0f, 20.0f);
		ImGui::SliderFloat("EmitRate", &m_emitter->m_imguiRate, 0.2f, 1000.0f);
		m_emitter->SetEmitRate();

		ImGui::SliderFloat("MinLife", m_emitter->GetMinLife(), 0.1f, 100.0f);
		ImGui::SliderFloat("MaxLife", m_emitter->GetMaxLife(), 0.1f, 100.0f);
		ImGui::SliderFloat("MinVelocity", m_emitter->GetMinVel(), 0.1f, 200.0f);
		ImGui::SliderFloat("MaxVelocity", m_emitter->GetMaxVel(), 0.1f, 200.0f);
		ImGui::SliderFloat("StartSize", m_emitter->GetStartSize(), 0.1f, 10.0f);
		ImGui::SliderFloat("EndSize", m_emitter->GetEndSize(), 0.1f, 10.0f);
		ImGui::SliderFloat4("StartColour", &m_emitter->GetStartColour()[0], 0.0f, 1.0f);
		ImGui::SliderFloat4("EndColour", &m_emitter->GetEndColour()[0], 0.0f, 1.0f);
		ImGui::Unindent();
	}
	ImGui::End();
}



