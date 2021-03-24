#include "Scene.h"
#include "Instance.h"
#include <imgui.h>
#include <glm/ext.hpp>
#include <string>
#include "Camera.h"
#include <glm/gtx/matrix_decompose.hpp>
#include <iostream>

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
	// ---------TO ADD -------------
	// Add what camera is being used
	// Change direction of lights
	// add color to each light
	// add rotation to models
	// Research and implement at least one of the following
	// Render targets
	// Post processing
	// Quaternions
	// Particle Effects
	// Shadows


	//scene lighting options
	ImGui::DragFloat3("Sunlight Direction", &GetLight().m_direction[0], 0.1f, -1.0f, 1.0f);
	ImGui::DragFloat3("Sunlight Color", &GetLight().m_color[0], 0.1f, 0.0f, 2.0f);
	
	glm::vec3 temp = { 0.0f, 0.0f, 0.0f };
	if (ImGui::CollapsingHeader("Objects"))
	{
		ImGui::Indent();
		for (auto i : m_instances)
		{
			if (ImGui::CollapsingHeader(i->m_objectName.c_str()))
			{

				glm::mat4 objectTransform = i->GetTransform();
				glm::vec3 o_scale;
				glm::quat o_rotation;
				glm::vec3 o_translation;
				glm::vec3 o_skew;
				glm::vec4 o_perspective;
				glm::decompose(objectTransform, o_scale, o_rotation, o_translation, o_skew, o_perspective);

				
				//this grabs the 4th column in the transform
				//so the x,y,z, then w (w isnt needed)
				std::string temp = i->m_objectName + " Position";
				ImGui::Indent();
				ImGui::SliderFloat3(temp.c_str(), &o_translation[0], -20.0f, 20.f);

				
				//std::cout << euler.x << " " << euler.y << " " << euler.z << std::endl;
				
				temp = i->m_objectName + "Rotation";
				glm::vec3 euler = glm::eulerAngles(o_rotation);
				euler *= 180.f / 3.14159f;
				ImGui::SliderFloat3(temp.c_str(), &euler[0], -90.0f, 90.f);
				euler *= 3.14159f / 180.0f;
				

				temp = i->m_objectName + " Scale";
				ImGui::SliderFloat3(temp.c_str(), &o_scale[0], 0.2f, 5.0f);

				i->SetTransform(i->MakeTransform(o_translation, euler, o_scale));
				
				ImGui::Unindent();
			}
		}
		ImGui::Unindent();
	}
	if (ImGui::CollapsingHeader("Lights"))
	{
		int count = 0;
		ImGui::Indent();
		for (auto i = m_pointLights.begin(); i != m_pointLights.end(); i++)
		{
			count++;

			std::string temp = "Light :" + std::to_string(count);
			if (ImGui::CollapsingHeader(temp.c_str()))
			{
				ImGui::Indent();
				temp += " Position";
				ImGui::SliderFloat3(temp.c_str(), &i->m_direction[0], -20.0f, 20.0f);

				temp = "Light :" + std::to_string(count) + " Color";
				ImGui::SliderFloat3(temp.c_str(), &i->m_color[0], 0.0f, 2.0f);
				ImGui::Unindent();
			}
		}
		ImGui::Unindent();
	}
}
