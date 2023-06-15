#pragma once

#include <Engine/Component.hpp>
#include <Physics/RigidBody3.hpp>
#include <Utils/Timer.hpp>
#include <LowRenderer/ParticleSystem.hpp>


class Player;

class Enemy : public Component
{
private:
	Rigidbody3* m_rigidbody = nullptr;
	Player* m_player = nullptr;
	ParticleSystem* m_hitParts = nullptr;
	Timer m_attackCooldown = Timer(1.f, true);
	int m_damages = 15;
	int m_life = 25;

	float m_speed = 100.f;
	float m_detectionRange = 100.f;
	float m_damageRange = 1.f;
	bool m_angry = false;

	Vector3f m_directionToPlayer = Vector3f::zero();
	float m_distanceToPlayer = 0.f;

	int m_indexInGameList = 0;

public:
	Enemy() = default;
	Enemy(GameObject* in_gameObject);

	//  Functions
	//  ---------

	void start() override;
	void update() override;
	void fixedUpdate() override;

	void takeDamage(int damages);

	void showImGUI() override;
	void saveComponentInSCNFile(std::ofstream& file) override;
	void loadComponentFromSCNFile(std::istringstream& lineStream) override;
};
