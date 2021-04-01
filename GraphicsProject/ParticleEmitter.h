#pragma once
#include <glm/glm.hpp>

struct Particle
{
	glm::vec3 m_position;
	glm::vec3 m_velocity;
	glm::vec4 m_color;
	float m_size;
	float m_lifetime;
	float m_lifespan;
};
struct ParticleVertex
{
	glm::vec4 m_position;
	glm::vec4 m_color;
};

class ParticleEmitter
{
public:
	ParticleEmitter();
	virtual ~ParticleEmitter();

	void Initalise(unsigned int a_maxParticles, unsigned int a_emitRate,
		float a_lifetimeMin, float a_lifeTimeMax,
		float a_velocityMin, float a_velocityMax,
		float a_startSize, float a_endSize,
		const glm::vec4& a_startColour, const glm::vec4& a_endColour);
	

	void Update(float deltaTime, glm::mat4 a_cameraTransform);
	void Emit();
	void Draw();

	float* GetEmitRate() { return &m_emitRate; }
	void SetEmitRate() { m_emitRate = 1.0f / m_imguiRate; }
	float* GetMinLife() { return &m_lifespanMin; }
	float* GetMaxLife() { return &m_lifespanMax; }

	float* GetMinVel() { return &m_velocityMin; }
	float* GetMaxVel() { return &m_velocityMax; }

	float* GetStartSize() { return &m_startSize; }
	float* GetEndSize() { return &m_endSize; }

	glm::vec4& GetStartColour() { return m_startColour; }
	glm::vec4& GetEndColour() { return m_endColour; }
	
	glm::vec3& GetPosition() { return m_position; }

	//this was to avoid using a static variable
	//and so that the imgui wouldnt cause a divide loop
	//its a seperated spawn rate so the number is nice on the 
	//imgui instead of a small float number
	float m_imguiRate;

protected:
	
	Particle* m_particles;
	unsigned int m_firstDead;
	unsigned int m_maxParticles;
	unsigned int m_vao, m_vbo, m_ibo;
	ParticleVertex* m_vertexData;

	glm::vec3 m_position;
	float m_emitTimer;
	float m_emitRate;

	float m_lifespanMin;
	float m_lifespanMax;
	float m_velocityMin;
	float m_velocityMax;

	float m_startSize;
	float m_endSize;

	glm::vec4 m_startColour;
	glm::vec4 m_endColour;

	glm::mat4 m_transform;
};

