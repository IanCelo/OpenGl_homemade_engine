#pragma once
#include <Maths/Quaternion.h>
#include <Maths/Matrix.h>
#include <Maths/Vector3.h>
#include <deque>

#include <Engine/Component.hpp>

//	Transform class, contain position, rotation and scale of an object
//	------------------------------------------------------------------

class Transform3 : public Component
{

private:

	//	Variable
	//	--------

	Mat4x4	m_model = mat4x4Identity();

	Quaternion m_quaternionRotation = quaternionIdentity();


	//	Return true if the transform and the other in parameters have family links
	//	Parameters : none
	//	-----------------
	bool areRelated(const Transform3* in_transform) const;


public:

	//	Constructors
	//	------------

	Transform3();
	~Transform3();
	Transform3(GameObject* in_gameObject);

	//	Variables
	//	---------

	Transform3* m_parent = nullptr;

	Vector3f	m_position = Vector3f::zero();
	Vector3f	m_rotation = Vector3f::zero();
	Vector3f	m_scale	   = { 1.f,1.f,1.f };

	std::deque<Transform3*> m_childList;

	//	Functions
	//	---------

	Mat4x4 translate();
	Mat4x4 scale();
	Mat4x4 rotate();

	std::string parentName;

	//	Update Mat4x4 of the transform from position, rotation and scale values
	//	Parameters : none
	//	-----------------
	void updateTransform();

	void calculateModel();
	void calculateQuaternion();

	void checkChildList();

	void makeParent(Transform3* in_parent);

	//	Get World Position
	//	Parameters : none
	//	-----------------
	Vector3f getWorldPosition() const;

	//	Get World Scale
	//	Parameters : none
	//	-----------------
	Vector3f getWorldScale() const;

	//	Get World Rotation in Euler
	//	Parameters : none
	//	-----------------
	Vector3f getWorldRotation() const;

	//	Get the quaternion of the transform rotation
	//	Parameters : none
	//	-----------------
	Quaternion getQuaternionRotation() const { return m_quaternionRotation; }

	//	Get Mat4x4 of the transform
	//	Parameters : none
	//	-----------------
	Mat4x4 getTransformMatrix() const 
	{ 
		if (m_parent) return m_parent->getTransformMatrix() * m_model;
		return m_model;
	}

	//	Override functions
	//	-----------------

	void birth() override;
	void update() override;
	void fixedUpdate()override {}

	// Referential vectors
	// -------------------
	Vector3f getForward() const { return Vector3f{ m_model.e[8], m_model.e[9], m_model.e[10] }.normalized(); }
	Vector3f getLeft() const { return Vector3f{ m_model.e[0], m_model.e[1], m_model.e[2] }.normalized(); }
	Vector3f getUp() const { return Vector3f{ m_model.e[4], m_model.e[5], m_model.e[6] }.normalized(); }

	void showImGUI() override;
	void saveComponentInSCNFile(std::ofstream& file) override;
	void loadComponentFromSCNFile(std::istringstream& lineStream) override;

};
