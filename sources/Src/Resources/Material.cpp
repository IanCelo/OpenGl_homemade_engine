#include <Resources/Material.hpp>
#include <Resources/ResourcesManager.hpp>
#include <Utils/StringExtractor.h>

#include <Utils/File.h>
#include <vector>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>


void Resources::Material::loadMaterial(std::ifstream& file)
{
	ResourcesManager* _resource = ResourcesManager::instance();

	std::string currentLine;
	bool is_empty = true;

	std::streampos pos = file.tellg();

	while (std::getline(file, currentLine))
	{
		if (currentLine[0] == '#' || currentLine[0] == '\n') continue;

		std::istringstream lineStream(currentLine);

		//	Get the type of the next values
		std::string type = FileParser::getString(lineStream);

		
		//	Material Shininess
		if (type == "Ns")
		{
			m_shininess = FileParser::getFloat(lineStream);
			is_empty = false;
		}

		//	Material - Ambient Color
		else if (type == "Ka")
		{
			m_ambient = FileParser::getVector3(lineStream);
			is_empty = false;
		}

		//	Material - Shininess Color
		else if (type == "Kd")
		{
			m_diffuse = FileParser::getVector3(lineStream);
			is_empty = false;
		}

		//	Material - Specular Color
		else if (type == "Ks")
		{
			m_specular = FileParser::getVector3(lineStream);
			is_empty = false;
		}

		//	Material - Emissive Color
		else if (type == "Ke")
		{
			m_emissive = FileParser::getVector3(lineStream);
			is_empty = false;
		}

		//	Material - Dissolve Factor
		else if (type == "d")
		{
			m_dissolve = FileParser::getFloat(lineStream);
			is_empty = false;
		}
		
		if (type.substr(0, 3) == "map" || type == "bump")
		{

			Vector3f offset = { 0.f, 0.f, 0.f };
			Vector3f tiling = { 1.f, 1.f, 1.f };

			//	Bump only
			float multiplier = 1.0f;

			std::string str = FileParser::getString(lineStream);

			//	Map Options
			while (str != "")
			{
				if (str == "-s")
				{
					tiling = FileParser::getVector3(lineStream);
				}
				else if (str == "-o")
				{
					offset = FileParser::getVector3(lineStream);
				}
				else if (str == "-bm")
				{
					multiplier = FileParser::getFloat(lineStream);
				}
				else break;
				
				str = FileParser::getString(lineStream);
			}

			std::string name = str;
			std::string path = std::string(Extractor::ExtractBeforeLast(m_path, '/', true) + name);

			//	Material - Diffuse Texture
			if (type == "map_Kd")
			{
				m_text_diffuse = TextureMap(_resource->loadTexture(path, name), offset, tiling);
			}

			//	Material - Emissive Texture
			else if (type == "map_Ke")
			{
				m_text_emissive = TextureMap(_resource->loadTexture(path, name), offset, tiling);
			}

			//	Material - Dissolve Texture
			else if (type == "map_d")
			{
				m_text_dissolve = TextureMap(_resource->loadTexture(path, name), offset, tiling);
			}

			//	Material - Normal Texture
			else if (type == "bump")
			{
				m_text_bump = BumpMap(_resource->loadTexture(path, name), offset, tiling, multiplier);
			}
		}

		//	Material - New One
		if (type == "newmtl")
		{
			file.seekg(pos);
			return;
		}

		pos = file.tellg();
	}
}

void Resources::Material::showImGui()
{
	ImGui::ColorEdit3("Ambient color", &m_ambient.x);
	ImGui::ColorEdit3("Diffuse color", &m_diffuse.x);
	ImGui::ColorEdit3("Specular color", &m_specular.x);
	ImGui::ColorEdit3("Emissive color", &m_emissive.x);
	ImGui::SliderFloat("Shininess", &m_shininess, 0, 100.f);

	ImGui::Text("Map");

	ImGui::Text("Diffuse Map");
	ImGui::PushID("diffuse");
	m_text_diffuse.showImGui();
	ImGui::PopID();


	ImGui::Text("Emissive Map");
	ImGui::PushID("emissive");
	m_text_emissive.showImGui();
	ImGui::PopID();

	ImGui::Text("Normal Map");
	ImGui::PushID("normal");
	m_text_bump.showImGui();
	ImGui::PopID();

	ImGui::Text("Specular Map");
	ImGui::PushID("specular");
	m_text_specular.showImGui();
	ImGui::PopID();

	ImGui::Text("Dissolve Map");
	ImGui::PushID("dissolve");
	m_text_dissolve.showImGui();
	ImGui::PopID();
}

void Resources::Material::saveInSCNFile(std::ofstream& file)
{
	FileWriter::writeVec3InFile(file, m_ambient); file << "/";
	FileWriter::writeVec3InFile(file, m_diffuse); file << "/";
	FileWriter::writeVec3InFile(file, m_specular); file << "/";
	FileWriter::writeVec3InFile(file, m_emissive); file << "/";
	file << m_shininess << "/";

	FileWriter::writeVec3InFile(file, m_text_diffuse.m_offset); file << "/";
	FileWriter::writeVec3InFile(file, m_text_diffuse.m_tiling); file << "/";
	FileWriter::writeVec3InFile(file, m_text_emissive.m_offset); file << "/";
	FileWriter::writeVec3InFile(file, m_text_emissive.m_tiling); file << "/";
	FileWriter::writeVec3InFile(file, m_text_bump.m_offset); file << "/";
	FileWriter::writeVec3InFile(file, m_text_bump.m_tiling); file << "/";
	FileWriter::writeVec3InFile(file, m_text_specular.m_offset); file << "/";
	FileWriter::writeVec3InFile(file, m_text_specular.m_tiling); file << "/";
	FileWriter::writeVec3InFile(file, m_text_dissolve.m_offset); file << "/";
	FileWriter::writeVec3InFile(file, m_text_dissolve.m_tiling); file << "/";
}

void Resources::Material::loadFromSCNFile(std::istringstream& lineStream)
{
	m_ambient = FileParser::getVector3(lineStream); lineStream.ignore();
	m_diffuse = FileParser::getVector3(lineStream); lineStream.ignore();
	m_specular = FileParser::getVector3(lineStream); lineStream.ignore();
	m_emissive = FileParser::getVector3(lineStream); lineStream.ignore();
	m_shininess = FileParser::getFloat(lineStream); lineStream.ignore();
	m_text_diffuse.m_offset = FileParser::getVector3(lineStream); lineStream.ignore();
	m_text_diffuse.m_tiling = FileParser::getVector3(lineStream); lineStream.ignore();
	m_text_emissive.m_offset = FileParser::getVector3(lineStream); lineStream.ignore();
	m_text_emissive.m_tiling = FileParser::getVector3(lineStream); lineStream.ignore();
	m_text_bump.m_offset = FileParser::getVector3(lineStream); lineStream.ignore();
	m_text_bump.m_tiling = FileParser::getVector3(lineStream); lineStream.ignore();
	m_text_specular.m_offset = FileParser::getVector3(lineStream); lineStream.ignore();
	m_text_specular.m_tiling = FileParser::getVector3(lineStream); lineStream.ignore();
	m_text_dissolve.m_offset = FileParser::getVector3(lineStream); lineStream.ignore();
	m_text_dissolve.m_tiling = FileParser::getVector3(lineStream);
}

void Resources::Material::setMaterialValue(const Material& mat)
{
	m_ambient = mat.m_ambient;
	m_diffuse = mat.m_diffuse;
	m_specular = mat.m_specular;
	m_emissive = mat.m_emissive;
	m_shininess = mat.m_shininess;

	m_text_diffuse.m_offset = mat.m_text_diffuse.m_offset;
	m_text_diffuse.m_tiling = mat.m_text_diffuse.m_tiling;
	m_text_emissive.m_offset = mat.m_text_emissive.m_offset;
	m_text_emissive.m_tiling = mat.m_text_emissive.m_tiling;
	m_text_bump.m_offset = mat.m_text_bump.m_offset;
	m_text_bump.m_tiling = mat.m_text_bump.m_tiling;
	m_text_specular.m_offset = mat.m_text_specular.m_offset;
	m_text_specular.m_tiling = mat.m_text_specular.m_tiling;
	m_text_dissolve.m_offset = mat.m_text_dissolve.m_offset;
	m_text_dissolve.m_tiling = mat.m_text_dissolve.m_tiling;
}