#include <Core/InputsManager.hpp>
#include <Core/TimeManager.h>
#include <Core/Log.hpp>

#include <Config.hpp>

#include <LowRenderer/Camera.hpp>

#include <Engine/Transform3.hpp>
#include <Engine/GameObject.hpp>

#include <Resources/Scene.hpp>
         
#include <Maths/Utils.h>
#include <Maths/Utils.h>

#include <imgui.h>

Camera::Camera(GameObject* in_gameObject) : Component(in_gameObject)
{
    m_type = (int)ComponentType::Camera;

    ComponentTypeInfo* _componentManager = ComponentTypeInfo::instance();

    m_name = _componentManager->m_componentList[(int)m_type].name;

    //  Width and height are useful to compute projection matrix with the right aspect ratio
    //  ------------------------------------------------------------------------------------
    aspect = (float)SCR_WIDTH / (float)SCR_HEIGHT;

    m_gameObject->m_sceneReference->m_rendererManager.m_cameraList[(int)m_gameObject->m_sceneReference->m_rendererManager.m_cameraList.size()] = this;
}


void Camera::update()
{
    Core::InputsManager* _inputs = Core::InputsManager::instance();
    Core::TimeManager* _time = Core::TimeManager::instance();

    setDimensions();
    if (!m_useParentAsPivot) 
    {
        m_transform->m_rotation.x += _inputs->m_mouseDeltaY * MOUSE_SENSIVITY * _time->deltaTime;
        m_transform->m_rotation.x = Maths::clamp(m_transform->m_rotation.x, -(float)HALF_PI, (float)HALF_PI);
    }
    else if (m_useParentAsPivot && m_transform->m_parent)
    {
        m_transform->m_parent->m_transform->m_rotation.x -= _inputs->m_mouseDeltaY * MOUSE_SENSIVITY * _time->deltaTime;
        m_transform->m_parent->m_transform->m_rotation.x = Maths::clamp(m_transform->m_parent->m_transform->m_rotation.x, -(float)HALF_PI, (float)HALF_PI);
        m_transform->m_parent->m_transform->updateTransform();
    }
}


Maths::Mat4x4 Camera::getViewMatrix() const 
{
    Vector3f pos = m_transform->m_position;
    Vector3f rot = m_transform->m_rotation;
    
    const Transform3* parent = m_transform->m_parent;
    while (parent)
    {
        pos += parent->m_transform->m_position;
        rot -= parent->m_transform->m_rotation;
        parent = parent->m_parent;
    }
    
    return Maths::rotateX(rot.x) * Maths::rotateY(rot.y) * Maths::translate(-pos);
}


Vector3f Camera::getPosition() const
{
    return m_transform->m_position;
}

void Camera::showImGUI()
{
    CameraBase::showCameraImGUI();

    ImGui::Checkbox("Use Parent As Pivot", &m_useParentAsPivot);
}


void Camera::destroy()
{
    //  get Renderer Manager of the scene
    Core::RendererManager* _renderer = &m_gameObject->m_sceneReference->m_rendererManager;

    //  Get last index (trash index)
    int lastIndex = (int)_renderer->m_cameraList.size() - 1;

    //  Parsing list of the current renderer component type
    for (auto comp : _renderer->m_cameraList)
    {
        //  Once we find "this", let's procees to the retirement of the component from the list
        if (comp.second == this)
        {
            //  Swap current with the last one if it's not already the last one
            if (comp.first != lastIndex)
            {
                _renderer->m_cameraList[comp.first] = _renderer->m_cameraList[lastIndex];
            }

            //  Set the pointer to null
            _renderer->m_cameraList[lastIndex] = nullptr;

            //  Then erase it from the list
            _renderer->m_cameraList.erase(lastIndex);

            //  And leave the loop
            //  To avoid errors and optimize
            break;
        }
    }

    //  Then call the common component destoy function
    Component::destroy();
}


void Camera::saveComponentInSCNFile(std::ofstream& file)
{
    file << "CAMERA\t\t" << (int)(fovY * RAD_TO_DEG) << "/" << near << "/" << far << "/" << projectionMode << "/" << m_useParentAsPivot << "\n";
}

void Camera::loadComponentFromSCNFile(std::istringstream& lineStream)
{
    //	Write in log
    Core::Log* _log = Core::Log::instance();
    _log->write("+\t Adding camera to new gameObject");

    fovY = FileParser::getFloat(lineStream) * DEG_TO_RAD;
    lineStream.ignore();
    near = FileParser::getFloat(lineStream);
    lineStream.ignore();
    far = FileParser::getFloat(lineStream);
    lineStream.ignore();
    projectionMode = FileParser::getInt(lineStream);
    lineStream.ignore();
    m_useParentAsPivot = FileParser::getBool(lineStream);
}