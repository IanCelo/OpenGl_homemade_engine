
#include <LowRenderer/SpriteBillboard.h>

#include <Engine/Transform3.hpp>
#include <Engine/GameObject.hpp>

#include <Core/RendererManager.hpp>
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

SpriteBillboard::SpriteBillboard(GameObject* in_gameObject) : Component(in_gameObject)
{
    init(ComponentType::SpriteBillboard);
    initRenderData();

    int i = (int)m_gameObject->m_sceneReference->m_rendererManager.m_spriteBillboardList.size();
    m_gameObject->m_sceneReference->m_rendererManager.m_spriteBillboardList[i] = this;

    Resources::ResourcesManager* _resources = Resources::ResourcesManager::instance();

    Resources::loadShader("Particle");
    m_shader = &_resources->m_shaderName_shader["Particle"];
}


void SpriteBillboard::draw() 
{
	if (!m_shader || !m_material) return;

	m_shader->use();
    m_shader->setMaterial(*m_material);

	m_shader->setFloat4("ParticleColor", m_color);
	m_shader->setMat4("Model", m_transform->getTransformMatrix());

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindVertexArray(0);
}

void SpriteBillboard::initRenderData()
{
    // configure VAO/VBO
    unsigned int VBO;
    float vertices[] = {
        // pos      // tex
        -0.5f,  0.5f, 0.0f, 1.0f,
         0.5f, -0.5f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.0f, 0.0f,

       -0.5f,  0.5f, 0.0f, 1.0f,
         0.5f,  0.5f, 1.0f, 1.0f,
         0.5f, -0.5f, 1.0f, 0.0f
    };


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

void SpriteBillboard::destroy()
{
    //  get Renderer Manager of the scene
    Core::RendererManager* _renderer = &m_gameObject->m_sceneReference->m_rendererManager;

    //  Get last index (trash index)
    int lastIndex = (int)_renderer->m_spriteBillboardList.size() - 1;

    //  Parsing list of the current renderer component type
    for (auto comp : _renderer->m_spriteBillboardList)
    {
        //  Once we find "this", let's procees to the retirement of the component from the list
        if (comp.second == this)
        {
            //  Swap current with the last one if it's not already the last one
            if (comp.first != lastIndex)
            {
                _renderer->m_spriteBillboardList[comp.first] = _renderer->m_spriteBillboardList[lastIndex];
            }

            //  Set the pointer to null
            _renderer->m_spriteBillboardList[lastIndex] = nullptr;

            //  Then erase it from the list
            _renderer->m_spriteBillboardList.erase(lastIndex);

            //  And leave the loop
            //  To avoid errors and optimize
            break;
        }
    }

    //  Then call the common component destoy function
    Component::destroy();
}


void SpriteBillboard::saveComponentInSCNFile(std::ofstream& file)
{
    file << "SPRITEBILLBOARD\t";
    file << m_materialPath << " ";
    file << m_materialName << " ";
    FileWriter::writeVec4InFile(file, m_color);
    file << "\n";
}

void SpriteBillboard::loadComponentFromSCNFile(std::istringstream& lineStream)
{
    //	Write in log
    Core::Log* _log = Core::Log::instance();
    _log->write("+\t Adding Sprite to new gameObject");

    Resources::ResourcesManager* _resources = Resources::ResourcesManager::instance();
    using namespace FileParser;
    m_materialPath = getString(lineStream);
    Resources::stringList nonUsed;
    _resources->parseMTL(
        Extractor::ExtractDirectory(std::string(m_materialPath)),
        Extractor::ExtractFilename(std::string(m_materialPath)),
        nonUsed
    );
    m_materialName = getString(lineStream);
    m_material = &_resources->m_materialName_material[m_materialName];

    lineStream.ignore();
    m_color = FileParser::getVector4(lineStream);
}



void SpriteBillboard::showImGUI()
{
    ImGui::SliderFloat4("Color", &m_color.x, 0, 1.f);
    if (m_material) m_material->showImGui();

    Resources::ResourcesManager* resources = Resources::ResourcesManager::instance();

    const char* materialPreview = m_material ? m_materialName.c_str() : "none";

    if (ImGui::BeginCombo("Material", materialPreview))
    {
        for (auto& material : resources->m_materialName_material)
        {
            if (ImGui::Selectable(material.first.c_str(), material.first == materialPreview))
            {
                m_material = &material.second;
                m_materialName = material.first;
            }
        }
        ImGui::EndCombo();
    }
}