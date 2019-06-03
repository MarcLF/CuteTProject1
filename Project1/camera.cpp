#include "camera.h"

#include <qvector4d.h>
#include <qmath.h>
#include <iostream>

Camera::Camera(int width, int height)
{
    position = QVector3D(0, 0, 0);

    viewportWidth = width;
    viewportHeight = height;

    fov = 60.0f;
    zNear = 0.5f;
    zFar = 10000.0f;
    yaw = 0.0f;
    pitch = 0.0f;
    speed = 10.0f;

    worldMatrix = QMatrix4x4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);
    viewMatrix =  QMatrix4x4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);

    PrepareMatrices();
}

QVector4D Camera::GetLeftRightBottomTop()
{
    const float aspectRatio = float(viewportWidth/viewportHeight);
    const float alpha = qDegreesToRadians(fov * 0.5f);
    const float top = zNear * qTan(alpha);
    const float bottom = -top;
    const float right = top * aspectRatio;
    const float left = -right;
    QVector4D params(left,right,bottom,top);
    return params;
}

QVector3D Camera::ScreenPointToWorldRay(int x, int y)
{
    QVector4D lrbt = GetLeftRightBottomTop();
    const float l = lrbt.x();
    const float r = lrbt.y();
    const float b = lrbt.z();
    const float t = lrbt.w();
    const float rayX = l +(r-l) * x/ viewportWidth;
    const float rayY = b + (t - b) * (viewportHeight - y - 1) / viewportHeight;
    const float rayZ = -zNear;
    QVector3D rayViewspace = QVector3D(rayX,rayY,rayZ);

    PrepareMatrices();

    QVector3D ray_world_space = QVector3D(worldMatrix * QVector4D(rayViewspace, 0.0f));

    return ray_world_space;
}

void Camera::PrepareMatrices()
{
    worldMatrix.setToIdentity();
    worldMatrix.translate(position);
    worldMatrix.rotate(yaw, QVector3D(0.0f,1.0f,0.0f));
    worldMatrix.rotate(pitch, QVector3D(1.0f,0.0f,0.0f));

    viewMatrix = worldMatrix.inverted();

    projectionMatrix.setToIdentity();
    projectionMatrix.perspective(fov,float(viewportWidth)/viewportHeight, zNear, zFar);
}

void Camera::Move(QVector3D movement)
{
    QVector3D moveVec = QVector3D(0.0f, 0.0f, 0.0f);
    //press W
    if(movement == QVector3D(0,0,1))
    {
        moveVec += QVector3D(-sinf(qDegreesToRadians(yaw)) * cosf(qDegreesToRadians(pitch)),
                         sinf(qDegreesToRadians(pitch)),
                         -cosf(qDegreesToRadians(yaw)) * cosf(qDegreesToRadians(pitch)));
    }

    //press S
    if(movement == QVector3D(0,0,-1))
    {
        moveVec += QVector3D(sinf(qDegreesToRadians(yaw)) * cosf(qDegreesToRadians(pitch)),
                            -sinf(qDegreesToRadians(pitch)),
                            cosf(qDegreesToRadians(yaw)) * cosf(qDegreesToRadians(pitch)));
    }

    //press A
    if(movement == QVector3D(-1,0,0))
    {
        moveVec += QVector3D(-cosf(qDegreesToRadians(yaw)),
                             0.0f,
                             sinf(qDegreesToRadians(yaw)));
    }

    //press D
    if(movement == QVector3D(1,0,0))
    {
        moveVec += QVector3D(cosf(qDegreesToRadians(yaw)),
                             0.0f,
                             -sinf(qDegreesToRadians(yaw)));
    }


    position += moveVec;
}

void Camera::Rotate(float x, float y)
{
    yaw -= x;
    pitch -= y;
}
