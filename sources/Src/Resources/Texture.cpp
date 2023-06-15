#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <Core/Log.hpp>
#include <Resources/Texture.hpp>
#include <Resources/ResourcesManager.hpp>

#include <Utils/StringExtractor.h>

#include <Resources/ResourcesManager.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

Resources::Texture::Texture(const std::string& filename, const std::string& textName)
{
	//	Log Instance
	Core::Log* _log = Core::Log::instance();

	float* data;
	int		colorType;

	//	Flip the texture vertically so it isn't inverted
	stbi_set_flip_vertically_on_load(true);

	m_path = Extractor::ExtractDirectory(filename);
	m_name = textName;

	//	Load the texture data
	//	If datas are successfully loaded
	if (data = stbi_loadf(filename.c_str(), &m_width, &m_height, &colorType, 0))
	{
		GLenum format1;
		GLenum format2;

		/**/ if (colorType == 1) { format1 = format2 = GL_RED; }
		else if (colorType == 3) { format1 = GL_RGB;  format2 = GL_RGB; }
		else if (colorType == 4) { format1 = GL_RGBA;  format2 = GL_RGBA; }

		//	Set new Texture in openGL
		generateTexture(format1, format2, data);

		Resources::ResourcesManager* _resources = Resources::ResourcesManager::instance();
		_resources->setTexture(textName, *this);

		//	Clean data
		stbi_image_free(data);

		_log->writeSuccess("Loaded texture \"" + std::string(filename) + "\"");

		return;
	}
	//	Else the texture loading failed
	_log->writeFailure("Failed to load texture \"" + std::string(filename) + "\"");
}

void Resources::Texture::generateTexture(const GLenum format1, const GLenum format2, const float* data)
{
	glGenTextures(1, &m_ID);
	glBindTexture(GL_TEXTURE_2D, m_ID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//	Automatically manage if the texture is RGB or RGBA
	//	--------------------------------------------------

	glTexImage2D(GL_TEXTURE_2D, 0, format1, m_width, m_height, 0, format2, GL_FLOAT, data);

	glGenerateMipmap(GL_TEXTURE_2D);

	//	Reset
	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}


void  Resources::Texture::showImGui() const
{
	if ((void*)(intptr_t)m_ID)
	{
		ImGui::Image((void*)(intptr_t)m_ID, ImVec2(128, 128),ImVec2(0,1), ImVec2(1,0));
	}
}



void Resources::Texture::bind(const int bindID) const
{
	glActiveTexture(bindID);
	glBindTexture(GL_TEXTURE_2D, m_ID);
}


//	---------------------------------------------------------

//	CubeMap Texture
//	---------------

Resources::TextureCubeMap::TextureCubeMap(const std::string& textName, const std::vector<std::string>& faces)
{
	//	Log Instance
	Core::Log* _log = Core::Log::instance();

	//int		colorType;

	//	Flip the texture vertically so it isn't inverted
	stbi_set_flip_vertically_on_load(true);

	//	Load the texture data
	//	If datas are successfully loaded

	if (generateTexture(faces))
	{
		Resources::ResourcesManager* _resources = Resources::ResourcesManager::instance();
		_resources->setTexture(textName, (*this));

	}
	else
	{
		//	Else the texture loading failed
		_log->writeFailure("Failed to load CubeMap texture \"" + std::string(textName) + "\"");
	}
}

//	Generate cube map Texture
bool Resources::TextureCubeMap::generateTexture(const std::vector<std::string>& faces)
{
	//	Log Instance
	Core::Log* _log = Core::Log::instance();

	glGenTextures(1, &m_ID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_ID);


	for (unsigned int i = 0; i < faces.size(); i++)
	{
		int colorType;
		float* data = stbi_loadf(faces[i].c_str(), &m_width, &m_height, &colorType, 0);
		if (data)
		{
			GLenum format1;
			GLenum format2;

			/**/ if (colorType == 1) { format1 = format2 = GL_RED; }
			else if (colorType == 3) { format1 = GL_RGB;  format2 = GL_RGB; }
			else if (colorType == 4) { format1 = GL_RGBA;  format2 = GL_RGBA; }


			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format1, m_width, m_height, 0, format2, GL_FLOAT, data);
			stbi_image_free(data);
		}
		else
		{
			_log->writeFailure("Cubemap texture failed to load at path: \"" + faces[i] + "\"");
			stbi_image_free(data);
			return false;
		}
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return true;
}


//	---------------------------------------------------------

//	Map Texture
//	-----------


Resources::TextureMap::TextureMap(Texture* in_texture)
{
	m_texture = in_texture;
}

Resources::TextureMap::TextureMap(Texture* in_texture, const Maths::Vector3f& in_offset, const Maths::Vector3f& in_tiling)
{
	m_texture = in_texture;
	m_offset = in_offset;
	m_tiling = in_tiling;
}


void Resources::TextureMap::bind(const int bindID) const
{
	glActiveTexture(bindID);
	glBindTexture(GL_TEXTURE_2D, getTextureID());
}

GLuint Resources::TextureMap::getTextureID() const
{
	if (m_texture) return m_texture->getID();
	return 0;
}


void Resources::TextureMap::showImGui()
{
	if (m_texture)
	{
		ImGui::DragFloat3("Offset", &m_offset.x);
		ImGui::DragFloat3("Tiling", &m_tiling.x);

		m_texture->showImGui();
	}
}



//	Bump Map Texture

Resources::BumpMap::BumpMap(Texture* in_texture, const Maths::Vector3f& in_offset, const Maths::Vector3f& in_tiling, const float in_multiplier) : TextureMap(in_texture, in_offset, in_tiling)
{
	m_multiplier = in_multiplier;
}


void Resources::BumpMap::showImGui()
{
	if (m_texture)
	{
		ImGui::DragFloat3("Offset", &m_offset.x);
		ImGui::DragFloat3("Tiling", &m_tiling.x);
		ImGui::SliderFloat("Multiplier", &m_multiplier,0.f, 5.f);

		m_texture->showImGui();
	}
}
