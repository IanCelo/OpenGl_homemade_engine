#pragma once

#include <Engine/Component.hpp>


class Collider2 : public Component
{
public:
	//	Constructor
	//	-----------
	Collider2() = default;
	Collider2(GameObject * in_gameObject);
	virtual ~Collider2() {};
	void fixedUpdate()override {};


	//	Variables
	//	---------
	Vector2f	m_position;

	//	Functions
	//	---------
	virtual bool IntersectWithPoint(Vector2f in_point) = 0;

};


class BoxCollider2D : public Collider2
{
public:

	BoxCollider2D(GameObject* in_gameObject) : Collider2(in_gameObject) 
	{
		init(ComponentType::BoxCollider2D);
		m_colliderScale = Vector2f(1, 1);
	};
	//	Variables
	//	---------

	Vector2f m_colliderScale;

	//	Functions
	//	---------

	bool IntersectWithPoint(Vector2f in_point) override;

	void saveComponentInSCNFile(std::ofstream& file) override;
	void loadComponentFromSCNFile(std::istringstream& lineStream) override;

	void showImGUI() override;
	void update() override {};
};