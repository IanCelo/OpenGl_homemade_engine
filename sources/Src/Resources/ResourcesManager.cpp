#include <iostream>
#include <iomanip>

#include <Core/Log.hpp>

#include <Resources/ResourcesManager.hpp>
#include <Utils/File.h>
#include <Utils/StringExtractor.h>

#include <imgui.h>

#include <ft2build.h>
#include FT_FREETYPE_H  

/*=================================== Constructor/ init ===================================*/

Resources::ResourcesManager::ResourcesManager()
{
	Core::Log* _log = Core::Log::instance();

	_log->write("Creating ResourcesManager");

	loadFont("Resource/Fonts/abnes.ttf");
	loadFont("Resource/Fonts/arial.ttf");
}

void Resources::ResourcesManager::initialize()
{
	Resources::Texture("Assets/none.png", "None");

	Resources::Material none_mat;

	none_mat.m_diffuse = { 1.000f,.350f, 1.000f };
	none_mat.m_emissive = { 1.000f,.350f, 1.000f };

	none_mat.m_text_diffuse = TextureMap(&m_textureName_texture["None"]);

	m_materialName_material["None"] = none_mat;
}


/*=================================== Verifications ===================================*/

bool Resources::RawVerticesList::empty()
{
	if (this->Pos.empty()) return true;

	return false;
}

bool Resources::ResourcesManager::fileLoaded(const std::string& path)
{
	for (std::string curr_file : m_loaded_file)
	{
		if (path == curr_file)
		{
			return true;
		}
	}
	m_loaded_file.push_back(path);

	return false;
}

bool Resources::ResourcesManager::textureLoaded(const std::string& name)
{
	for (std::string curr_file : m_loaded_text)
	{
		if (name == curr_file)
		{
			return true;
		}
	}
	m_loaded_text.push_back(name);

	return false;
}

/*=================================== Sets / gets ===================================*/

//	Set Indices in order
void setIndices(std::istringstream& line, std::vector<unsigned int>& indices)
{
	for (int i = 0; i < 3; i++)
	{
		unsigned int indice[3];

		for (int j = 0; j < 3; j++)
		{
			line >> indice[j];
			line.ignore();		//	Ignore : '/'
		}

		indices.push_back(indice[0] - 1);	//	Coordinates	indices
		indices.push_back(indice[1] - 1);	//	Texture UV'	indices
		indices.push_back(indice[2] - 1);	//	Normal		indices
	}
}


//	Sort Vertices and place them in a new mesh
Resources::Mesh setMesh(Resources::RawVerticesList& rawList)
{
	std::vector<Resources::Vertex> vertices;

	for (size_t i = 0; i < rawList.indices.size(); i += 3)
	{
		Resources::Vertex	vrt;
		vrt.Position = rawList.Pos[rawList.indices[i]];
		vrt.TexCoords = rawList.Tex[rawList.indices[i + 1]];
		vrt.Normals = rawList.Nor[rawList.indices[i + 2]];


		vertices.push_back(vrt);
	}

	//	Clear the RawList Indices, because, they do not reset 
	//	(Same indices for every meshes) Unlike vertices informations
	rawList.indices.clear();

	Resources::Mesh out(vertices);
	vertices.clear();

	return out;
}

Resources::Texture* Resources::ResourcesManager::loadTexture(const std::string& path, const std::string& text_name)
{ 
	if (!textureLoaded(text_name))
	{
		Texture newTexture(path, text_name);

		if (newTexture.getID() == 0)
		{
			return nullptr;
		}
	}
	return &m_textureName_texture[text_name];
}



Resources::Texture* Resources::ResourcesManager::getCubeMapTexture(const std::string& path, const std::string& text_name)
{
	if (!textureLoaded(text_name))
	{

		std::ifstream file;

		if (!FileParser::openFile(path, file)) return &m_textureName_texture["None"];

		int i = 0;

		std::vector<std::string> faces;

		std::string currentLine = "";
		std::string dir = Extractor::ExtractDirectory(path);

		while (std::getline(file, currentLine))
		{

			if (i > 6)
			{
				//  Put log warning
				break;
			}

			faces.push_back(dir + currentLine);

			i++;
		}
		file.close();

		TextureCubeMap newTexture(text_name, faces);

		if (newTexture.getID() == 0)
		{
			return &m_textureName_texture["None"];
		}
	}

	return &m_textureName_texture[text_name];
}

std::vector<const char*> Resources::ResourcesManager::getAllTextureName()
{
	std::vector<const char*> names;
	for (const auto& tex : m_textureName_texture)
	{
		names.push_back(tex.first.c_str());
	}
	return names;
}

void  Resources::ResourcesManager::setTexture(const std::string& textName, Texture& text)
{
	m_textureName_texture[textName] = text;
}

void Resources::ResourcesManager::loadShader(const std::string& shader_name, const std::string& vertex_path, const std::string& fragment_path, const std::string& geometry_path)
{
	if (!fileLoaded(shader_name))
	{
		m_shaderName_shader[shader_name] = Resources::Shader(vertex_path, fragment_path, geometry_path);
	}
}


void Resources::ResourcesManager::loadFont(const std::string& path)
{
	Core::Log* _log = Core::Log::instance();

	if (fileLoaded(path)) return;


	FT_Library ft;
	if (FT_Init_FreeType(&ft))
	{
		_log->writeError("FreeType -> Could not init FreeType Library");
	}

	FT_Face face;

	if (FT_New_Face(ft, path.c_str(), 0, &face))
	{
		_log->writeFailure("FreeType -> Failed to load font : \"" + path + "\"");
		return;
	}
	else
	{
		_log->writeSuccess("FreeType -> loaded font : \"" + path + "\"");
	}

	FT_Set_Pixel_Sizes(face, 0, 96);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction


	for (unsigned char c = 0; c < 128; c++)
	{
		// load character glyph 

		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
			continue;
		}
		// generate texture

		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

		// set texture options

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// now store character for later use

		Character character = {
			texture,
			{(float)face->glyph->bitmap.width, (float)face->glyph->bitmap.rows},
			{(float)face->glyph->bitmap_left, (float)face->glyph->bitmap_top},
			(unsigned int)face->glyph->advance.x
		};

		//m_characterListPerFonts[Extractor::ExtractFilename(path)].insert(std::pair<char, Character>(c, character));
		m_characterListPerFonts[Extractor::ExtractFilename(path)][c] = character; // .insert(std::pair<char, Character>(c, character));
	}

	FT_Done_Face(face);
	FT_Done_FreeType(ft);
}

/*=================================== Parser ===================================*/


bool Resources::ResourcesManager::parseMTL(const std::string& path, const std::string& fileName, stringList& material_name_list)
{
	std::ifstream file;
	if (FileParser::openFile(path + fileName, file) == false)
	{
		return false;
	}

	std::string currentLine;
	std::string fileNameWithoutExtension = Extractor::ExtractNameWithoutExtension(fileName);

	Resources::Material new_mat;
	new_mat.setPath() = path + fileName;

	bool is_empty = true;
	while (std::getline(file, currentLine))
	{
		if (currentLine[0] == '#' || currentLine[0] == '\n') continue;

		std::istringstream lineStream(currentLine);

		//	Get the type of the next values
		std::string type = FileParser::getString(lineStream);

		//	Material - New One
		if (type == "newmtl")
		{
			material_name_list.push_back(fileNameWithoutExtension + "_" + FileParser::getString(lineStream));
			new_mat.loadMaterial(file);
			m_materialName_material[material_name_list.back()] = new_mat;

			//is_empty = true;
			continue;
		}
	}

	file.close();
	return true;
}


bool putInRawList(std::istringstream& lineStream, std::string& type, Resources::RawVerticesList& out)
{
	if (type == "v") out.Pos.push_back(FileParser::getVector3(lineStream));

	else if (type == "vt") out.Tex.push_back(FileParser::getVector2(lineStream));

	else if (type == "vn") out.Nor.push_back(FileParser::getVector3(lineStream));

	else if (type == "f")  setIndices(lineStream, out.indices);

	else return false;

	return true;
}


//	Parse Object file
//	-----------------
bool Resources::ResourcesManager::parseOBJ(const std::string& path, const std::string& fileName, Resources::stringList& mesh_name_list)
{
	if (FileParser::CheckExtension(std::string(fileName), "obj") == false) return false;

	std::ifstream file;
	if (FileParser::openFile(path + fileName, file) == false) return false;

	Resources::stringList	material_name_list;
	RawVerticesList			rawList;

	std::string		currentLine;
	std::string		mtl_file;

	bool has_material_lib = false;

	while (std::getline(file, currentLine))
	{
		if (currentLine[0] == '#' || currentLine[0] == '\n') continue;

		std::istringstream lineStream(currentLine);

		//	Get the type of the next values
		std::string	type = FileParser::getString(lineStream);

		//	Get Unsorted Vertice data
		if (putInRawList(lineStream, type, rawList)) continue;

		if (type == "o")
		{
			//	if the rawList is not empty, add previous charged meshes to the mesh list
			if (!rawList.empty())
			{
				m_meshName_mesh[mesh_name_list.back()] = setMesh(rawList);
			}

			//	THEN add the new name to the mesh name list
			mesh_name_list.push_back(FileParser::getString(lineStream));

			continue;
		}

		//	Save the material used by the mesh in the "used material" list
		if (type == "usemtl")
		{
			if (has_material_lib)
			{
				//	Get material name
				std::string mtl_name = Extractor::ExtractNameWithoutExtension(mtl_file) + "_" + FileParser::getString(lineStream);

				bool exist = false;

				//	Search if this material exist
				for (std::string curr : material_name_list)
				{
					if (curr == mtl_name) exist = true;
				}

				//	if it exist, just add it to the used material list
				if (exist)
				{
					m_meshName_materialName[mesh_name_list.back()] = mtl_name;
					continue;
				}
			}

			//	Else if there isn't material lib OR the material doesn't exist,
			//	the used material is None for the current mesh
			//  si tu lis ca t'es bo
			m_meshName_materialName[mesh_name_list.back()] = "None";

			continue;
		}

		//	Load material file that contain differents materials used by the OBJ
		if (type == "mtllib")
		{
			//	Parse MTL file
			lineStream >> mtl_file;
			const char* mtl_file_name = mtl_file.c_str();

			if (parseMTL(path, mtl_file_name, material_name_list))
			{
				has_material_lib = true;
			}
			continue;
		}
	}

	//	Adding last mesh to the mesh list
	m_meshName_mesh[mesh_name_list.back()] = setMesh(rawList);
	m_meshName_mesh[mesh_name_list.back()].setPath() = path;
	file.close();

	return true;
}


bool Resources::ResourcesManager::loadOBJ(const std::string& path, const std::string& fileName)
{
	Core::Log* _log = Core::Log::instance();

	if (fileLoaded(path + fileName) == false)
	{
		_log->write("+\t\t Loading new OBJ file");
		_log->breakLine();
	}
	else
	{
		_log->breakLine();
		return true;
	}

	stringList		mesh_name_list;


	int previ_mesh_count = (int)mesh_name_list.size();
	if (parseOBJ(path, fileName, mesh_name_list) == false)
	{
		_log->writeFailure("Failed to Parse \"" + path + fileName + "\"");
		return false;
	}

	//	Let's stock Meshes name in the Object
	m_obj[std::string(fileName)] = mesh_name_list;
	mesh_name_list.clear();

	return true;
}

#include <imgui.h>

void Resources::ResourcesManager::showImGUIResourcesManager()
{
	if (ImGui::CollapsingHeader("Resources", ImGuiTreeNodeFlags_DefaultOpen))
	{
		for (auto& mat : m_materialName_material)
		{
			if (ImGui::TreeNode(mat.first.c_str()))
			{
				mat.second.showImGui();
				ImGui::TreePop();
			}
		}

	}
}


void Resources::ResourcesManager::showImGuiWindows()
{
	if (ImGui::Begin("ResourcesManager"))
	{
		//	Object Loader
		ImGui::PushID("ObjectLoader");
		ImGui::Text("Object Loader");

		static char OBJPath[256] = "Assets/";
		ImGui::InputText("###OBJPATH", OBJPath, IM_ARRAYSIZE(OBJPath));
		ImGui::SameLine();

		if (FileParser::CheckExtension(OBJPath, "obj"))
		{
			if (ImGui::Button("Load", { 50,20 }))
			{
				loadOBJ(
					Extractor::ExtractDirectory(std::string(OBJPath)),
					Extractor::ExtractFilename(std::string(OBJPath))
				);
			}
		}
		else ImGui::NewLine();
		ImGui::PopID();

		//	Texture Loader
		ImGui::PushID("TextureLoader");
		ImGui::Text("Texture Loader");

		static std::string acceptedImageExtension[4] =
		{
			"png", "jpeg", "jpg", "bmp"
		};

		static char TexturePath[256] = "Assets/";
		ImGui::InputText("###TEXPATH", TexturePath, IM_ARRAYSIZE(TexturePath));
		ImGui::SameLine();

		if (FileParser::CheckExtension(TexturePath, acceptedImageExtension, IM_ARRAYSIZE(acceptedImageExtension)))
		{
			if (ImGui::Button("Load", { 50,20 }))
			{
				loadTexture(
					TexturePath,
					Extractor::ExtractBeforeLast(Extractor::ExtractFilename(std::string(TexturePath)), '.', false)
				);
			}
		}
		else ImGui::NewLine();
		ImGui::PopID();

		//	Shader Loader
		ImGui::PushID("ShaderLoader");
		ImGui::Text("Shader Loader");

		static char ShaderName[256];
		ImGui::Text("Resource/Shader/");
		ImGui::InputText("###SHADPATH", ShaderName, IM_ARRAYSIZE(ShaderName));
		ImGui::SameLine();

		if (FileParser::CheckExtension(ShaderName, "shad"))
		{
			if (ImGui::Button("Load", { 50,20 }))
			{
				Resources::loadShader(Extractor::ExtractBeforeLast(ShaderName, '.', false));
			}
		}
		else ImGui::NewLine();
		ImGui::PopID();

		//	MTL Loader
		ImGui::PushID("MTLLoader");
		ImGui::Text("MTL Loader");

		static char MTLPath[256] = "Assets/";
		ImGui::InputText("###MTLPATH", MTLPath, IM_ARRAYSIZE(MTLPath));
		ImGui::SameLine();

		if (FileParser::CheckExtension(MTLPath, "mtl"))
		{
			if (ImGui::Button("Load", { 50,20 }))
			{
				Resources::stringList nonUsed;
				parseMTL(
					Extractor::ExtractDirectory(std::string(MTLPath)),
					Extractor::ExtractFilename(std::string(MTLPath)),
					nonUsed
				);

			}
		}
		else ImGui::NewLine();
		ImGui::PopID();
	}
	ImGui::End();
}
