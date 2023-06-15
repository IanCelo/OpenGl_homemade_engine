#include <Scripts/Enemy.hpp>
#include <Scripts/Player.hpp>

#include <Core/Graph.hpp>
#include <Core/GameManager.hpp>
#include <Core/Log.hpp>

#include <Maths/Random.hpp>

#include <imgui.h>

Enemy::Enemy(GameObject* in_gameObject) : Component(in_gameObject)
{
	init(ComponentType::Enemy);
}

void Enemy::start()
{
	//	Get Components

	m_rigidbody = m_gameObject->getComponent<Rigidbody3>();
	m_gameObject->tryGetComponent<ParticleSystem>(&m_hitParts);

	//	Find and get player component
	GameObject* player = Core::Graph::instance()->getCurrentScene()->findGameObjectInScene("Player");

	if (player != nullptr)
	{
		m_player = player->getComponent<Player>();
		Core::Log::instance()->write("Found player");
	}
}

void Enemy::update()
{
	//	Set direction toward player
	Vector3f vectorToPlayer = m_player->m_transform->m_position - m_transform->m_position;
	m_distanceToPlayer = vectorToPlayer.squareLength();
	m_directionToPlayer.y = 0;
	m_directionToPlayer = vectorToPlayer.normalized();

	//	Update attack cooldown
	m_attackCooldown.update();

	//	If player in range and cooldown is done
	if (m_distanceToPlayer < m_damageRange * m_damageRange && m_attackCooldown.ended())
	{
		//	Deal damages to player
		m_player->damage(m_damages);

		//	PLay hit particles
		if (m_hitParts != nullptr)
		{
			m_hitParts->pos.m_position = m_transform->m_position;
			m_hitParts->pos.updateTransform();
			m_hitParts->play();
		}

		//	Reset cooldown
		m_attackCooldown.restart();
	}

	//	If life beneath 0
	if (m_life <= 0)
	{
		//	Play Sounds
		
		float percent = Maths::randRange(0, 15);
		if (percent < 10)
			Core::Graph::instance()->AudioPlayer->play2D("Assets/audio/explosion.wav", false);
		else
			Core::Graph::instance()->AudioPlayer->play2D("Assets/audio/easter_egg_explosion.mp3", false);

		//	Disable Enemy and remove it
		m_gameObject->m_isEnabled = false;
		Core::GameManager::instance()->removeEnemyFromList(m_indexInGameList);
	}
}

void Enemy::fixedUpdate()
{
	//	Enemy chase
	//	If ennemy was provoked OR player is in range
	if (m_angry || m_distanceToPlayer < m_detectionRange * m_detectionRange)
	{
		//	Update Enemy position
		m_rigidbody->addForce(m_directionToPlayer * m_speed, ForceMode::Acceleration);

		//	Update Enemy rotation
		float angle = dotProduct(m_directionToPlayer, m_transform->getLeft());
		if (fabsf(angle) > 0.05f)
		{
			m_transform->m_rotation.y += 2.0f * Core::TimeManager::instance()->fixedDeltaTime * (angle > 0 ? 1 : -1);
		}
	}
}

void Enemy::takeDamage(int damages)
{
	//	Take received damage and decrease life
	m_life -= damages;

	//	The enemy has been provoked
	m_angry = true;
}

void Enemy::saveComponentInSCNFile(std::ofstream& file)
{
	file << "ENEMY\t\t";

	file << m_life << " ";
	file << m_damages;
	file << "/";
	file << m_detectionRange << " ";
	file << m_damageRange;
	file << "/";
	file << m_speed;
	file << "\n";
}

void Enemy::showImGUI()
{
	using namespace ImGui;

	DragInt("Lives", &m_life);
	DragFloat("Speed", &m_speed);
	DragInt("Damages", &m_damages);
	DragFloat("Detection range", &m_detectionRange);
	DragFloat("Attack range", &m_damageRange);
}

void Enemy::loadComponentFromSCNFile(std::istringstream& lineStream)
{
	//	Write in log
	Core::Log* _log = Core::Log::instance();
	_log->write("+\t Adding player script to new gameObject");

	m_indexInGameList = Core::GameManager::instance()->addEnemyToList(m_gameObject);
	
	m_life =	FileParser::getInt(lineStream);
	m_damages = FileParser::getInt(lineStream);
	lineStream.ignore();

	m_detectionRange =  FileParser::getFloat(lineStream);
	m_damageRange =	FileParser::getFloat(lineStream);
	lineStream.ignore();

	m_speed = FileParser::getFloat(lineStream);
}
