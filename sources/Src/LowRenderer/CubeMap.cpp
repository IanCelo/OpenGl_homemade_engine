#include <LowRenderer/CubeMap.hpp>
#include <Resources/ResourcesManager.hpp>
#include <Utils/File.h>
#include <Utils/StringExtractor.h>


LowRenderer::CubeMap::CubeMap(std::string path)
{

    Resources::ResourcesManager* _resources = Resources::ResourcesManager::instance();

    std::string name = Extractor::ExtractNameWithoutExtension(Extractor::ExtractFilename(path));
    m_texture = _resources->getCubeMapTexture(path, name);

    Resources::loadShader("CubeMap");
    m_shader = &_resources->m_shaderName_shader["CubeMap"];
    m_shader->use();
    m_shader->setInt("Skybox", 0);

    initRenderData();
}

void LowRenderer::CubeMap::initRenderData()
{
    float skyboxVertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    unsigned int VBO;

    //	Create VAO - Vertex Array Object
    glGenVertexArrays(1, &VAO);

    //	Create VBO - Vertex Buffer Object
    glGenBuffers(1, &VBO);

    //	Define VBO and VAO
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    //	Attach VBO to VAO
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);

    //	Position & texCoord
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}


void LowRenderer::CubeMap::draw()
{
    if (m_shader && m_texture)
    {
        glDepthMask(GL_FALSE);
        m_shader->use();

        glBindVertexArray(VAO);

        glActiveTexture(0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture->getID());

        glDrawArrays(GL_TRIANGLES, 0, 36);
        glDepthMask(GL_TRUE);

    }
}