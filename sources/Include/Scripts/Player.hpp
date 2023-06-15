#pragma once

#include <Maths/Matrix.h>
#include <Maths/Utils.h>

#include <Physics/Rigidbody3.hpp>
using namespace Physics;
#include <Engine/Component.hpp>
#include <Utils/Timer.hpp>

#define DISPLACEMENT_SPEED 100.f;
#define JUMP_SPEED 50.f;

class Player : public Component
{
private:

    //  Private Internal Variables
    //  --------------------------

    Rigidbody3* m_rigidbody = nullptr;
    Vector3f m_spawnPoint = Vector3f::zero();
    Timer m_invulnerableFrame = Timer(1.f, true);

    bool m_movementLocked = true;

public:

    //  Constructor
    //  -----------

    Player() = default;
    ~Player() = default;
    Player(GameObject* in_gameObject);

    //  Functions
    //  ---------

    void start() override;
    void update() override;
    void fixedUpdate() override;
    void lateUpdate() override {};

    void updatePosition();

    void showImGUI() override;
    void saveComponentInSCNFile(std::ofstream & file) override;
    void loadComponentFromSCNFile(std::istringstream& lineStream) override;

    void heal() {};
    void damage(int damages);
    void lose();

    // Datas
    int m_playerLife = 100;
    int m_playerMaxLife = 100;
    GameObject* m_weapon = nullptr;

    // TODO : add pointer on weapon (create classs weapon and inheritance, etc...)

    bool m_isGrounded = false;
    bool m_isColliding = false;

    // Movement
    Vector3f m_playerDirection = Vector3f::zero();
    float displacement_speed = DISPLACEMENT_SPEED;
    float jump_speed = JUMP_SPEED;
};