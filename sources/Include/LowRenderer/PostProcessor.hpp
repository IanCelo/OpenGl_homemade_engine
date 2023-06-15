#pragma once

#ifndef POST_PROCESSOR_H
#define POST_PROCESSOR_H

#include <glad/glad.h>

#include <Resources/Texture.hpp>
//#include <Sprite.h"
#include <Resources/Shader.hpp>

class PostProcessor
{
public:
    
    //  Public Internal Variables
    //  -------------------------

    Resources::Shader* m_shader;
    Resources::Shader* m_gaussianShader;

    GLuint m_texture[2];
    GLuint m_pingPongTexture[2];

    //  Constructor
    //  -----------
    ~PostProcessor();
    PostProcessor();

    //  Public Internal Variables
    //  -------------------------

    //  Prepares the postprocessor's framebuffer operations before rendering the game
    void beginRender();

    //  Should be called after rendering the game, so it stores all the rendered data into a texture object
    void endRender();

    //  Renders the PostProcessor texture quad (as a screen-encompassing large sprite)
    void render();

    //  Show ImGui window parameters
    void showImGui();

private:

    GLuint FBO; // FrameBufferObject
    GLuint RBO; // RenderBufferObject
    GLuint VAO; // VertexArrayObject

    GLuint pingpongFBO[2];

    float m_exposure = .80f;
    float m_gamma = .90f;
    int m_bloomAmount = 10;

    bool m_bloom = false;

    //  Initialize quad for rendering postprocessing texture
    void initRenderData();

    //  Reshape the texture size if screen size is modified
    void reshape();
};

#endif