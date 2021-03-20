#include "Scene.h"
#include "Instance.h"
#include <imgui.h>
#include <glm/ext.hpp>
#include <string>
#include "Camera.h"

Scene::Scene(Camera* a_camera, glm::vec2 a_windowSize, Light& a_light, glm::vec3 a_ambientLight)
	: m_camera(a_camera), m_windowSize(a_windowSize), m_light(a_light), m_ambientLight(a_ambientLight)
{
	
}

Scene::~Scene()
{
	for (auto i = m_instances.begin(); i != m_instances.end(); i++)
	{
		delete(*i);
	}
	
}

void Scene::AddInstances(Instance* a_instance)
{
	m_instances.push_back(a_instance);
}

void Scene::Draw()
{
	for (int i = 0; i < MAX_LIGHTS && i < m_pointLights.size(); i++)
	{
		m_pointLightPositions[i] = m_pointLights[i].m_direction;
		m_pointLightColors[i] = m_pointLights[i].m_color;
	}
	for (auto i = m_instances.begin(); i != m_instances.end(); i++)
	{
		Instance* instance = *i;
		instance->Draw(this);
	}
}

void Scene::ChangeCamera(int ID)
{
	for (auto i : m_cameraList)
	{
		if (i->GetID() == ID)
			m_camera = i;
	}
}

void Scene::ImGuiScene()
{
	//scene lighting options
	ImGui::DragFloat3("Sunlight Direction", &GetLight().m_direction[0], 0.1f, -1.0f, 1.0f);
	ImGui::DragFloat3("Sunlight Color", &GetLight().m_color[0], 0.1f, 0.0f, 2.0f);
	//Model Settings for each model
	// Position
	// Rotation
	// Scale
	glm::vec3 temp = { 0.0f, 0.0f, 0.0f };
	if (ImGui::CollapsingHeader("Objects"))
	{
		for (auto i : m_instances)
		{
			if (ImGui::CollapsingHeader(i->m_objectName.c_str()))
			{
				//this grabs the 4th column in the transform
				//so the x,y,z, then w (w isnt needed)
				std::string temp = i->m_objectName + " Position";
				ImGui::Indent();
				ImGui::SliderFloat3(temp.c_str(), &i->GetTransform()[3][0], -20.0f, 20.f);

				temp = i->m_objectName + "RotationX";
				
				ImGui::SliderFloat(temp.c_str(), &i->GetTransform()[0][3], -90.f, 90.f);


				//This Rotates (work with that later for showing off model kind of thing)
				/*
				static float rotations[3];
				temp = i->m_objectName + "Rotation";
				ImGui::SliderFloat3(temp.c_str(), rotations, -20.0f, 20.f);
				
				glm::mat4 yeet = glm::rotate(i->GetTransform(), glm::radians(rotations[0]), glm::vec3(1, 0, 0));
				yeet = glm::rotate(i->GetTransform(), glm::radians(rotations[1]), glm::vec3(0, 1, 0));
				i->SetTransform(yeet);
				yeet = glm::rotate(i->GetTransform(), glm::radians(rotations[2]), glm::vec3(0, 0, 1));
				*/
				ImGui::Unindent();
			}
		}
	}
	//light settings for each in m_scene 
	//Dynamic
	//Direction
	//Position
	//Color

	//Ambient / Stationary Light
	//direction
	//color

	//Camera Settings (3 camers for x,y,z make 2d position movement but facing axis)
	//	x, y position
}
