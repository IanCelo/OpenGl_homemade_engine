#pragma once
#include <Maths/Vector4.h>
#include <Engine/Component.hpp>
#include <Resources/Texture.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Resources
{
    class Shader;
}

class Sprite : public Component
{
public:
    Sprite(GameObject* in_gameObject);
    Sprite(const char* path, const char* name);
    void draw();
    void showImGUI() override;
    void destroy() override;
    void saveComponentInSCNFile(std::ofstream& file) override;
    void loadComponentFromSCNFile(std::istringstream& lineStream) override;
    void updateRenderData();

    //  Public Variables
    //  ----------------

    Maths::Vector3f m_color = Vector3f::one();

private:

    //  Private Variables
    //  -----------------

    Resources::Shader* m_shader;
    Resources::Texture* m_texture;

    Maths::Vector3f m_default_color;

    unsigned int VAO;

    void initRenderData();
};