#pragma once

#include <Physics/RigidBody3.hpp>
#include <Engine/Component.hpp>
#include <Utils/Timer.hpp>
#include <LowRenderer/ParticleSystem.hpp>
#include <LowRenderer/Light.hpp>

class Weapon : public Component
{
protected:

	ParticleSystem* m_hitParticules = nullptr;
	Light* m_shootFlash = nullptr;

	int m_clipSize = 30;
	int m_clipAmmo = 30;
	
	int m_ammoReserve = 180;
	int m_ammoMaxCount = 180;
	
	int m_damages = 0;

	float m_cooldownTime = 0.1f;
	float m_range = 200.f;

	Timer m_cooldown = Timer(0.1f, true);

	Timer m_reloadCd = Timer(1.f, false);
	virtual void Shoot();

public:
	Weapon() = default;
	Weapon(GameObject* in_gameObject);

	void start() override;

	void update() override;
	void fixedUpdate() override {}
	void Reload();
	void PickupAmmo(int ammunitionAmount);
	//int getAmmoCount() const { return m_ammoCount; }

	void showImGUI() override;
	void saveComponentInSCNFile(std::ofstream& file) override;
	void loadComponentFromSCNFile(std::istringstream& lineStream) override;
};