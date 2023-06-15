#pragma once

#include <vector>

#include <Engine/Component.hpp>
#include <Resources/Particle.hpp>

namespace Resources
{
	class Material;
	class Shader;
};

class GameObject;

enum class ParticleSystemPresets
{
	HIT,
	DEATH,
	VENT
};

class ParticleSystem : public Component
{
public:
	//	Constructor & Destructor
	//	------------------------

	ParticleSystem() = default;
	ParticleSystem(GameObject* in_gameObject);
	~ParticleSystem() {}

	//	Public Members
	//	-------------------------

	void draw();
	void update()    override;
	void showImGUI() override;
	void destroy() override;
	void emit();

	void saveComponentInSCNFile(std::ofstream& file) override;
	void loadComponentFromSCNFile(std::istringstream& lineStream) override;
	//	Getters And Setters
	//	-------------------------

	void setActive(bool state) { m_active = state; }

	void setConeDir(const Maths::Vector3f& dir) { m_coneDir = dir; }
	void play();

	void ApplyPreset(ParticleSystemPresets preset);
	Transform3 pos;

protected:
	//  Internal Variables
	//	-------------------------

	std::vector<Resources::Particle> particles;

	Resources::Shader*   m_shader   = nullptr;
	Resources::Material* m_material = nullptr;

	unsigned int VAO;

	std::string m_shaderName;
	std::string m_materialName;
	std::string m_materialPath;

	bool m_active = false;
	bool m_loop = false;
	Vector2f m_spawnrateRange { 1.f, 1.f }; //waiting time until new respawn in seconds
	
	Timer m_playTime;
	
	//  Particles Specs
	//	----------------

	Vector2f m_lifetimeRange  { 1.f, 1.f }; //in seconds
	Vector2f m_startSize      { 1.f, 1.f }; //scale unit
	Vector2f m_startSpeed     { 1.f, 1.f }; // meters/seconds
	Vector4f m_startColorMin  { 1.f,1.f,1.f,1.f }; // RGBA
	Vector4f m_startColorMax  { 1.f,1.f,1.f,1.f }; // RGBA
	Vector3f m_StartVelocity = { 0.f, 0.f, 0.f };
	
	float    m_gravityMultiplier = 0.f;

	// Cone Specs
	// ------------
	bool isCone = true;
	float Conesize = 1.f;
	float spawnCircleRadius = 1.f;
	float endCircleRadius   = 3.f;
	Vector3f m_coneDir = {0.f,1.f,0.f};

	int  m_maxParticles = 10;
	bool m_playOnBirth = true;
	bool m_useLights = true;

	void initGL();
	void destroyParticle(int i);

	Timer timer;
};