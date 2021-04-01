#include "Instance.h"

#include "Scene.h"
#include "OBJMesh.h"
#include "Camera.h"
#include "Mesh.h"
#include "Shader.h"

#include <Texture.h>
#include <Application.h>
#include "glm/ext.hpp"

Instance::Instance(glm::mat4 a_transform, aie::OBJMesh* a_mesh, aie::ShaderProgram* a_shader)
	: m_transform(a_transform), m_mesh(a_mesh), m_shader(a_shader)
{
}
//Massive constructor for an instance
Instance::Instance(glm::vec3 a_position, glm::vec3 a_eulerAngles, glm::vec3 a_scale, 
	aie::OBJMesh* a_mesh, aie::ShaderProgram* a_shader, std::string objName)
		: m_mesh(a_mesh), m_shader(a_shader), m_objectName(objName)
{
	m_rotation = a_eulerAngles;
	m_transform = MakeTransform(a_position, a_eulerAngles, a_scale);
}

glm::vec3 Instance::GetPosition()
{
	return glm::vec3();
}

glm::vec3 Instance::GetRotation()
{
	return m_rotation;
}

glm::vec3 Instance::GetScale()
{
	return glm::vec3();
}

void Instance::Draw(Scene* a_scene)
{
	//bind whatever the models shader is
	m_shader->bind();

	//Bind the camera position
	auto pvm = a_scene->GetCamera()->GetProjectionMatrix(a_scene->GetWindowSize().x, 
		a_scene->GetWindowSize().y) * a_scene->GetCamera()->GetViewMatrix() * m_transform;
	
	//get all scene values for the vertex and fragment shader 
	m_shader->bindUniform("ProjectionViewModel", pvm);
	m_shader->bindUniform("CameraPosition", a_scene->GetCamera()->GetPosition());
	m_shader->bindUniform("AmbientColor", a_scene->GetAmbientLight());
	m_shader->bindUniform("LightColor", a_scene->GetLight().m_color);
	m_shader->bindUniform("LightDirection", a_scene->GetLight().m_direction);
	//m_shader->bindUniform("SpecularPower", 32);
	m_shader->bindUniform("ModelMatrix", m_transform);

	//get the number of lights in the scene so that the lighting can be blended 
	//and the model can be effected by more than one light
	int numLights = a_scene->GetNumberLights();
	m_shader->bindUniform("numLights", numLights);
	m_shader->bindUniform("PointLightPosition", numLights, a_scene->GetPointLightPositions());
	m_shader->bindUniform("PointLightColor", numLights, a_scene->GetPointLightColors());

	
	m_mesh->draw();
}

glm::mat4 Instance::MakeTransform(glm::vec3 a_position, glm::vec3 a_eulerAngles, glm::vec3 a_scale)
{
	//translate then rotate by xyz then scale
	return glm::translate(glm::mat4(1), a_position)
		* glm::rotate(glm::mat4(1), glm::radians(a_eulerAngles.x), glm::vec3(1, 0, 0))
		* glm::rotate(glm::mat4(1), glm::radians(a_eulerAngles.y), glm::vec3(0, 1, 0))
		* glm::rotate(glm::mat4(1), glm::radians(a_eulerAngles.z), glm::vec3(0, 0, 1))
		* glm::scale(glm::mat4(1), a_scale);
}
