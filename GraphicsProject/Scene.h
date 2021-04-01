#pragma once
#include <list>
#include <vector>
#include <glm/glm.hpp>

class Camera;
class Instance;

const int MAX_LIGHTS = 4;

struct Light
{
	Light()
	{
		m_direction = glm::vec3(1);
		m_color = glm::vec3(1);
	};
	Light(glm::vec3 a_position, glm::vec3 a_color, float a_intensity)
	{
		m_direction = a_position;
		m_color = a_color * a_intensity;		
	}
	glm::vec3 m_direction;
	glm::vec3 m_color;
};

class Scene
{
public:
	Scene(Camera* a_camera, glm::vec2 a_windowSize, Light& a_light, glm::vec3 a_ambientLight);
	~Scene();

	void AddInstances(Instance* a_instance);
	void Draw();

	void ChangeCamera(int ID);

	void ImGuiScene();

	Camera* GetCamera() { return m_camera; }
	glm::vec2 GetWindowSize() { return m_windowSize; }

	Light& GetLight() { return m_light; }
	glm::vec3 GetAmbientLight() { return m_ambientLight; }
	
	int GetNumberLights() { return (int)m_pointLights.size(); }
	glm::vec3* GetPointLightPositions() { return &m_pointLightPositions[0]; }
	glm::vec3* GetPointLightColors() { return &m_pointLightColors[0]; }

	std::vector<Light>& GetPointLights() { return m_pointLights; }

	void AddCamera(Camera* a_camera) { m_cameraList.push_back(a_camera); }

	bool RotateAmbient = false;

protected:
	Camera* m_camera;

	std::vector<Camera*> m_cameraList;

	glm::vec2 m_windowSize;

	//this is a general light to be the sun
	Light m_light;
	
	std::vector<Light> m_pointLights;
	glm::vec3 m_ambientLight;

	//objects
	std::list<Instance*> m_instances;

	//seperate values for the shaders
	glm::vec3 m_pointLightPositions[MAX_LIGHTS];
	glm::vec3 m_pointLightColors[MAX_LIGHTS];
}; 

	


