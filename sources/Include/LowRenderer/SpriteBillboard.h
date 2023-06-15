#pragma once

#include <Engine/Component.hpp>

namespace Resources 
{
	class Material;
	class Shader;
}

class SpriteBillboard : public Component
{
public:
	SpriteBillboard() = default;
	SpriteBillboard(GameObject* in_gameObject);

	void draw();
	void showImGUI() override;
	void destroy() override;
	void saveComponentInSCNFile(std::ofstream& file) override;
	void loadComponentFromSCNFile(std::istringstream& lineStream) override;

private:
	Resources::Material* m_material;
	Resources::Shader*  m_shader;

	std::string m_materialName;
	std::string m_materialPath;

	unsigned int VAO;

	Maths::Vector4f m_color = { 1.f, 1.f, 1.f, 1.f };

	void initRenderData();
};