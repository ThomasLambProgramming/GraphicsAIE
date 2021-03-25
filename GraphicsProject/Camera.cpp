#include "Camera.h"
#include "glm/glm.hpp"
#include <Input.h>


Camera::Camera()
{
	m_position = glm::vec3(-10, 2, 0);
	m_phi = 0;
	m_theta = 0;
}

void Camera::Update(float a_deltaTime)
{
	if (isStationary)
		return;

	aie::Input* input = aie::Input::getInstance();
	float thetaR = glm::radians(m_theta);
	float phiR = glm::radians(m_phi);

	
	//calculate the forwards and right axes and the up axis for the camera
	glm::vec3 forward(glm::cos(phiR) * glm::cos(thetaR),
		glm::sin(phiR), glm::cos(phiR) * glm::sin(thetaR));
	glm::vec3 right(-glm::sin(thetaR), 0, glm::cos(thetaR));
	glm::vec3 up(0, 1, 0);

	
	#pragma region InputMovement
	if (input->isKeyDown(aie::INPUT_KEY_X))
	{
		m_position += up * a_deltaTime;
	}
	if (input->isKeyDown(aie::INPUT_KEY_Z))
	{
		m_position -= up * a_deltaTime;
	}
	if (input->isKeyDown(aie::INPUT_KEY_A))
	{
		m_position -= right * a_deltaTime;
	}
	if (input->isKeyDown(aie::INPUT_KEY_D))
	{
		m_position += right * a_deltaTime;
	}
	if (input->isKeyDown(aie::INPUT_KEY_W))
	{
		m_position += forward * a_deltaTime;
	}
	if (input->isKeyDown(aie::INPUT_KEY_S))
	{
		m_position -= forward * a_deltaTime;
	}

	//getting the mouse input for this frame 
	float mx = input->getMouseX();
	float my = input->getMouseY();
	const float turnSpeed = glm::radians(180.0f);
	
	//if the right button is down, increment the theta and phi

	if (input->isMouseButtonDown(aie::INPUT_MOUSE_BUTTON_RIGHT))
	{
		m_theta += turnSpeed * (mx - m_lastMouseX) * a_deltaTime;
		m_phi += turnSpeed * (my - m_lastMouseY) * a_deltaTime;
	}

	//now store the frames last values for the next

	m_lastMouseX = mx;
	m_lastMouseY = my;

	#pragma endregion 
}

glm::mat4 Camera::GetViewMatrix()
{
	float thetaR = glm::radians(m_theta);
	float phiR = glm::radians(m_phi);
	forward = glm::vec3(glm::cos(phiR) * glm::cos(thetaR),
		glm::sin(phiR), glm::cos(phiR) * glm::sin(thetaR));

	
	return glm::lookAt(m_position, m_position + forward, glm::vec3(0, 1, 0));
}

glm::mat4 Camera::GetProjectionMatrix(float a_width, float a_height)
{	
	return glm::perspective(glm::pi<float>() * 0.25f, 
		a_width/ a_height, 0.1f, 1000.0f);
}

void Camera::SetRotation(float x, float y)
{
	m_theta = x;
	m_phi = y;
}

glm::mat4 Camera::GetTransform()
{
	return glm::translate(glm::mat4(1), m_position)
		* glm::rotate(glm::mat4(1), glm::radians(m_theta), glm::vec3(1, 0, 0))
		* glm::rotate(glm::mat4(1), glm::radians(m_phi), glm::vec3(0, 1, 0));
		
}


