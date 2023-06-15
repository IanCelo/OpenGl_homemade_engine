#pragma once

#include <memory>

#include <Resources/Mesh.hpp>
#include <Resources/Material.hpp>

#include <Engine/Component.hpp>

class Model : public Component
{
private: 
	//	DEBUG
	bool debugNormal = false;
public:
	//	Constructors
	//	------------

	Model() = default;
	Model(GameObject* in_gameObject);
	~Model();

	//	Public Internal Functions
	//	-------------------------

	void draw();
	void showImGUI() override;
	void destroy() override;

	void saveComponentInSCNFile(std::ofstream& file) override;
	void loadComponentFromSCNFile(std::istringstream& lineStream) override;
	void loadModel(const std::string& modelName, const std::string& matName, const std::string& shaderName, const std::string& fullPath);
	void loadModel(const std::string& modelName, const std::string& shaderName, const std::string& fullPath);


	//	Public Internal Variables
	//	-------------------------

	Resources::Mesh*		m_mesh = nullptr;
	Resources::Material*	m_material = nullptr;
	Resources::Material		m_materialInstance;
	Resources::Shader*		m_shader = nullptr;

	//std::string m_path;

	std::string m_path;
	std::string m_shaderName;
	std::string m_meshName;
	std::string m_materialName;

};

