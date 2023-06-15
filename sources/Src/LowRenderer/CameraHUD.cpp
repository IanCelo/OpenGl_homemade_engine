#include <LowRenderer/CameraHUD.hpp>
#include <Core/InputsManager.hpp>
#include <Core/TimeManager.h>
#include <Maths/Utils.h>
#include <Config.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

CameraHUD::CameraHUD()
{
    aspect = SCR_WIDTH / SCR_HEIGHT;
    projectionMode = ORTHOGRAPHIC;
    m_position = Vector3f(0, 0, 1);
}


void CameraHUD::update()
{
    setDimensions();
}

Maths::Mat4x4 CameraHUD::getViewMatrix() const
{
    return Maths::translate(-m_position);
}


void CameraHUD::showHUDCameraImGui()
{
    ImGui::DragFloat3("position", &m_position.x);
}