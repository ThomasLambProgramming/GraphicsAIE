#pragma once
/*
	MADE BY THOMAS LAMB
	DATE MADE 13/03/2021
	This header file and cpp is made for
	the creation of a mesh and for storing its 
	opengl values
	Last Edited 9/04/2021
*/
#include "glm/glm.hpp"


class Mesh
{
public:
	Mesh() : triCount(0), vao(0), vbo(0), ibo(0) {}
	virtual ~Mesh();

	struct Vertex
	{
		
		glm::vec4 position;
		glm::vec4 normal;
		glm::vec2 texCoord;
	};
	
	void InitialiseQuad();
	void Initialise(unsigned int a_vertexCount, const Vertex* a_verticies, 
		unsigned int a_indexCount = 0, 
		unsigned int* a_indices = nullptr);
	virtual void Draw();


protected:
	unsigned int triCount;
	unsigned int vao, vbo, ibo;
				//vertex array, vertex buffer, index buffer
	


	
};

