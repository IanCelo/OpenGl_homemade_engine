#pragma once

#include <Maths/Vector3.h>

#include <Engine/Component.hpp>

//	Light types
enum class LightType
{
	POINT_LIGHT,			// 0
	DIRECTIONNAL_LIGHT,		// 1
	SPOT_LIGHT,				// 2
};

class Light : public Component
{
public:

	Light() = default;
	~Light() = default;
	Light(GameObject* in_gameObject);

	//Maths::Vector3f position = { 0.f,0.f,0.f};

	//	Colors

	Maths::Vector3f ambient = { 0.15f, 0.15f, 0.15f };
	Maths::Vector3f diffuse = { 0.5f, 0.5f, 0.4f };
	Maths::Vector3f specular = { 0.5f, 0.5f, 0.4f };

	Maths::Vector3f attenuation = { 1.f, 1.f, 0.1f };
	Maths::Vector3f direction = { 0.f, -1.f, 0.f };

	float power  = 0.f;

	//	Spot light values

	float cutOff = .9f;
	float outerCutOff = .85f;

	int lightType = (int)LightType::POINT_LIGHT;

	//	Show ImGui Light window
	//	Parameters : none
	//	-----------------
	void showImGUI() override;
	void destroy() override;

	void saveComponentInSCNFile(std::ofstream& file) override;
	void loadComponentFromSCNFile(std::istringstream& lineStream) override;
};

