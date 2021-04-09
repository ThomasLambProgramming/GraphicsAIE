#pragma once
#include "glm/ext.hpp"

/*
	MADE BY THOMAS LAMB
	DATE MADE 13/03/2021
	This header file and cpp is made for
	managing cameras, and returning all values needed
	Last Edited 9/04/2021
*/



class Camera
{
public:
	Camera();
	~Camera() {};

	void Update(float a_deltaTime);
	glm::vec3 GetPosition() { return m_position; }
	void SetPosition(glm::vec3 a_pos) { m_position = a_pos; }

	glm::mat4 GetViewMatrix();
	glm::mat4 GetProjectionMatrix(float a_width, float a_height);



	void SetRotation(float x = 0.0f, float y = 0.0f);

	glm::vec2 GiveRotation() { return { m_theta, m_phi }; }

	int GetID() { return m_ID; }
	void SetID(int a_id) { m_ID = a_id; }

	void SetStationary(bool a_state) { isStationary = a_state; }

	glm::mat4 MakeTransform();
	

	glm::vec3 forward = { 0,0,0 };
private:

	float m_theta; //in degrees X 
	float m_phi; //in degrees Y
	glm::vec3 m_position;

	//for ease of switching each camera has an id;
	int m_ID;
	bool isStationary = false;

	//mouse positions
	float m_lastMouseX, m_lastMouseY;

	
};

