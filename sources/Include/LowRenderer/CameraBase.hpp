#pragma once

#include <Maths/Matrix.h>
#include <Maths/Utils.h>

enum ProjectionMode
{
    PERSPECTIVE,
    ORTHOGRAPHIC
};

class CameraBase
{
public:

    //  Constructor
    //  -----------

    CameraBase();

    //  Get the view matrix of the camera
    //  Parameters : none
    //  -----------------
    virtual Maths::Mat4x4 getViewMatrix() const = 0;

    virtual Vector3f getPosition() const { return Vector3f::zero(); };

    //  Get the projection of the camera
    //  Parameters : none
    //  -----------------
    Maths::Mat4x4 getPerspectiveProjection() const;
    Maths::Mat4x4 getOrthographicProjection() const;

    void showCameraImGUI();
    void setDimensions();

    //  Public Variables
    //  ----------------

    int projectionMode = PERSPECTIVE;

    //Projection
    float width;
    float height;
    float aspect;

    float fovY;
    float near;
    float far;
};