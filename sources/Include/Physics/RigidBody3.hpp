#pragma once

#include <Maths/Intersection3.h>
#include <Maths/Quaternion.h>

#include <Engine/Component.hpp>

#include <Physics/Collider3.hpp>

#define EARTH_GRAVITY 9.81f

namespace Physics
{
	enum class ForceMode
	{
		Acceleration,
		Impulse
	};

	class Rigidbody3 : public Component
	{
	public:
		//	Constructor
		//	-----------
		Rigidbody3() = default;
		Rigidbody3(GameObject* in_gameObject);
		~Rigidbody3() override;

		//	Internal Public Variables
		//	-------------------------
		Collider3* m_collider = nullptr;

		float m_mass = 1.0f;
		float m_drag = 0.2f;
		float m_gravityMultiplier = 1.0f;

		Vector3f m_forceAcceleration = { 0 };
		Vector3f m_forceImpulse = { 0 };
		Vector3f m_acceleration = { 0 };
		Vector3f m_velocity = { 0 };

		bool  m_useGravity = true;
		bool m_isKinematic = false;

		void awake() override;

		//	Internal Public Functions
		//	-------------------------
		void addForce(const Vector3f& force, ForceMode mode);

		//	Calculate the next frame position
		//	Parameters : float deltaTime
		//	----------------------------
		Vector3f calculateNextPos();
		void updatePosition();

		void showImGUI() override;
		void saveComponentInSCNFile(std::ofstream& file) override;
		void loadComponentFromSCNFile(std::istringstream& lineStream) override;

		void Reset();
	};
}


