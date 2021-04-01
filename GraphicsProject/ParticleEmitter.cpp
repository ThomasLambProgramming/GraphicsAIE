#include "ParticleEmitter.h"
#include <gl_core_4_4.h>
#include "glm/ext.hpp"


ParticleEmitter::ParticleEmitter()
	: m_particles(nullptr),
	m_firstDead(0),
	m_maxParticles(0),
	m_position(0,0,0),
	m_vao(0), m_vbo(0), m_ibo(0),
	m_vertexData(nullptr)
{
}

ParticleEmitter::~ParticleEmitter()
{
	delete[] m_particles;
	delete[] m_vertexData;

	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_ibo);
}

void ParticleEmitter::Initalise(unsigned int a_maxParticles, unsigned int a_emitRate, 
	float a_lifetimeMin, float a_lifeTimeMax, 
	float a_velocityMin, float a_velocityMax, 
	float a_startSize, float a_endSize, 
	const glm::vec4& a_startColour, const glm::vec4& a_endColour)
{
	// set up the emit timers
	m_emitTimer = 0;
	m_emitRate = 1.0f / a_emitRate;
	m_imguiRate = a_emitRate;
	// add in all arguments to set variables
	m_startColour = a_startColour;
	m_endColour = a_endColour;
	m_startSize = a_startSize;
	m_endSize = a_endSize;
	m_velocityMin = a_velocityMin;
	m_velocityMax = a_velocityMax;
	m_lifespanMin = a_lifetimeMin;
	m_lifespanMax = a_lifeTimeMax;
	m_maxParticles = a_maxParticles;
	
	// create particle array
	m_particles = new Particle[m_maxParticles];
	m_firstDead = 0;

	// create the array of vertices for the particles
	// 4 vertices per particle for a quad.
	// will be filled during update
	m_vertexData = new ParticleVertex[m_maxParticles * 4];

	//create index buffer for the particles
	//6 points for each as the triangles need 3 points and 2 to make quad
	//this doesnt change as we dont need to change the order
	//so we can set the whole thing

	unsigned int* indexData = new unsigned int[m_maxParticles * 6];
	for (unsigned int i = 0; i < m_maxParticles; i++)
	{
		//triangle 1
		indexData[i * 6 + 0] = i * 4 + 0;
		indexData[i * 6 + 1] = i * 4 + 1;
		indexData[i * 6 + 2] = i * 4 + 2;

		//triangle 2
		indexData[i * 6 + 3] = i * 4 + 0;
		indexData[i * 6 + 4] = i * 4 + 2;
		indexData[i * 6 + 5] = i * 4 + 3;
	}

	//generate all the buffers and vertex arrays needed
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ibo);

	//bind and fill the buffers with data
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, m_maxParticles * 4 * sizeof(ParticleVertex), 
		m_vertexData, GL_DYNAMIC_DRAW);

	//bind and fill index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_maxParticles * 6 * sizeof(unsigned int), 
		indexData, GL_STATIC_DRAW);

	//enable the vertex index 0 and 1 to be accessed
	glEnableVertexAttribArray(0); // position
	glEnableVertexAttribArray(1); // colour
	//give the layouts of the memory buffers
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE,
		sizeof(ParticleVertex), 0);

	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 
		sizeof(ParticleVertex), ((char*)0) + 16);

	//now that everything is bound unbind everything as it might
	//mess with other rendering
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); 
	//delete the index data as its now in gpu memory 
	delete[] indexData;
}

void ParticleEmitter::Update(float deltaTime, glm::mat4 a_cameraTransform)
{
	m_emitTimer += deltaTime;
	while (m_emitTimer > m_emitRate)
	{
		Emit();
		m_emitTimer -= m_emitRate;
	}
	unsigned int quad = 0;

	//organise the array for the dead and the alive particles
	for (unsigned int i = 0; i < m_firstDead; i++)
	{
		//get the current particle
		Particle* particle = &m_particles[i];
		particle->m_lifetime += deltaTime;
		//update particle
		if (particle->m_lifetime >= particle->m_lifespan)
		{
			//if particle dead
			*particle = m_particles[m_firstDead - 1];
			m_firstDead--;
		}
		else
		{
			//update position
			particle->m_position += particle->m_velocity * deltaTime;
			
			//update the colour and size based on time
			particle->m_size = glm::mix(m_startSize, m_endSize,
				particle->m_lifetime / particle->m_lifespan);
			particle->m_color = glm::mix(m_startColour, m_endColour,
				particle->m_lifetime / particle->m_lifespan);

			//same logic as basis vectors, this defines how much to move in 
			//x and y lengths (half lengths)
			float halfSize = particle->m_size * 0.5f;

			m_vertexData[quad * 4 + 0].m_position = glm::vec4(halfSize, halfSize, 0, 1);
			m_vertexData[quad * 4 + 0].m_color = particle->m_color;

			m_vertexData[quad * 4 + 1].m_position = glm::vec4(-halfSize, halfSize, 0, 1);
			m_vertexData[quad * 4 + 1].m_color = particle->m_color;

			m_vertexData[quad * 4 + 2].m_position = glm::vec4(-halfSize, -halfSize, 0, 1);
			m_vertexData[quad * 4 + 2].m_color = particle->m_color;

			m_vertexData[quad * 4 + 3].m_position = glm::vec4(halfSize, -halfSize, 0, 1);
			m_vertexData[quad * 4 + 3].m_color = particle->m_color;

			//create all the xyz basis vectors to create a matrix four that will transform 
			//the positions and rotations of the particles to face the correct way
			//i know the explaination isnt the greatest but its so the "forward" axis is facing camera

			//get the z to equal the direction from particle to camera
			glm::vec3 zAxis = glm::normalize(glm::vec3(a_cameraTransform[3]) - particle->m_position);
			//cross product with z and (0,1,0) vector to get the x (perpendicular)
			glm::vec3 xAxis = glm::normalize(glm::cross(glm::vec3(a_cameraTransform[1]), zAxis));
			//now that we have the x and z do the cross again for perpendicular between
			//x and z to get the y axis
			glm::vec3 yAxis = glm::cross(zAxis, xAxis); 

			
			glm::mat4 billboard(glm::vec4(xAxis,0),
								glm::vec4(yAxis,0),
								glm::vec4(zAxis,0),
								glm::vec4(0,0,0,1));

			//update the quad to be transformed by billboard
			m_vertexData[quad * 4 + 0].m_position = billboard * 
				m_vertexData[quad * 4 + 0].m_position + glm::vec4(particle->m_position, 0);

			m_vertexData[quad * 4 + 1].m_position = billboard *
				m_vertexData[quad * 4 + 1].m_position + glm::vec4(particle->m_position, 0);

			m_vertexData[quad * 4 + 2].m_position = billboard *
				m_vertexData[quad * 4 + 2].m_position + glm::vec4(particle->m_position, 0);

			m_vertexData[quad * 4 + 3].m_position = billboard *
				m_vertexData[quad * 4 + 3].m_position + glm::vec4(particle->m_position, 0);

			++quad;
			
		}
	}

}

void ParticleEmitter::Emit()
{
	//only emit if there is a dead particle to use
	if (m_firstDead >= m_maxParticles)
		return;

	//resurrect the first dead particle
	Particle& particle = m_particles[m_firstDead++];

	//assign its starting position
	particle.m_position = m_position;

	//randomise its lifespandd
	particle.m_lifetime = 0;
	particle.m_lifespan = (rand() / (float)RAND_MAX) * 
		(m_lifespanMax - m_lifespanMin) + m_lifespanMin;

	//set starting size and colour
	particle.m_color = m_startColour;
	particle.m_size = m_startSize;
	// randomise velocity direction and strength
	float velocity = (rand() / (float)RAND_MAX) * 
		(m_velocityMax - m_velocityMin) + m_velocityMin;

	particle.m_velocity.x = (rand() / (float)RAND_MAX) * 2 - 1;
	particle.m_velocity.y = (rand() / (float)RAND_MAX) * 2 - 1;
	particle.m_velocity.z = (rand() / (float)RAND_MAX) * 2 - 1;
	particle.m_velocity = glm::normalize(particle.m_velocity) * velocity;
}

void ParticleEmitter::Draw()
{
	//bind the buffers, give only the data we need instead of the whole buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, m_firstDead * 4 * sizeof(ParticleVertex), m_vertexData);

	//bind the vertex array to be drawn then draw the amount of particles
	// * 6 because each particle has 3 vertexs per trianlge * 2 for square
	glBindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLES, m_firstDead * 6, GL_UNSIGNED_INT, 0);
}