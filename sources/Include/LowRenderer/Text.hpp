#pragma once

#include "Resources/Shader.hpp"
#include "Maths/Vector3.h"
#include "Utils/Singleton.h"
#include <vector>




class TextRender : public Singleton<TextRender>
{
private :

	//	Private structure
	//	-----------------

	struct TextParameter
	{
		std::string font;
		std::string text;
		Maths::Vector2f pos;
		float size;
		Maths::Vector3f color;
	};

	//	Private Variables
	//	-----------------

	Resources::Shader* m_shader;
	unsigned int VAO, VBO;
	std::vector<TextParameter> m_textBuffer;

	//	Private functions
	//	-----------------

	void RenderText(const TextParameter& render);

public:

	//	Constructor
	//	-----------

	TextRender();

	//	Public Functions
	//	----------------
	void AddText(const std::string& font, const std::string& text, const Maths::Vector2f& pos, float scale, const Maths::Vector3f& color);
	void RenderTextBuffer();
};

