#include <LowRenderer/CameraBase.hpp>
#include <Config.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <Core/Window.hpp>

CameraBase::CameraBase()
{
    fovY = DEG_TO_RAD * DEFAULT_FOV;
    near = 0.1f;
    far = 10000.f;

    void setDimensions();
}

void CameraBase::setDimensions()
{
    Core::Window* _window = Core::Window::instance();

    int newwidth = 0, newheight = 0;

    width = (float)_window->m_width;
    height = (float)_window->m_height;
    aspect = width / height;
}



void CameraBase::showCameraImGUI()
{
    using namespace ImGui;

    if (ImGui::TreeNode("Camera setting"))
    {

        float fov = fovY * RAD_TO_DEG;
        if (SliderFloat("FOV", &fov, 0.f, 160.f))
        {
            fovY = fov * DEG_TO_RAD;
        }

        SliderFloat("Near", &near, 0.f, 5.f);
        SliderFloat("Far", &far, 5.f, 20000.f);

        std::string mode;
        if (projectionMode == ORTHOGRAPHIC)
        {
            mode = "orthographic";
        }
        else
        {
            mode = "perspective";
        }

        if (ImGui::BeginCombo("Projection mode", mode.c_str()))
        {
            if (ImGui::Selectable("perspective"))
            {
                projectionMode = PERSPECTIVE;
            }
            if (ImGui::Selectable("orthographic"))
            {
                projectionMode = ORTHOGRAPHIC;
            }
            ImGui::EndCombo();
        }
        ImGui::TreePop();
    }
}

Maths::Mat4x4 CameraBase::getPerspectiveProjection() const
{
    return Maths::perspective(fovY, aspect, near, far);
}


Maths::Mat4x4 CameraBase::getOrthographicProjection() const
{
    float scale = ORTHOGRAPHIC_SCALE;
    return Maths::Orthographic(-scale, scale, -scale, scale, -scale, scale);
}