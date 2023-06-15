#pragma once

#include <Engine/Transform3.hpp>
#include <Utils/Timer.hpp>

struct ParticleSpecs
{
	float lifetime;
	float startSize;
	float startSpeed;
	Maths::Vector4f startColor;
	Maths::Vector3f velocity;
	bool isCone;
	float Conesize;
	float spawnCircleRadius;
	float endCircleRadius;
	float gravity_multiplier;
	Maths::Vector3f coneDirection;
};

namespace Resources
{
	class Particle
	{
	public:

		//	Constructor & Destructor
		//	------------------------

		Particle(Transform3* parent, const ParticleSpecs& infos);

		//	Public Variables
		//	---------------

		void update();

		Maths::Vector3f velocity;
		Transform3 transform;
		Timer timer;
		Maths::Vector3f scale;
		//	Getters And Setters
		//	-------------------------

		Maths::Vector4f getColor() const { return color; }

	private:

		Maths::Vector4f color;
		float speed = 0.f;
		float gravity_multiplier = 0.f;
	};
}