#pragma once

#include <Resources/Texture.hpp>
#include <Resources/ResourcesManager.hpp>
#include <Resources/Shader.hpp>
#include <unordered_map>

namespace LowRenderer
{
	class CubeMap
	{
	private:

		//  Initialize cube
		void initRenderData();

		Resources::Shader* m_shader;

		GLuint VAO;

	public:
		CubeMap() = default;
		CubeMap(std::string path);

		void draw();

		Resources::Texture* m_texture;
	};
}