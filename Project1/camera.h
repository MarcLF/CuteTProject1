#ifndef CAMERA_H
#define CAMERA_H

#include <QVector3D>
#include <QMatrix4x4>

class Camera
{
public:
    Camera(int width, int height);

    QVector4D GetLeftRightBottomTop();

    QVector3D ScreenPointToWorldRay(int x, int y);
    QVector3D ScreenDisplacementToWorldVector(int x0, int y0, int x1, int y1);
    QVector2D WorldToScreenPoint(const QVector3D &p);

    void PrepareMatrices();

public:
    int viewportWidth;
    int viewportHeight;

    float fov;
    float zNear;
    float zFar;
    float yaw;
    float pitch;
    float speed;

    QVector3D position;
    QVector3D targetPosition;

    QMatrix4x4 worldMatrix;
    QMatrix4x4 viewMatrix;
    QMatrix4x4 projectionMatrix;
};

#endif // CAMERA_H
