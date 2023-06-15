#include <iostream>
#include <glad/glad.h>

#include <Core/Log.hpp>
         
#include <Resources/ResourcesManager.hpp>
#include <Resources/Shader.hpp>
         
#include <Utils/File.h>
         
#include <Maths/Matrix.h>
         
#include <LowRenderer/Light.hpp>

#include <Engine/Transform3.hpp>

std::string loadStringFromFile(const std::string& path)
{
    std::ifstream file;
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    std::string out;


   file.open(path);
   std::stringstream sstream;

   sstream << file.rdbuf();

   file.close();

   out = sstream.str();
    
    return out;
}


void checkCompileErrors(unsigned int shad, const std::string& shaderName)
{
    Core::Log* _log = Core::Log::instance();

    int success;
    char infoLog[512];

    //  print compile errors if any
    glGetShaderiv(shad, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shad, 512, NULL, infoLog);
        _log->writeError(shaderName + " ->\tcompilation failed");
        _log->write(infoLog);
    }
    else
    {
        _log->writeSuccess(shaderName + " ->\tSuccessfully compiled");
    }
}

Resources::Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath)
{
    Core::Log* _log = Core::Log::instance();

    //  1. retrieve the vertex/fragment source code from filePath
    //  ---------------------------------------------------------

    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;

    bool hasGeometry = geometryPath != "";

    try
    {
        vertexCode = loadStringFromFile(vertexPath);
        fragmentCode = loadStringFromFile(fragmentPath);
    }
    catch (std::ifstream::failure e)
    {
        _log->writeError("SHADER ->\tFILE NOT SUCCESSFULLY READED");
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    //  2. compile shaders
    //  ------------------

    unsigned int vertex, fragment, geometry;
    int success;
    char infoLog[512];

    //  Vertex Shader
    {
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);

        checkCompileErrors(vertex, "Vertex Shader");
    }

    //  Fragment Shader
    {
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);

        checkCompileErrors(fragment, "Fragment Shader");
    }

    //  Geometry Shader
    {
        // if geometry shader is given, compile geometry shader
        if (hasGeometry)
        {
            const char* gShaderCode = geometryCode.c_str();
            geometry = glCreateShader(GL_GEOMETRY_SHADER);
            glShaderSource(geometry, 1, &gShaderCode, NULL);
            glCompileShader(geometry);

            checkCompileErrors(geometry, "Geometry Shader");
        }
    }

    //  shader Program

    {
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        if (hasGeometry) glAttachShader(ID, geometry);
        glLinkProgram(ID);

        //  print linking errors if any
        glGetProgramiv(ID, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(ID, 512, NULL, infoLog);
            _log->writeError("SHADER PROGRAM ->\tLINKING_FAILED");
            _log->write(infoLog);
        }
        else
        {
            _log->writeSuccess("Shader program ->\tSuccessfully linked");
        }
    }

    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    if(hasGeometry) glDeleteShader(geometry);

    use();

    setInt("mat.diffuseMap.text", 0);
    setInt("mat.normalMap.map.text", 1);
    setInt("mat.specularMap.text", 2);
    setInt("mat.emissiveMap.text", 3);
    setInt("mat.maskMap.text", 4);
}

void Resources::Shader::use()
{
    glUseProgram(ID);
}

void Resources::Shader::setBool(const std::string& name, const bool value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Resources::Shader::setInt(const std::string& name, const int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Resources::Shader::setFloat(const std::string& name, const float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Resources::Shader::setFloat2(const std::string& name, const Maths::Vector2f& value) const
{
    glUniform2f(glGetUniformLocation(ID, name.c_str()), value.x, value.y);
}

void Resources::Shader::setFloat3(const std::string& name, const Maths::Vector3f& value) const
{
    glUniform3f(glGetUniformLocation(ID, name.c_str()), value.x, value.y, value.z);
}

void Resources::Shader::setFloat4(const std::string& name, const  Maths::Vector4f& value) const
{
    glUniform4f(glGetUniformLocation(ID, name.c_str()), value.x, value.y, value.z, value.w);
}

void Resources::Shader::setMat4(const std::string& name, const Maths::Mat4x4& value) const
{
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, value.e);
}

void Resources::Shader::setLights(const std::unordered_map<int, const Light*>& list) const
{

    setInt("LightNumber", (int)list.size());
    int i = 0;
    for (auto& curr_light : list)
    {
        std::string l = "light_list[" + std::to_string(i) + "].";

        setBool(l + "enabled", curr_light.second->isActive());

        setFloat3(l + "position", curr_light.second->m_transform->getWorldPosition());

        setFloat3(l + "ambient", curr_light.second->ambient);
        setFloat3(l + "diffuse", curr_light.second->diffuse);
        setFloat3(l + "specular", curr_light.second->specular);

        setFloat3(l + "attenuation", curr_light.second->attenuation);
        setFloat3(l + "direction", curr_light.second->direction);
        
        setFloat(l + "power", curr_light.second->power);
        setFloat(l + "cutOff", curr_light.second->cutOff);
        setFloat(l + "outerCutOff", curr_light.second->outerCutOff);

        setInt(l + "lightType", curr_light.second->lightType);

        i++;
    }
}


void Resources::Shader::setMaterial(const Resources::Material& in_material) const
{
    //  Send materials data to the GPU
    //  ------------------------------

    setFloat3("mat.ambient", in_material.m_ambient);
    setFloat3("mat.diffuse", in_material.m_diffuse);
    setFloat3("mat.specular", in_material.m_specular);
    setFloat3("mat.emissive", in_material.m_emissive);

    setFloat("mat.shininess", in_material.m_shininess);    

    //  Texture
    //  -------

    //  Diffuse Texture

    setBool("mat.diffuseMap.exist", in_material.m_text_diffuse.getTextureID() != 0);
    setFloat3("mat.diffuseMap.offset", in_material.m_text_diffuse.m_offset);
    setFloat3("mat.diffuseMap.tiling", in_material.m_text_diffuse.m_tiling);
    in_material.m_text_diffuse.bind(GL_TEXTURE0);

    //  Normal Texture

    setBool("mat.normalMap.map.exist", in_material.m_text_bump.getTextureID() != 0);
    setFloat3("mat.normalMap.map.offset", in_material.m_text_bump.m_offset);
    setFloat3("mat.normalMap.map.tiling", in_material.m_text_bump.m_tiling);
    setFloat("mat.normalMap.multiplier", in_material.m_text_bump.m_multiplier);
    in_material.m_text_bump.bind(GL_TEXTURE1);

    //  Specular Texture

    setBool("mat.specularMap.exist", in_material.m_text_specular.getTextureID() != 0);
    setFloat3("mat.specularMap.offset", in_material.m_text_specular.m_offset);
    setFloat3("mat.specularMap.tiling", in_material.m_text_specular.m_tiling);
    in_material.m_text_specular.bind(GL_TEXTURE2);

    //  Emissive Texture

    setBool("mat.emissiveMap.exist", in_material.m_text_emissive.getTextureID() != 0);
    setFloat3("mat.emissiveMap.offset", in_material.m_text_emissive.m_offset);
    setFloat3("mat.emissiveMap.tiling", in_material.m_text_emissive.m_tiling);
    in_material.m_text_emissive.bind(GL_TEXTURE3);

    //  Mask Texture

    setBool("mat.maskMap.exist", in_material.m_text_dissolve.getTextureID() != 0);
    setFloat3("mat.maskMap.offset", in_material.m_text_dissolve.m_offset);
    setFloat3("mat.maskMap.tiling", in_material.m_text_dissolve.m_tiling);
    in_material.m_text_dissolve.bind(GL_TEXTURE4);

}



void Resources::loadShader(std::string shaderName)
{
    std::string filePath = "Resource/Shader/" + shaderName + ".shad";
    Resources::ResourcesManager* resources = Resources::ResourcesManager::instance();

    if (resources->fileLoaded(filePath)) return;

    Core::Log* _log = Core::Log::instance();

    //	Open file in read mode
    //	----------------------
    std::ifstream file(filePath);

    //	If failed return
    //	----------------
    if (!file)
    {
        _log->writeError("Failed to open file \"" + filePath + "\".");
        return;
    }

    std::string curr_line;

    std::string vertex_shader = "";
    std::string fragment_shader = "";
    std::string geometryPath = "";

    int shaderType = 0;
    while (std::getline(file, curr_line))
    {

        std::istringstream lineStream(curr_line);

        std::string type = FileParser::getString(lineStream);
        if (type == "TYPE")
        {
            std::string shaderTypeStr = FileParser::getString(lineStream);
            if (shaderTypeStr == "SHADER_3D")
            {
                shaderType = Resources::ShaderType::SHADER_3D;
            }
            else if (shaderTypeStr == "SPRITE_2D")
            {
                shaderType = Resources::ShaderType::SPRITE_2D;
            }
            else if (shaderTypeStr == "POST_PROCESS")
            {
                shaderType = Resources::ShaderType::POST_PROCESS;
            }
            else if (shaderTypeStr == "CUBE_MAP")
            {
                shaderType = Resources::ShaderType::CUBE_MAP;
            }
        }
        else if (type == "VERT")
        {
            vertex_shader = FileParser::getString(lineStream);
        }
        else if (type == "FRAG")
        {
            fragment_shader = FileParser::getString(lineStream);
        }
        else if (type == "GEOM")
        {
            geometryPath = FileParser::getString(lineStream);
        }
    }

    if (fragment_shader == "" || vertex_shader == "") return;
    
    const char* vertexPath = vertex_shader.c_str();
    const char* fragmentPath = fragment_shader.c_str();

    _log->write("+ Loading Shaders \"" + filePath + "\"");

    resources->m_shaderName_shader[shaderName] = Resources::Shader(vertexPath, fragmentPath, geometryPath);
    resources->m_shaderName_shader[shaderName].m_type = shaderType;

    file.close();

    _log->breakLine();
}