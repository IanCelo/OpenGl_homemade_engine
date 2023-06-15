#include <iostream>
#include <fstream>
#include <sstream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <Resources/Mesh.hpp>

Resources::Mesh::Mesh(const std::vector<Vertex>& verticesIn)
{
	m_vertices = verticesIn;
	setupMesh();
}

Resources::Mesh::~Mesh() 
{ 
	m_vertices.clear(); 
}


void Resources::Mesh::setupMesh()
{
	//	Create VAO - Vertex Array Object
	glGenVertexArrays(1, &VAO);

	//	Create VBO - Vertex Buffer Object
	glGenBuffers(1, &VBO);

	//	Define VBO and VAO
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//	Attach VBO to VAO / Bind attributes (position) in VAO
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), m_vertices.data(), GL_STATIC_DRAW);


	//	Position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);

	//	Normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(offsetof(Vertex, Normals)));

	//	Textures
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(offsetof(Vertex, TexCoords)));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}


void Resources::Mesh::draw()
{
	glBindVertexArray(VAO);

	glDrawArrays(GL_TRIANGLES, 0, (GLsizei)m_vertices.size());

	glBindVertexArray(0);
}
