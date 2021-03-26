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
	std::string cameraID; 
	int id = m_camera->GetID();
	if (id == 1)
		cameraID = "Current camera = Flyby";
	else if (id == 2)
		cameraID = "Current camera = X Axis";
	else if (id == 3)
		cameraID = "Current camera = Y Axis";
	else
		cameraID = "Current camera = Z Axis";
	
	ImGui::Text(cameraID.c_str());
	ImGui::Text("Press 1 for flyby camera");
	ImGui::Text("Press 2-4 for X Y Z Axis Stationary Cameras");
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
				ImGui::SliderFloat3(temp.c_str(), &i->m_rotation[0], -90.0f, 90.f);

				temp = i->m_objectName + " Scale";
				ImGui::SliderFloat3(temp.c_str(), &o_scale[0], 0.2f, 5.0f);

				i->SetTransform(i->MakeTransform(o_translation, i->m_rotation, o_scale));
				
				ImGui::Unindent();
			}
		}
		ImGui::Unindent();
	}
	if (ImGui::CollapsingHeader("Ambient Light"))
	{
		ImGui::Indent();
		ImGui::SliderFloat3("Color", &m_ambientLight[0], 0.0f, 20.0f);
		ImGui::Unindent();
	}
	if (ImGui::CollapsingHeader("Dynamic Light"))
	{
		ImGui::Indent();
		std::string temp;
		temp = "Light Direction";
		ImGui::SliderFloat3(temp.c_str(), &m_light.m_direction[0], -20.0f, 20.0f);

		temp = "Light Color";
		ImGui::SliderFloat3(temp.c_str(), &m_light.m_color[0], 0.0f, 40.0f);
		ImGui::Unindent();
	}
	if (ImGui::CollapsingHeader("Point Lights"))
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
				ImGui::SliderFloat3(temp.c_str(), &i->m_color[0], 0.0f, 40.0f);
				ImGui::Unindent();
			}
		}
		ImGui::Unindent();
	}
}
