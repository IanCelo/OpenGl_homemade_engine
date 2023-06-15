#include <Physics/Collider2.hpp>

#include <Engine/GameObject.hpp>
#include <Engine/Transform3.hpp>

#include <Maths/Intersection2.h>
#include <Core/Log.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

Collider2::Collider2(GameObject* in_gameObject) : Component(in_gameObject)
{
}


bool BoxCollider2D::IntersectWithPoint(Vector2f in_point)
{
	Vector3f pos3 = m_transform->getWorldPosition();
	Vector2f pos = Vector2f(pos3.x + m_position.x, -(pos3.y + m_position.y));

	Vector3f scale3 = m_transform->getWorldScale();

	return intersectionPointBox(in_point, Box(pos, m_colliderScale.y * scale3.y, m_colliderScale.x * scale3.x));
}


void BoxCollider2D::saveComponentInSCNFile(std::ofstream& file)
{
	file << "BCOLLIDER2\t";

	FileWriter::writeVec2InFile(file, m_position);
	file << "/";
	FileWriter::writeVec2InFile(file, m_colliderScale);
	file << "\n";
}

void BoxCollider2D::loadComponentFromSCNFile(std::istringstream& lineStream)
{
	//	Write in log
	Core::Log* _log = Core::Log::instance();
	_log->write("+\t Adding box collider 2D to new gameObject");

	//	Set Rotation Coordinate
	//	-----------------------
	m_position = FileParser::getVector2(lineStream);

	//	Set Rotation Scale
	//	------------------
	lineStream.ignore();
	m_colliderScale = FileParser::getVector2(lineStream);


}

void BoxCollider2D::showImGUI()
{
	ImGui::DragFloat2("Position", &m_position.x);
	ImGui::DragFloat2("Scale", &m_colliderScale.x);
}