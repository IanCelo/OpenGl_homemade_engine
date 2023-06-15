#pragma once

#include <Maths/Shapes3.h>

#include <Engine/Component.hpp>

namespace Physics
{
	enum class ColliderType
	{
		BOX,
		SPHERE
	};

	struct PhysicsMaterial
	{
		float m_friction = 0.05f;
		float m_bounciness = 0.0f;
	};

	class Collider3 : public Component
	{
	public:
		//	Constructor
		//	-----------
		Collider3() = default;
		Collider3(GameObject* in_gameObject);
		~Collider3() override;
		void fixedUpdate()override {};


		//	Variables
		//	---------
		ColliderType m_type = ColliderType::BOX;
		bool m_isTrigger = false;
		int m_physicsLayer = 0;
		PhysicsMaterial m_physicsMat;

		//	Functions
		//	---------

		Vector3f getParentScaleMultiplier();
	};

	class SphereCollider3D : public Collider3
	{
	public:
		//	Constructor
		//	-----------
		SphereCollider3D() = default;
		SphereCollider3D(GameObject* in_gameObject);
		~SphereCollider3D() {};

		//	Variables
		//	---------
		Sphere collider = Sphere();

		//	Functions
		//	---------
		void saveComponentInSCNFile(std::ofstream& file) override;
		void loadComponentFromSCNFile(std::istringstream& lineStream) override;
		void showImGUI() override;
	};



	class BoxCollider3D : public Collider3
	{
	public:
		//	Constructor
		//	-----------
		BoxCollider3D() = default;
		BoxCollider3D(GameObject* in_gameObject);
		~BoxCollider3D();

		//	Variables
		//	---------
		Box collider = Box();

		//	Functions
		//	---------
		void awake() override;
		void saveComponentInSCNFile(std::ofstream& file) override;
		void loadComponentFromSCNFile(std::istringstream& lineStream) override;
		void showImGUI() override;
	};
}
