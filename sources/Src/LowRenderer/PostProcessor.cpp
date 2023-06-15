#include <LowRenderer/PostProcessor.hpp>
#include <Resources/ResourcesManager.hpp>

#include <Core/Window.hpp>
#include <Core/Log.hpp>
#include <Core/InputsManager.hpp>
#include <Core/TimeManager.h>

#include <iostream>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

PostProcessor::~PostProcessor()
{
    glDeleteRenderbuffers(1, &RBO);
    glDeleteFramebuffers(1, &FBO);
}

PostProcessor::PostProcessor()
{
    Resources::ResourcesManager* _resources = Resources::ResourcesManager::instance();
    Core::Window* _window = Core::Window::instance();
    //Core::InputsManager* _inputs = Core::InputsManager::instance();
    Core::Log* _log = Core::Log::instance();

    Resources::loadShader("PostProcess");
    m_shader = &_resources->m_shaderName_shader["PostProcess"];

    Resources::loadShader("GaussianBlur");
    m_gaussianShader = &_resources->m_shaderName_shader["GaussianBlur"];

    //_inputs->getWindowSize(m_width, m_height);

    //  Generate Frame Buffer Object
    //  ----------------------------

    glGenFramebuffers(1, &FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    //  Generate Texture
    //  ----------------

    glActiveTexture(GL_TEXTURE0);
    glGenTextures(2, m_texture);

    for (unsigned int i = 0; i < 2; i++)
    {
        glBindTexture(GL_TEXTURE_2D, m_texture[i]);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _window->m_width, _window->m_height, 0, GL_RGBA, GL_FLOAT, NULL);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_texture[i], 0);
    }

    //  Generate Render Buffer Object
    //  -----------------------------

    glGenRenderbuffers(1, &RBO);
    glBindRenderbuffer(GL_RENDERBUFFER, RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, _window->m_width, _window->m_height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, RBO);


    unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
    glDrawBuffers(2, attachments);

    if ((glCheckFramebufferStatus(GL_FRAMEBUFFER)) != GL_FRAMEBUFFER_COMPLETE)
    {
        _log->writeError("POSTPROCESSOR -> Incomplete frameBuffer");
    }

   glGenFramebuffers(2, pingpongFBO);
   glGenTextures(2, m_pingPongTexture);
   for (unsigned int i = 0; i < 2; i++)
   {
       glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);
       glBindTexture(GL_TEXTURE_2D, m_pingPongTexture[i]);
       glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, _window->m_width, _window->m_height, 0, GL_RGBA, GL_FLOAT, NULL);

       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

       glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_pingPongTexture[i], 0 );
   }

   glBindFramebuffer(GL_FRAMEBUFFER, 0);


   m_shader->use();
   m_shader->setInt("Scene", 0);
   m_shader->setInt("BloomBlur", 1);
   m_shader->setFloat("Exposure", m_exposure);
   m_shader->setFloat("Gamma", m_gamma);
   m_shader->setBool("Bloom", m_bloom);


   m_gaussianShader->use();
   m_gaussianShader->setInt("Image", 0);
   
   initRenderData();
}


void PostProcessor::initRenderData()
{
    // configure VAO/VBO
    unsigned int VBO;
    float vertices[] = {
        // pos        // tex
        -1.0f, -1.0f, 0.0f, 0.0f,
         1.0f,  1.0f, 1.0f, 1.0f,
        -1.0f,  1.0f, 0.0f, 1.0f,

        -1.0f, -1.0f, 0.0f, 0.0f,
         1.0f, -1.0f, 1.0f, 0.0f,
         1.0f,  1.0f, 1.0f, 1.0f
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(VAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void PostProcessor::reshape()
{
    //Core::InputsManager* _inputs = Core::InputsManager::instance();
    Core::Window* _window = Core::Window::instance();

    /*int oldWidth  = m_width;
    int oldHeight = m_height;
    _inputs->getWindowSize(m_width, m_height);*/

    //if (oldWidth == m_width && oldHeight == m_height) return;

    if (_window->m_wasReshaped == false) return;

    for (unsigned int i = 0; i < 2; i++)
    {
        glBindTexture(GL_TEXTURE_2D, m_texture[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _window->m_width, _window->m_height, 0, GL_RGBA, GL_FLOAT, NULL);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    glBindRenderbuffer(GL_RENDERBUFFER, RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, _window->m_width, _window->m_height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    for (unsigned int i = 0; i < 2; i++)
    {

        glBindTexture(GL_TEXTURE_2D, m_pingPongTexture[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _window->m_width, _window->m_height, 0, GL_RGBA, GL_FLOAT, NULL);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

}

void PostProcessor::beginRender()
{
    reshape();
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glClearColor(0, 0,0,0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void PostProcessor::endRender()
{
    //  Back to default
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    if (m_bloom)
    {
        bool horizontal = true;
        m_gaussianShader->use();
    
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_texture[1]);
        glBindVertexArray(VAO);

        //  Loop to add bloom effects
        for (int i = 0; i < m_bloomAmount; i++)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[horizontal]);
            m_gaussianShader->setBool("Horizontal", horizontal);

            glDrawArrays(GL_TRIANGLES, 0, 6);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, m_pingPongTexture[horizontal]);

            horizontal = !horizontal;
        }
        glBindVertexArray(0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /*
    * DEBUGING
    * 
    
    if (ImGui::Begin("DEBUG"))
    {
        ImGui::Image((void*)(intptr_t)m_texture[0], ImVec2(256, 256), ImVec2(0, 1), ImVec2(1, 0));

        ImGui::SameLine();
        ImGui::Image((void*)(intptr_t)m_texture[1], ImVec2(256, 256), ImVec2(0, 1), ImVec2(1, 0));

        ImGui::SameLine();
        ImGui::Image((void*)(intptr_t)m_pingPongTexture[0], ImVec2(256, 256), ImVec2(0, 1), ImVec2(1, 0));

        ImGui::SameLine();
        ImGui::Image((void*)(intptr_t)m_pingPongTexture[1], ImVec2(256, 256), ImVec2(0, 1), ImVec2(1, 0));
    }
    ImGui::End();
    */
}




void PostProcessor::render()
{
    m_shader->use();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texture[0]);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_pingPongTexture[1]);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}



void PostProcessor::showImGui()
{
    if (ImGui::Checkbox("Bloom", &m_bloom))
    {
        m_shader->use();
        m_shader->setBool("Bloom", m_bloom);
    }

    if (m_bloom)
    {
        bool changed = false;

        changed  = ImGui::SliderFloat("Exposure", &m_exposure, 0.f, 1.f);
        changed |= ImGui::SliderFloat("Gamma", &m_gamma, 0.f, 5.f);

        ImGui::SliderInt("Bloom Amount", &m_bloomAmount, 1, 50);

        if (changed)
        {
            m_shader->use();

            m_shader->setFloat("Exposure", m_exposure);
            m_shader->setFloat("Gamma", m_gamma);
        }
    }
}