#pragma once

#include <LowRenderer/CameraBase.hpp>

#include <Engine/Component.hpp>

class CameraHUD : public  CameraBase
{
private:
public:

    //  Constructor
    //  -----------
    CameraHUD();
    ~CameraHUD() = default;


    //  Variables
    //  ---------

    Maths::Vector3f m_position;

    //  Functions
    //  ---------

    void update();

    //  Get the view matrix of the camera
    //  Parameters : none
    //  -----------------
    Maths::Mat4x4 getViewMatrix() const;


    void showHUDCameraImGui();
};
