
#include <Resources/Particle.hpp>

#include <Core/TimeManager.h>

#include <Physics/RigidBody3.hpp>

#include <Maths/Random.hpp>
namespace Resources
{
	Particle::Particle(Transform3* parent, const ParticleSpecs& infos)
	{
		timer = { infos.lifetime, true };
		speed = infos.startSpeed;
		transform.m_scale = infos.startSize;
		transform.makeParent(parent);
		color = infos.startColor;
		scale = infos.startSize;
		if (infos.isCone)
		{
			velocity = Maths::randCone({transform.m_position.z, transform.m_position.z + infos.Conesize},infos.spawnCircleRadius, infos.endCircleRadius);
			Maths::Quaternion rot = Maths::quaternionFromVector3ToVector3({0.f,0.f,1.f}, infos.coneDirection);
			velocity = Maths::vector3RotateByQuaternion(velocity, rot);
		}
		velocity += infos.velocity;
		gravity_multiplier = infos.gravity_multiplier;
	}

	void Particle::update() 
	{
		timer.update();

		velocity += -Vector3f::yAxis() * EARTH_GRAVITY * gravity_multiplier * Core::TimeManager::instance()->deltaTime;
		transform.m_position += Core::TimeManager::instance()->deltaTime * speed * velocity;

		transform.updateTransform();
	}
}