#pragma once

#include <LowRenderer/CameraBase.hpp>

#include <Engine/Component.hpp>

class Camera : public Component, public CameraBase
{
private:

    //  Private Internal Variables
    //  --------------------------

    bool m_movementLocked = true;
    float m_rotationSpeed = 0.045f;

    bool m_useParentAsPivot = false;
public:

    //  Constructor
    //  -----------

    Camera() = default;
    ~Camera() = default;
    Camera(GameObject* in_gameObject);

    //  Functions
    //  ---------

    void update() override;
    Vector3f getPosition() const override;

    void updateCameraEditor() {}

    //  Get the view matrix of the camera
    //  Parameters : none
    //  -----------------
    Maths::Mat4x4 getViewMatrix() const;

    void showImGUI() override;
    void destroy()   override;

    void saveComponentInSCNFile(std::ofstream& file) override;
    void loadComponentFromSCNFile(std::istringstream& lineStream) override;
    //Transform 
    //float displacement_speed = DISPLACEMENT_SPEED;
};
