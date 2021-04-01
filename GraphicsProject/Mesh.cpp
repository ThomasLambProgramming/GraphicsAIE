#include "Mesh.h"
#include <gl_core_4_4.h>
Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);
}
void Mesh::InitialiseQuad()
{
	//check init already has not been run
	assert(vao == 0);

	//make buffer
	glGenBuffers(1, &vbo);
	//make vertex array
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);


	//this sets the 6 points of the quad 
	//3 points for 1 triangle and 2 triangles for quad = 6 points
	Vertex vertices[6];
	vertices[0].position = { -0.5f, 0.f, 0.5f, 1.0f };
	vertices[1].position = {  0.5f, 0.f, 0.5f, 1.0f };
	vertices[2].position = { -0.5f, 0.f,-0.5f, 1.0f };

	vertices[3].position = { -0.5f, 0.f, -0.5f, 1.0f };
	vertices[4].position = {  0.5f, 0.f, 0.5f, 1.0f };
	vertices[5].position = { 0.5f, 0.f,-0.5f, 1.0f };

	//we set the normal to just face up in world
	vertices[0].normal = {0,1,0,0};
	vertices[1].normal = {0,1,0,0};
	vertices[2].normal = {0,1,0,0};
	vertices[3].normal = {0,1,0,0};
	vertices[4].normal = {0,1,0,0};
	vertices[5].normal = {0,1,0,0};

	//where to place all the texture amounts
	vertices[0].texCoord = { 0, 1 }; // Bottom left
	vertices[1].texCoord = { 1, 1 }; // Bottom right
	vertices[2].texCoord = { 0, 0 }; // Top left
	vertices[3].texCoord = { 0, 0 }; // Top left
	vertices[4].texCoord = { 1, 1 }; // Bottom right
	vertices[5].texCoord = { 1, 0 }; // Top right

	//set the buffer memory for what we need
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(Vertex), vertices, GL_STATIC_DRAW);

	//enabel the first element for the vertex array
	glEnableVertexAttribArray(0);
	//give memory layout of the vertex attribute
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	//enable the second element as a normal
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)16);

	//enable the third element (text coords)
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)32);
	
	//unbind so no issues are caused
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//quad has two triangles
	triCount = 2;	
}

void Mesh::Initialise(unsigned int a_vertexCount, const Vertex* a_verticies, unsigned int a_indexCount, unsigned int* a_indices)
{
	//vertex array check
	assert(vao == 0);

	//generate the vertex buffer and array
	glGenBuffers(1, &vbo);
	glGenVertexArrays(1, &vao);

	//bind both 
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	//fill buffer
	glBufferData(GL_ARRAY_BUFFER, a_vertexCount * sizeof(Vertex), a_verticies, GL_STATIC_DRAW);
	//allow first index
	glEnableVertexAttribArray(0);
	//fill first attrib
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	//if there are any indicies bind them
	//indices are used to reduce overlap and calling areas 
	//eg calling a 2 triangles to make square would need 6 points with indicies it needs 4
	if (a_indexCount != 0)
	{
		//make the index buffer for indice
		glGenBuffers(1, &ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		//give the buffer the information
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, a_indexCount * sizeof(unsigned int),a_indices,GL_STATIC_DRAW);
		//for every 3 indices there is one triangle
		triCount = a_indexCount / 3;

	}
	else
	{
		//unbind everything as we wont use it straight away 
		//as all information now sits in the gpu through the vao we can bind and unbind whenever
		triCount = a_vertexCount / 3;
		//unbind everything for no issues caused
		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}


void Mesh::Draw()
{
	//bind the generated data filled vertex array 
	glBindVertexArray(vao);
	//if indices are used then use them to draw
	if (ibo != 0)
	{
		glDrawElements(GL_TRIANGLES, 3 * triCount, GL_UNSIGNED_INT, 0);
		
	}
	else
	{
		//if indicies arent used then draw without
		glDrawArrays(GL_TRIANGLES, 0, 3 * triCount);
	}
}
