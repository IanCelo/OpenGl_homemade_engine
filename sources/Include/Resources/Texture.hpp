#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Maths/Vector3.h>
#include <vector>

namespace Resources
{
	enum class TextureType
	{
		Default,
		CubeMap
	};
	
	class Texture
	{
	private:

		//	Private Internal Functions
		//	--------------------------

		void generateTexture(const GLenum format1, const GLenum format2, const float* data);

	protected:

		//	Protected Internal Variables
		//	----------------------------
		GLuint	m_ID = 0;

		//bool loadTexture(const std::string& filename, const std::string& texName);

	public:

		//	Constructor & Destructor
		//	------------------------
		Texture() = default;
		Texture(const std::string& filename, const std::string& textName);

		int m_width = 0;
		int m_height = 0;

		std::string m_path;
		std::string m_name;

		//	Public Internal Functions
		//	-------------------------	
		void bind(const int bindID) const;
		GLuint getID() const { return m_ID; }

		void showImGui() const;
	};

	class TextureCubeMap : public Texture
	{
	private :

		//	Private Internal Functions
		//	-------------------------	

		bool generateTexture(const std::vector<std::string>& faces);

	public:

		//	Constructor & Destructor
		//	------------------------
		TextureCubeMap() = default;
		TextureCubeMap(const std::string& textName,const std::vector<std::string>& faces);

	};


	class TextureMap
	{
	protected:

		//	Protected Internal Variables
		//	----------------------------

		const Texture* m_texture = nullptr;

	public:

		//	Constructor & Destructor
		//	------------------------

		TextureMap() = default;
		TextureMap(Texture * in_texture);
		TextureMap(Texture* in_texture, const Maths::Vector3f& in_offset, const Maths::Vector3f& in_tiling);

		//	Public Internal Variables
		//	-------------------------

		Maths::Vector3f m_offset = { 0.0f, 0.0f, 0.0f };
		Maths::Vector3f m_tiling = { 1.0f, 1.0f, 1.0f };

		void bind(const int bindID) const;
		GLuint getTextureID() const;

		virtual void showImGui();
	};



	class BumpMap : public TextureMap
	{
	public:

		BumpMap() = default;
		BumpMap(Texture * in_texture, const Maths::Vector3f & in_offset, const Maths::Vector3f & in_tiling, const float in_multiplier);

		float m_multiplier = 1.0f;
		void showImGui() override;
	};
}