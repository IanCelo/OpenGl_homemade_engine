#include <Scripts/Player.hpp>

#include <Core/InputsManager.hpp>
#include <Core/GameManager.hpp>
#include <Core/TimeManager.h>
#include <Core/Graph.hpp>
#include <Core/Log.hpp>
#include <LowRenderer/Text.hpp>

#include <Config.hpp>

#include <Engine/GameObject.hpp>
#include <Engine/Transform3.hpp>
#include <Physics/PhysicsManager.hpp>

#include <Maths/Utils.h>

#include <imgui.h>

#include <Physics/Collider3.hpp>

#include <sstream>
#include <iomanip>


Player::Player(GameObject* in_gameObject) : Component(in_gameObject)
{
    init(ComponentType::Player);
}

void Player::start()
{
    m_rigidbody = m_gameObject->getComponent<Rigidbody3>();
    m_spawnPoint = m_gameObject->m_transform->m_position;
}

void Player::update()
{
    //  Write player health point in HUD
    {
        //  Life percentage for text color
        float percent = (float)m_playerLife / (float)m_playerMaxLife;

        //  Convert life integer value to string with '0' padding of 3 (0XX)
        std::stringstream ss;
        ss << std::setw(3) << std::setfill('0') << m_playerLife;

        //  Set color (Cyan to Red)
        float r = .5f + (1.f - percent) * .5f;
        float gb = percent;

        //  Stock text in text render buffer
        TextRender::instance()->AddText("abnes.ttf", ss.str() + " HP", { -8.5f, -8.25f }, .5f, { r,gb,gb });
    }

    //  Check if player is grounded
    RaycastHit groundHit;
    m_isGrounded = Core::Graph::instance()->getCurrentScene()->m_physicsManager.raycast
    (
        m_transform->m_position, 
        { 0, -1, 0 }, 
        ((SphereCollider3D*)(m_rigidbody->m_collider))->collider.m_radius + 0.3f,
        groundHit
    );

    //  Update player transform
    if (m_rigidbody != nullptr) updatePosition();

    //  Update invulnerable timer 
    m_invulnerableFrame.update();

    //  if plqyer life is 0, the player lost
    if (m_playerLife <= 0) lose();
}


void Player::updatePosition()
{
    m_rigidbody->m_gravityMultiplier = 5.0f;

    Core::TimeManager* _time = Core::TimeManager::instance();
    Core::InputsManager* _inputs = Core::InputsManager::instance();

    //  Update angle in function of the new mouse position
    m_transform->m_rotation.y -= _inputs->m_mouseDeltaX * MOUSE_SENSIVITY * _time->deltaTime;

    //  Initialize movements coefficients
    float forward_movement = 0.f;
    float strafe_movement = 0.f;

    //  Forward movements
    if (_inputs->m_key[FORWARD_INPUT]->down) forward_movement = -1;

    if (_inputs->m_key[BACKWARD_INPUT]->down) forward_movement = 1;

    //  Strafe movements
    if (_inputs->m_key[RIGHT_INPUT]->down) strafe_movement = -1;

    if (_inputs->m_key[LEFT_INPUT]->down) strafe_movement = 1;

    //  Update movements

    if (!m_isGrounded)
    {
        forward_movement *= 0.4f;
        strafe_movement *= 0.4f;
    }

    //  Update Y rotations
    float yaw = -m_transform->m_rotation.y;
    m_playerDirection.x = -(forward_movement * sinf(yaw) + strafe_movement * cosf(yaw));
    m_playerDirection.z = (forward_movement * cosf(yaw) - strafe_movement * sinf(yaw));

    //  Jumnp
    if (m_isGrounded && _inputs->m_key[UP_INPUT]->pressed)
    {
        m_rigidbody->addForce({ 0.f, jump_speed * EARTH_GRAVITY, 0.f }, Physics::ForceMode::Impulse);
    }

    //  Replace the player to spawn point when it falls under -MIN_HEIGHT in y axis
    
    float minHeight = MIN_HEIGHT;

    if (m_transform->m_position.y < -minHeight)
    {
        m_transform->m_position = Vector3f(0.f, 5.f, 0.f);
        m_rigidbody->m_velocity = { 0 };
    }
}

void Player::damage(int damages)
{
    //  If vulnerable
    if (m_invulnerableFrame.ended())
    {
    //	Play Sound
        Core::Graph::instance()->AudioPlayer->play2D("Assets/audio/player_hit.wav", false);
        
        //  Player takes damage
        m_playerLife = Maths::max(m_playerLife - damages,0);

        //  Reset invulnerable frame
        m_invulnerableFrame.restart();
    }
}

void Player::lose()
{
    Core::GameManager::instance()->WinOrLoseGame(false);
}

void Player::fixedUpdate()
{
    Core::TimeManager* _time = Core::TimeManager::instance();

    //  Update player velocity
    m_rigidbody->addForce(m_playerDirection.normalized() * displacement_speed, Physics::ForceMode::Acceleration);

    m_playerDirection = Vector3f::zero();
}

void Player::saveComponentInSCNFile(std::ofstream& file)
{
    file << "PLAYER\t\t";
    
    FileWriter::writeVec3InFile(file, m_spawnPoint); file << "/";

    file << m_playerMaxLife << " ";
    file << m_playerLife;
    file << "/";

    file << displacement_speed << " ";
    file << jump_speed;
    file << "\n";
}

void Player::showImGUI()
{
    using namespace ImGui;
    SliderInt("Life", &m_playerLife, 0, m_playerMaxLife);
    DragInt("Max life", &m_playerMaxLife);

    DragFloat("Speed", &displacement_speed);
    DragFloat("Jump height", &jump_speed);
}

void Player::loadComponentFromSCNFile(std::istringstream& lineStream)
{
    //	Write in log
    Core::Log* _log = Core::Log::instance();
    _log->write("+\t Adding player script to new gameObject");

    m_spawnPoint = FileParser::getVector3(lineStream);
    lineStream.ignore();

    m_playerMaxLife = FileParser::getInt(lineStream);
    m_playerLife = FileParser::getInt(lineStream);
    lineStream.ignore();
    
    displacement_speed = FileParser::getFloat(lineStream);
    jump_speed = FileParser::getFloat(lineStream);
}


