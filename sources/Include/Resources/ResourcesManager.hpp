#pragma once

#include <map>

#include <Utils/Singleton.h>
#include <LowRenderer/Model.hpp>
#include <LowRenderer/Light.hpp>
#include <LowRenderer/Camera.hpp>


namespace Resources
{
	struct RawVerticesList
	{
		std::vector<Maths::Vector3f>	Pos;
		std::vector<Maths::Vector2f>	Tex;
		std::vector<Maths::Vector3f>	Nor;

		std::vector<Maths::Vector3f>	tan;
		std::vector<Maths::Vector2f>	bitan;

		std::vector<unsigned int>		indices;
		bool empty();
	};


	struct Character {
		unsigned int	TextureID = 0;  // ID handle of the glyph texture
		Maths::Vector2f Size;       // Size of glyph
		Maths::Vector2f Bearing;    // Offset from baseline to left/top of glyph
		unsigned int	Advance = 0;    // Offset to advance to next glyph
	};

	typedef std::map<char, Character> FontCharacter;
	typedef std::map<std::string, FontCharacter> FontList;
	typedef std::vector<std::string> stringList;

	class ResourcesManager : public Singleton<ResourcesManager>
	{
	private:

		//	Private Internal Variables
		//	-------------------------

		stringList m_loaded_text;
		//std::map<char, Character> m_characters;

		//	Parse a OBJ file
		//	Parameters : const std::string& path, const std::string& fileName, Resources::stringList& mesh_name_list
		//	------------------------------------------------------------------------------------------
		bool parseOBJ(const std::string& path, const std::string& fileName, Resources::stringList& mesh_name_list);

		//	Check if a texture has been loaded
		//	Parameters : const std::string& path
		//	------------------------------------
		bool textureLoaded(const std::string& path);
		
	public:
		//	Constructor
		//	-----------

		ResourcesManager();

		//	Public Internal Variables
		//	-------------------------

		std::map<std::string, stringList>			m_obj;
		std::map<std::string, Resources::Mesh>		m_meshName_mesh;
		std::map<std::string, Resources::Material>	m_materialName_material;
		std::map<std::string, std::string>			m_meshName_materialName;
		std::map<std::string, Resources::Shader>	m_shaderName_shader;
		std::map<std::string, Texture>				m_textureName_texture;

		FontList	m_characterListPerFonts;
		stringList	m_loaded_file;

		//	Public Internal Functions
		//	-------------------------

		//	Initialize the ResourceManager
		//	Parameters : none
		//	-----------------
		void initialize();

		//	Load OBJ file
		//	Parameters : const std::string& path, const std::string& fileName
		//	-----------------------------------------------------------------
		bool loadOBJ(const std::string& path, const std::string& fileName);

		//	Load Shader
		//	Parameters : const std::string& shader_name, const std::string& vertex_path, const std::string& fragment_path
		//	-------------------------------------------------------------------------------------------------------------
		void loadShader(const std::string& shader_name, const std::string& vertex_path, const std::string& fragment_path, const std::string& geometry_path);
		
		//	Parse a MTL file
		//	Parameters : const std::string& path, const std::string& fileName, Resources::stringList& material_name_list
		//	----------------------------------------------------------------------------------------------
		bool parseMTL(const std::string& path, const std::string& fileName, Resources::stringList& material_name_list);

		//	Load Texture from a file
		//	Parameters : const std::string& path, const std::string& text_name
		//	-----------------------------------------------------------
		Resources::Texture* loadTexture(const std::string& path, const std::string& text_name);

		//	Get Cube Map Texture from a file
		//	Parameters : const std::string& path, const std::string& text_name
		//	-----------------------------------------------------------
		Resources::Texture* getCubeMapTexture(const std::string& path, const std::string& text_name);

		//	Set Texture
		//	Parameters : const std::string& textName
		//	----------------------------------------
		void setTexture(const std::string& textName, Texture& text);

		//	Check if a file is loaded
		//	Parameters : const std::string& path
		//	-----------------------------
		bool fileLoaded(const std::string& path);
		
		//	Return Combo string of all textures
		//	Parameters : const std::string& path
		//	-----------------------------
		std::vector<const char*> getAllTextureName();


		//	Load font
		//	Paraneters : const std::string& path
		//	------------------------------------
		void loadFont(const std::string& path);

		void showImGuiWindows();
		void showImGUIResourcesManager();
	};
}
