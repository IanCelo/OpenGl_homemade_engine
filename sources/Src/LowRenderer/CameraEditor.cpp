#include <LowRenderer/CameraEditor.hpp>
#include <Core/InputsManager.hpp>
#include <Core/TimeManager.h>
#include <Maths/Utils.h>
#include <Config.hpp>

CameraEditor::CameraEditor()
{
    displacement_speed = CAMERA_SPEED;
    pitch = 0.f;
    yaw = 0.f;

    aspect = SCR_WIDTH / SCR_HEIGHT;
}


void CameraEditor::update()
{
    Core::InputsManager* _inputs = Core::InputsManager::instance();
    Core::TimeManager* _time = Core::TimeManager::instance();

    setDimensions();


    if (_inputs->m_key[RIGHT_CLICK]->down)
    {
        // Avoid having an abrupt camera movement when right-clicking
        if (m_movementLocked) _inputs->resetOldMouse();

        m_movementLocked = false;
        _inputs->hideMouse();
    }
    else
    {
        m_movementLocked = true;
        _inputs->displayMouse();
        return;
    }



    if (_inputs->m_key[SPEED_UP]->down) displacement_speed += 1.f;

    if (_inputs->m_key[SPEED_DOWN]->down) displacement_speed -= 1.f;



    //  Update angle in function of the new mouse position
    //  --------------------------------------------------
    //yaw
    yaw += _inputs->m_mouseDeltaX * MOUSE_SENSIVITY * _time->deltaTime;

    //  pitch
    pitch += _inputs->m_mouseDeltaY * MOUSE_SENSIVITY * _time->deltaTime;
    pitch = Maths::clamp(pitch, -(float)HALF_PI, (float)HALF_PI);


    //  Initialize movements coefficients
    //  ---------------------------------
    float forward_movement = 0.f;
    float strafe_movement = 0.f;
    float height_movement = 0.f;

    //  Forward movements
    //  -----------------

    if (_inputs->m_key[FORWARD_INPUT]->down) forward_movement -= _time->deltaTime * displacement_speed;

    if (_inputs->m_key[BACKWARD_INPUT]->down) forward_movement += _time->deltaTime * displacement_speed;

    //  Strafe movements
    //  ----------------

    if (_inputs->m_key[RIGHT_INPUT]->down) strafe_movement -= _time->deltaTime * displacement_speed;

    if (_inputs->m_key[LEFT_INPUT]->down) strafe_movement += _time->deltaTime * displacement_speed;


    //  Height movements
    //  ----------------

    if (_inputs->m_key[UP_INPUT]->down) height_movement += _time->deltaTime * displacement_speed;

    if (_inputs->m_key[DOWN_INPUT]->down) height_movement -= _time->deltaTime * displacement_speed;

    //  Update angle
    //  ------------

    // To put in FollowCamera
    /*
    yaw -= strafe_movement;
    pitch -= forward_movement;
    float savedPitch = pitch;
    pitch = Maths::clamp(pitch, -(float)HALF_PI, (float)HALF_PI);


    if (savedPitch != pitch)
        forward_movement = 0;
    
    */

    //  Update position
    //  ---------------

    pos.x -= forward_movement * sinf(yaw) + strafe_movement * cosf(yaw);
    pos.z += forward_movement * cosf(yaw) - strafe_movement * sinf(yaw);
    pos.y += height_movement;
}

Maths::Mat4x4 CameraEditor::getViewMatrix() const
{
    return Maths::rotateX(pitch) * Maths::rotateY(yaw) * Maths::translate(-pos);
}

Vector3f CameraEditor::getPosition() const
{
    return pos;
}