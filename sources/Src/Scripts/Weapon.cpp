#include <Scripts/Weapon.hpp>
#include <Scripts/Enemy.hpp>

#include <Maths/Utils.h>

#include <Core/InputsManager.hpp>
#include <Core/Window.hpp>
#include <Core/Graph.hpp>
#include <Core/Log.hpp>

#include <LowRenderer/Text.hpp>

#include <Engine/GameObject.hpp>

#include <Physics/PhysicsManager.hpp>

#include <imgui.h>

#include <sstream>
#include <iomanip>


Weapon::Weapon(GameObject* in_gameObject) : Component(in_gameObject)
{
    init(ComponentType::Weapon);
}

void Weapon::start()
{
    //  Get Components
    m_gameObject->tryGetComponent<ParticleSystem>(&m_hitParticules);
    m_gameObject->tryGetComponent<Light>(&m_shootFlash);
}

void Weapon::update()
{
    Core::InputsManager* _inputs = Core::InputsManager::instance();

    //  Write weapon ammo in HUD

    {
        //  clip percentage
        float percent = ((float)m_clipAmmo / (float)m_clipSize);

        //  Convert clip ammo in string
        std::stringstream ss;
        ss << std::setw(3) << std::setfill('0') << m_clipAmmo;

        //  Set colors (cyan to red)
        float r = .5f + (1.f - percent) * .5f;
        float gb = percent;

        //  Stock text in text render buffer
        TextRender::instance()->AddText("abnes.ttf", ss.str(), { 5.5f,-8.25f }, .4f, { .5f,percent,percent });
    }

    {
        //  Convert max clip ammo in string
        std::stringstream ss;
        ss << std::setw(3) << std::setfill('0') << m_ammoReserve;

        //  Stock text in text render buffer (cyan)
        TextRender::instance()->AddText("abnes.ttf", "/" + ss.str(), { 7.f,-8.25f }, .4f, { .5f,1.f,1.f });
    }

    if (m_shootFlash != nullptr)
    {
        m_shootFlash->m_isEnabled = false;
    }

    //  Get ammos
    // is E Pressed ?
    if (_inputs->m_key[SPEED_UP]->pressed)
    {
        //  Get sngleton instance
        Core::Graph* _graph = Core::Graph::instance();

        //  Set forward
        Camera* characterCamera = (Camera*)_graph->getCurrentCamera();
        Vector3f forward;
        if (characterCamera->m_transform)
        {
            forward = vector3RotateByQuaternion({ 0, 0, 1 }, quaternionFromEuler(characterCamera->m_transform->getWorldRotation()));

            //  Check if there is ammo box
            Physics::RaycastHit hitPoint;
            if (_graph->getCurrentScene()->m_physicsManager.raycast(characterCamera->m_transform->getWorldPosition(), -forward, 15.f, hitPoint))
            {
                if (hitPoint.collider->m_gameObject->m_layer.name == "Ammos")
                {
                    _graph->AudioPlayer->play2D("Assets/audio/pickup_ammo.wav", false);
                    PickupAmmo(60);
                }
            }
        }

        
    }

    //  Update cooldown
    m_cooldown.update();
    
    //  Reloading
    if (m_reloadCd.isActive()) 
    {
        m_reloadCd.update();
        
        if (m_reloadCd.getTime() < m_reloadCd.m_endTime / 4.f)
        {
            m_transform->m_rotation.x += .12f;
        }
        else if (m_reloadCd.getTime() > 3.f * (m_reloadCd.m_endTime / 4.f))
        {
            m_transform->m_rotation.x -= .12f;
        }
        
        if (m_reloadCd.ended())
        {
            m_reloadCd.reset();
            m_transform->m_rotation.x = 0.f;
        }
        else return;
    }

    //  Shoot and reload

    if (_inputs->m_key[LEFT_CLICK]->down)Shoot();

    if (_inputs->m_key[RELOAD]->pressed)Reload();
}

void Weapon::Shoot()
{
    Core::Graph* _graph = Core::Graph::instance();
    if (m_clipAmmo > 0 && m_cooldown.ended())
    {
        Camera* characterCamera = (Camera*)_graph->getCurrentCamera();

        Vector3f forward = vector3RotateByQuaternion({ 0, 0, 1 }, quaternionFromEuler(characterCamera->m_transform->getWorldRotation()));

        Physics::RaycastHit hitPoint;
        if (_graph->getCurrentScene()->m_physicsManager.raycast(characterCamera->m_transform->getWorldPosition(), -forward, m_range, hitPoint))
        {
            Enemy* target = nullptr;
            if (hitPoint.collider->m_gameObject->tryGetComponent<Enemy>(&target))
            {
                target->takeDamage(m_damages);
            }

            
        }

        if (m_hitParticules != nullptr)
        {
            m_hitParticules->pos.m_position = hitPoint.hit.point;
            m_hitParticules->setConeDir(hitPoint.hit.normal);
            m_hitParticules->pos.updateTransform();
            m_hitParticules->play();
        }

        // TODO :
        if (m_shootFlash != nullptr)
            m_shootFlash->m_isEnabled = true;
        // ---

        m_clipAmmo--;
        _graph->AudioPlayer->play2D("Assets/audio/shoot.mp3", false);
        if (m_clipAmmo == 0) Reload();
        m_cooldown.restart();
    }
    else if (m_clipAmmo + m_ammoReserve <= 0 && m_cooldown.ended())
    {
        _graph->AudioPlayer->play2D("Assets/audio/empty_shoot.wav", false);
        m_cooldown.restart();
    }
}

void Weapon::Reload()
{
    if (m_ammoReserve <= 0 || m_clipAmmo == m_clipSize)
        return;

    if (m_ammoReserve + m_clipAmmo >= m_clipSize)
    {
        m_ammoReserve -= (m_clipSize - m_clipAmmo);
        m_clipAmmo = m_clipSize;
    }
    else 
    {
        m_clipAmmo += m_ammoReserve;
        m_ammoReserve = 0;
    }
    Core::Graph::instance()->AudioPlayer->play2D("Assets/audio/reload.wav", false);
    m_reloadCd.on();
}

void Weapon::PickupAmmo(int ammunitionAmount) 
{
    m_ammoReserve = clamp(m_ammoReserve+ammunitionAmount, 0, m_ammoMaxCount);
}

void Weapon::saveComponentInSCNFile(std::ofstream& file)
{
    file << "WEAPON\t";

    file << m_range << " ";
    file << m_damages << " ";
    file << m_cooldownTime;

    file << "/";

    file << m_clipSize << " ";
    file << m_clipAmmo << " ";
    file << m_ammoReserve << " ";
    file << m_ammoMaxCount;

    file << "\n";
}

void Weapon::loadComponentFromSCNFile(std::istringstream& lineStream)
{
    //	Write in log
    Core::Log* _log = Core::Log::instance();
    _log->write("+\t Adding weapon script to new gameObject");

    m_range        = FileParser::getFloat(lineStream);
    m_damages      = FileParser::getInt(lineStream);
    m_cooldown     = FileParser::getFloat(lineStream);

    lineStream.ignore();

    m_clipSize     = FileParser::getInt(lineStream);
    m_clipAmmo     = FileParser::getInt(lineStream);
    m_ammoReserve  = FileParser::getInt(lineStream);
    m_ammoMaxCount = FileParser::getInt(lineStream);
}

void Weapon::showImGUI()
{
    using namespace ImGui;

    static int reloadCount = 10;
    DragInt("new ammunitions", &reloadCount);
    if (Button("Pickup Ammo"))
        PickupAmmo(reloadCount);
    if (Button("Reload"))
        Reload();

    Text("Reserve Ammo: "); SameLine(); Text(std::to_string(m_ammoReserve).c_str());
    Text("Clip Ammo: "); SameLine(); Text(std::to_string(m_clipAmmo).c_str());
    DragInt("Damages", &m_damages);
    DragFloat("Range", &m_range);

    if (DragFloat("Cooldown", &m_cooldownTime))
        m_cooldown.setEndTime(m_cooldownTime);
}
