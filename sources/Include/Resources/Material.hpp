#pragma once

#include <Resources/Texture.hpp>
#include <Maths/Vector3.h>


namespace Resources
{
	class Material
	{
	public:

		//	Constructor
		//	-----------

		Material() = default;

		//	Public Internal Variables
		//	-------------------------

		//	Colors

		Maths::Vector3f m_ambient = { 0,0,0 };
		Maths::Vector3f m_diffuse = { 0,0,0 };
		Maths::Vector3f m_specular = { 0,0,0 };
		Maths::Vector3f m_emissive = { 0,0,0 };

		//	Values

		float m_dissolve = 0;
		float m_shininess = 25.f;

		//	Textures

		TextureMap	m_text_diffuse;
		TextureMap	m_text_specular;
		TextureMap	m_text_emissive;
		TextureMap	m_text_dissolve;
		BumpMap		m_text_bump;

		//	Public Internal Function
		//	------------------------

		std::string  getPath() const { return m_path; }
		std::string& setPath() { return m_path; }

		void loadMaterial(std::ifstream& file);
		void showImGui();

		void saveInSCNFile(std::ofstream& file);
		void loadFromSCNFile(std::istringstream& lineStream);

		void setMaterialValue(const Material& mat);

	private:

		//	Private Internal Variables
		//	--------------------------

		std::string m_path;
	};
}