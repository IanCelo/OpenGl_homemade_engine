#include<LowRenderer/Text.hpp>
#include <Resources/ResourcesManager.hpp>
#include <Core/Window.hpp>

TextRender::TextRender()
{
	Resources::ResourcesManager* _resources = Resources::ResourcesManager::instance();
	Resources::loadShader("Text");

	m_shader = &_resources->m_shaderName_shader["Text"];
    m_shader->setInt("Text", 0);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}


void TextRender::RenderText(const TextParameter& render)
{
    float windowCoef = Core::Window::instance()->m_windowCoef;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Resources::ResourcesManager* _resources = Resources::ResourcesManager::instance();

    //  If font not found load it
    if (_resources->m_characterListPerFonts.find(render.font) == _resources->m_characterListPerFonts.end())
    {
        _resources->loadFont(render.font);
    }

    //  Activate corresponding render state	
    m_shader->use();
    m_shader->setFloat3("TextColor", render.color);

    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    //  Iterate through all characters

    std::string::const_iterator c;

    float x = render.pos.x;

    for (c = render.text.begin(); c != render.text.end(); c++)
    {
        Resources::Character ch = _resources->m_characterListPerFonts[render.font][*c];

        float xpos = x + ch.Bearing.x * render.size;
        float ypos = render.pos.y - (ch.Size.y - ch.Bearing.y) * render.size;

        float w = ch.Size.x * render.size * windowCoef;
        float h = ch.Size.y * render.size;

        //  Update VBO for each character

        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }
        };

        //  Render glyph texture over quad

        glBindTexture(GL_TEXTURE_2D, ch.TextureID);

        //  Update content of VBO memory

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        //  Render quad

        glDrawArrays(GL_TRIANGLES, 0, 6);

        //  Now advance cursors for next glyph (note that advance is number of 1/64 pixels)

        x += (ch.Advance >> 6) * render.size; //    Bitshift by 6 to get value in pixels (2^6 = 64)
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glDisable(GL_BLEND);
}

void TextRender::AddText(const std::string& font, const std::string& text, const Maths::Vector2f& pos, float scale, const Maths::Vector3f& color)
{
    m_textBuffer.push_back(
        {
            font,
            text,
            pos,
            scale / 100.f,
            color
        }
    );
}

void TextRender::RenderTextBuffer()
{
    for (TextParameter text : m_textBuffer)
    {
        RenderText(text);
    }

    m_textBuffer.clear();
}