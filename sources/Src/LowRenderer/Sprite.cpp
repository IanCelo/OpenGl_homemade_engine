#include <LowRenderer/Sprite.hpp>

#include <Engine/Transform3.hpp>
#include <Engine/GameObject.hpp>

#include <Core/RendererManager.hpp>
#include <Core/Window.hpp>
#include <Core/Log.hpp>

#include <Resources/Shader.hpp>
#include <Resources/ResourcesManager.hpp>
#include <Resources/Scene.hpp>
#include <Maths/Matrix.h>

#include <Utils/File.h>
#include <Utils/StringExtractor.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

Sprite::Sprite(GameObject* in_gameObject) : Component(in_gameObject)
{
    init(ComponentType::Sprite);
    initRenderData();

    int i = (int)m_gameObject->m_sceneReference->m_rendererManager.m_spriteList.size();
    m_gameObject->m_sceneReference->m_rendererManager.m_spriteList[i] = this;

    m_texture = 0; 

    Resources::ResourcesManager* _resources = Resources::ResourcesManager::instance();

    Resources::loadShader("Sprite");
    m_shader = &_resources->m_shaderName_shader["Sprite"];

    m_shader->use();
    m_shader->setInt("Image", 0);
}

Sprite::Sprite(const char* path, const char* name) 
{
    initRenderData();

    Resources::ResourcesManager* _resources = Resources::ResourcesManager::instance();
    m_texture = 0;
    Resources::loadShader("Sprite");
    m_shader = &_resources->m_shaderName_shader["Sprite"];

    m_shader->use();
    m_shader->setInt("Image", 0);

    m_texture = _resources->loadTexture(path, name);

    m_default_color = m_color = Vector3f::one();
}

void Sprite::initRenderData()
{
    float wCoef = Core::Window::instance()->m_windowCoef * .5f;

    float vertices[] = {
        // pos      // tex
        -wCoef,  0.5f, 0.0f, 1.0f,
         wCoef, -0.5f, 1.0f, 0.0f,
        -wCoef, -0.5f, 0.0f, 0.0f,

        -wCoef,  0.5f, 0.0f, 1.0f,
         wCoef,  0.5f, 1.0f, 1.0f,
         wCoef, -0.5f, 1.0f, 0.0f
    };

    // configure VAO/VBO
    unsigned int VBO;

    //	Create VAO - Vertex Array Object
    glGenVertexArrays(1, &VAO);

    //	Create VBO - Vertex Buffer Object
    glGenBuffers(1, &VBO);

    //	Define VBO and VAO
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    //	Attach VBO to VAO / Bind attributes (texCoord) in VAO
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //	Position & texCoord
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}


void Sprite::updateRenderData()
{
    Core::Window* _window = Core::Window::instance();

    //  if window wasn't reshaped exit
    if (_window->m_wasReshaped == false) return;

    float wCoef = _window->m_windowCoef * .5f;

    float vertices[] = {
        // pos      // tex
        -wCoef,  0.5f, 0.0f, 1.0f,
         wCoef, -0.5f, 1.0f, 0.0f,
        -wCoef, -0.5f, 0.0f, 0.0f,

        -wCoef,  0.5f, 0.0f, 1.0f,
         wCoef,  0.5f, 1.0f, 1.0f,
         wCoef, -0.5f, 1.0f, 0.0f
    };

    // configure VAO/VBO
    unsigned int VBO;

    //	Create VBO - Vertex Buffer Object
    glGenBuffers(1, &VBO);

    //	Define VBO and VAO
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    //	Attach VBO to VAO / Bind attributes (texCoord) in VAO
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //	Position & texCoord
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Sprite::draw()
{
    if (m_shader && m_texture)
    {
        updateRenderData();
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);

        m_shader->use();

        m_shader->setMat4("Model", m_transform->getTransformMatrix());
        m_shader->setFloat3("SpriteColor", m_color);
        m_color = m_default_color;

        m_texture->bind(GL_TEXTURE0);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        glDisable(GL_BLEND);
    }
}

void Sprite::destroy()
{
    //  get Renderer Manager of the scene
    Core::RendererManager* _renderer = &m_gameObject->m_sceneReference->m_rendererManager;
    
    //  Get last index (trash index)
    int lastIndex = (int)_renderer->m_spriteList.size() - 1;

    //  Parsing list of the current renderer component type
    for (auto comp : _renderer->m_spriteList)
    {
        //  Once we find "this", let's procees to the retirement of the component from the list
        if (comp.second == this)
        {
            //  Swap current with the last one if it's not already the last one
            if (comp.first != lastIndex)
            {
                _renderer->m_spriteList[comp.first] = _renderer->m_spriteList[lastIndex];
            }

            //  Set the pointer to null
            _renderer->m_spriteList[lastIndex] = nullptr;

            //  Then erase it from the list
            _renderer->m_spriteList.erase(lastIndex);

            //  And leave the loop
            //  To avoid errors and optimize
            break;
        }
    }

    //  Then call the common component destoy function
    Component::destroy();
}


void Sprite::saveComponentInSCNFile(std::ofstream& file)
{
    file << "SPRITE\t\t" << m_texture->m_path << m_texture->m_name << ".png" << " ";
    FileWriter::writeVec3InFile(file, m_color);
    file << "\n";
}

void Sprite::loadComponentFromSCNFile(std::istringstream& lineStream)
{
    //	Write in log
    Core::Log* _log = Core::Log::instance();
    _log->write("+\t Adding Sprite to new gameObject");

    Resources::ResourcesManager* _resources = Resources::ResourcesManager::instance();
    
    std::string path = FileParser::getString(lineStream);
    std::string name = Extractor::ExtractNameWithoutExtension(Extractor::ExtractFilename(path));

    m_texture = _resources->loadTexture(path, name);

    lineStream.ignore();
    m_default_color = m_color = FileParser::getVector3(lineStream);
}



void Sprite::showImGUI()
{
    if (ImGui::SliderFloat3("Color", &m_color.x, 0, 1.f)) m_default_color = m_color;
    if (m_texture) m_texture->showImGui();

    std::string textureName = m_texture ? m_texture->m_name : "none";

    if (ImGui::BeginCombo("Texture", textureName.c_str()))
    {
        Resources::ResourcesManager* _resources = Resources::ResourcesManager::instance();

        for (auto& textName : _resources->m_textureName_texture)
        {
            if (ImGui::Selectable(textName.first.c_str()))
            {
                m_texture = &_resources->m_textureName_texture[textName.first];
            }
        }
        ImGui::EndCombo();
    }
}