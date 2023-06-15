#pragma once

#include <LowRenderer/CameraBase.hpp>

#include <Engine/Component.hpp>

class CameraEditor : public  CameraBase
{
private:

    //  Private Internal Variables
    //  --------------------------

    bool m_movementLocked = true;
    Vector3f pos = { 0.0f , 2.5 ,-7.5f };

    float yaw, pitch;

public:

    //  Constructor
    //  -----------
    CameraEditor();
    ~CameraEditor() = default;

    //  Functions
    //  ---------

    void update();
    //void showImGUI();
    Vector3f getPosition() const override;

    //  Get the view matrix of the camera
    //  Parameters : none
    //  -----------------
    Maths::Mat4x4 getViewMatrix() const;

    //Transform 
    float displacement_speed;

};
