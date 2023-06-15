#pragma once

#include <string>
#include <vector>

#include <fstream>
#include <sstream>

#include <iostream>
#include <unordered_map>

#include <Maths/Matrix.h>

#include <Resources/Material.hpp>

class Light;

namespace Resources
{
    enum ShaderType
    {
        SHADER_3D,
        SPRITE_2D,
        CUBE_MAP,
        POST_PROCESS,
    };

    class Shader
    {
    public:
        //  Constructors
        //  ------------

        Shader() = default;
        Shader(const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath);

        //  Public Internal Variables
        //  -------------------------

        unsigned int ID = 0;
        int m_type = 0;

        //  Public Internal Functions
        //  -------------------------

        //  Use/Activate the shader
        //  Parameters : none
        //  -----------------
        void use();

        //  Send a Boolean value to the shader
        //  Parameteters : const string& name, const bool value
        //  --------------------------------------------------------
        void setBool(const std::string& name, const bool value) const;

        //  Send a Integer value to the shader
        //  Parameteters : const string& name, const int value
        //  -------------------------------------------------------
        void setInt(const std::string& name, const int value) const;

        //  Send a Floating value to the shader
        //  Parameteters : const string& name, const float value
        //  ---------------------------------------------------------
        void setFloat(const std::string& name, const float value) const;

        //  Send a Vector2 value to the shader
        //  Parameteters : const string& name, const Vector2 value
        //  ---------------------------------------------------------
        void setFloat2(const std::string& name, const Maths::Vector2f& value) const;

        //  Send a Vector3 to the shader
        //  Parameteters : const string& name, const Vector3 value
        //  -----------------------------------------------------------
        void setFloat3(const std::string& name, const Maths::Vector3f& value) const;

        //  Send a Vector4 to the shader
        //  Parameteters : const string& name, const Vector4 value
        //  -----------------------------------------------------------
        void setFloat4(const std::string& name, const Maths::Vector4f& value) const;

        //  Send a Matrix 4x4 to the shader
        //  Parameteters : const string& name, const Mat4 value
        //  ---------------------------------------------------
        void setMat4(const std::string& name, const Maths::Mat4x4& value) const;

        //  Send Light list to the shader
        //  Parameteters : const vector<Light>& list
        //  ----------------------------------------
        void setLights(const std::unordered_map<int, const Light*>& list) const;

        //  Send Material to the shader
        //  Parameters : const Material* in_material
        //  ----------------------------------------
        void setMaterial(const Resources::Material& in_material) const;
    };

    void loadShader(std::string shaderName);
}