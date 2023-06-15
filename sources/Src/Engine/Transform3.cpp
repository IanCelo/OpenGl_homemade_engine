#include <Engine/Transform3.hpp>
#include <Engine/GameObject.hpp>

#include <Core/Log.hpp>

#include <Physics/RigidBody3.hpp>

#include <Maths/Utils.h>

#include <Resources/Scene.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>


Transform3::Transform3()
{
	init(ComponentType::Transform);

	m_model = mat4x4Identity(); 

	m_position = Vector3f::zero();
	m_rotation = Vector3f::zero();
	m_scale = { 1.f,1.f,1.f };

	m_quaternionRotation = quaternionFromEuler(m_rotation);
}


Transform3::~Transform3()
{
	//	Destroy all children objects
	while (m_childList.empty() == false)
	{
		m_childList.back()->m_gameObject->destroy();
	}

	//	Remove itself from its parent
	if (m_parent)
	{
		int i = 0;

		for (Transform3* me : m_parent->m_childList)
		{
			if (me == this)
			{
				m_parent->m_childList.erase(m_parent->m_childList.begin() + i);
				break;
			}
			i++;
		}
	}
}

Transform3::Transform3(GameObject* in_gameObject) : Component(in_gameObject)
{
	init(ComponentType::Transform);

	m_gameObject = in_gameObject;

	//	Matrix default value
	m_model = mat4x4Identity();

	//	Transform default value
	m_position = Vector3f::zero();
	m_rotation = Vector3f::zero();
	m_scale = { 1.f,1.f,1.f };

	m_quaternionRotation = quaternionFromEuler(m_rotation);

	in_gameObject->m_transform = m_transform =  this;
}


Mat4x4 Transform3::translate()
{
	return Maths::translate(m_position);
}

Mat4x4 Transform3::scale()
{
	return Maths::scale(m_scale);
}

Mat4x4 Transform3::rotate()
{
	return
	(
		Maths::rotateZ(m_rotation.z) *
		Maths::rotateY(m_rotation.y) *
		Maths::rotateX(m_rotation.x)
	);
}

void Transform3::birth()
{
	if (parentName == "") return;

	Core::Log* _log = Core::Log::instance();

	bool found = false;

	//	If a transform parent is expected
	for (auto currentObject : m_gameObject->m_sceneReference->m_objectList)
	{
		//	The transform parent has been found
		if (currentObject.second->m_name == parentName)
		{
			makeParent(currentObject.second->m_transform);

			found = true;

			_log->write("+\t Transform parent found");

			break;
		}
	}

	//	If the expected transform parent couldn't be found
	if (found == false)
	{
		_log->writeWarning(m_name + " has no parent");
	}
}

void Transform3::update()
{
	checkChildList();
}

void Transform3::updateTransform()
{
	if (m_rotation.x < -TAU)
		m_rotation.x += TAU;
	if (m_rotation.y < -TAU)
		m_rotation.y += TAU;
	if (m_rotation.z < -TAU)
		m_rotation.z += TAU;

	if (m_rotation.x > TAU)
		m_rotation.x -= TAU;
	if (m_rotation.y > TAU)
		m_rotation.y -= TAU;
	if (m_rotation.z > TAU)
		m_rotation.z -= TAU;

	calculateModel();
	calculateQuaternion();
}

void Transform3::calculateModel()
{
	m_model = translate() * rotate() * scale();
}

void Transform3::calculateQuaternion()
{
	m_quaternionRotation = quaternionFromEuler(m_rotation);
}

Vector3f Transform3::getWorldPosition() const
{
	Vector3f worldPosition = m_position;

	if (m_parent)
	{
		worldPosition += m_parent->getWorldPosition();
	}

	return worldPosition;
}

Vector3f Transform3::getWorldScale() const
{
	Vector3f worldScale = m_scale;

	if (m_parent)
	{
		Vector3f parentsScale = m_parent->getWorldScale();

		worldScale = Vector3f(worldScale.x * parentsScale.x,
			worldScale.y * parentsScale.y,
			worldScale.z * parentsScale.z);
	}
	return worldScale;
}

Vector3f Transform3::getWorldRotation() const
{
	Vector3f worldRotation = m_rotation;

	if (m_parent)
	{
		worldRotation += m_parent->getWorldRotation();
	}

	return worldRotation;
}

void Transform3::checkChildList()
{
	int i = 0;
	for (auto child : m_childList)
	{
		if (!child)
		{
			m_childList.erase(m_childList.begin() + i);
		}
		else if (child->m_parent != this)
		{
			m_childList.erase(m_childList.begin() + i);
		}
		i++;
	}
}

void Transform3::makeParent(Transform3* in_parent)
{
	if (in_parent == nullptr)
	{
		//in_parent->checkChildList();
		//in_parent->m_childList.push_back(this);
		m_parent = nullptr;
		parentName = "";
	}
	else
	{
		m_parent = in_parent;
		in_parent->checkChildList();
		in_parent->m_childList.push_back(this);
		parentName = in_parent->m_gameObject->m_name;
	}
}


bool Transform3::areRelated(const Transform3* in_transform) const
{
	bool out = false;
	
	if (m_parent)
	{
		return  m_parent == in_transform || m_parent->areRelated(in_transform);
	}

	return false;
	
}

void Transform3::showImGUI()
{	
	using namespace ImGui;

	bool canUpdateTransform = false;

	Vector3f rot = m_rotation * RAD_TO_DEG;

	canUpdateTransform |= DragFloat3(" <- Object Position", &m_position.c[0]);
	canUpdateTransform |= DragFloat3(" <- Object Rotation", &rot.c[0]);
	canUpdateTransform |= DragFloat3(" <- Object Scale", &m_scale.c[0]);

	m_rotation = rot * DEG_TO_RAD;

	const char* preview = (m_parent ? m_parent->m_gameObject->m_name.c_str() : "no parent");

	ImGui::NewLine();
	if (ImGui::BeginCombo("Parent Object", preview)) // The second parameter is the label previewed before opening the combo.
	{
		bool is_selected = (preview == "no parent");

		//	Nothing is selected
		if (ImGui::Selectable("no parent", is_selected))
		{
			m_parent = nullptr;
			canUpdateTransform = true;
		}
		
		for (auto& object : m_gameObject->m_sceneReference->m_objectList)
		{
			if (m_gameObject == object.second) continue;
			if (object.second->m_transform->areRelated(this) || object.second->m_transform == m_parent) continue;

			is_selected = (object.second->m_name.c_str() == preview);

			if (ImGui::Selectable(object.second->m_name.c_str(), is_selected))
			{
				makeParent(object.second->m_transform);

				canUpdateTransform = true;
			}
		}
		ImGui::EndCombo();
	}

	if (canUpdateTransform)
	{
		updateTransform();
	}
}


void Transform3::saveComponentInSCNFile(std::ofstream& file)
{
	file << "TRANSFORM\t";

	FileWriter::writeVec3InFile(file, m_position); file << "/";
	FileWriter::writeVec3InFile(file, m_rotation); file << "/";
	FileWriter::writeVec3InFile(file, m_scale);

	if (m_parent)
	{
		file << "\t" << m_parent->m_gameObject->m_name;
	}

	file << "\n";
}

void Transform3::loadComponentFromSCNFile(std::istringstream& lineStream)
{
	Core::Log* _log = Core::Log::instance();
	_log->write("+\t Adding transform data to new gameObject");

	//	Set Rotation Coordinate
	m_position = FileParser::getVector3(lineStream);

	//	Set Rotation Transform
	lineStream.ignore();
	m_rotation = FileParser::getVector3(lineStream);

	//	Set Rotation Scale
	lineStream.ignore();
	m_scale = FileParser::getVector3(lineStream);

	m_parent = nullptr;

	std::string parent = FileParser::getString(lineStream);
	if (parent != "\n" && parent != "")
	{
		//	Will be set in birth function
		parentName = parent;
	}

	updateTransform();
}
