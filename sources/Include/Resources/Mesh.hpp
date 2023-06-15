#pragma once

#include <vector>
#include <string>

#include <Maths/Vector3.h>

#include <Resources/Shader.hpp>
#include <Resources/Texture.hpp>
#include <Engine/Transform3.hpp>



namespace Resources
{
	struct Vertex
	{
		Maths::Vector3f Position;
		Maths::Vector3f Normals;
		Maths::Vector2f TexCoords;
	};

	class Mesh
	{
	public:

		//	Constructors & Destructors
		//	--------------------------

		Mesh() = default;
		Mesh(const std::vector<Vertex> & verticesIn);
		~Mesh();
	
		//	Public Internal Function
		//	------------------------

		//	Draw mesh with the shader
		//	Parameters : const Shader* _shader
		//	----------------------------------
		void draw();
		std::string  getPath() const { return m_path; }
		std::string& setPath() { return m_path; }

	private:

		//	Private Internal Variables
		//	--------------------------

		std::string m_path;

		//	Private Internal Variables
		//	--------------------------

		//	Vertice list
		std::vector<Vertex> m_vertices;

		GLuint VAO;
		GLuint VBO;
		//GLuint TangentBuffer, BiTangentBuffer;

		//	Configure mesh with OpenGL
		//	Parameters : none
		//	-----------------
		void setupMesh();	
	};
}