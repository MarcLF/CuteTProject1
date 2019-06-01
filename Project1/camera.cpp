#include "camera.h"

#include <qvector4d.h>
#include <qmath.h>
#include <iostream>

Camera::Camera(int width, int height)
{
    viewportWidth = width;
    viewportHeight = height;

     viewportWidth = 128;
     viewportHeight = 128;

    fov = 90.0f;
    zNear = 0.1f;
    zFar = 150.0f;
    yaw = 0.0f;
    pitch = 0.0f;
    speed = 0.5f;

    worldMatrix = QMatrix4x4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);
    viewMatrix =  QMatrix4x4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);
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
