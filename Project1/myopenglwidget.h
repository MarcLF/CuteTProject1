#ifndef MYOPENGLWIDGET_H
#define MYOPENGLWIDGET_H

#define PI 3.14159265358979323846f
#define RADTODEG(angle_in_radiants) (angle_in_radiants*180.0f)/PI
#define DEGTORAD(angle_in_degrees) (angle_in_degrees*PI)/180.0f

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include<QtOpenGL/QtOpenGL>

#include "qopengl.h"


class Camera;
class Mesh;

enum RendererMode
{
    Albedo = 0,
    Lighting,
    Normals,
    DepthTest
};

class MyOpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:
    explicit MyOpenGLWidget(QWidget *parent = nullptr);
    ~MyOpenGLWidget()override;

    void initializeGL() override;
    void resizeGL(int width, int height) override;
    void paintGL() override;

    void InitBuffers();

    void SetRendererDisplay(int mode);

    void SetLightDirection(QVector3D lightDir);
public:
    QOpenGLShaderProgram program;

    QOpenGLShaderProgram blurProgram;
    QOpenGLBuffer vboblur;
    QOpenGLBuffer vaoblur;

private:
    Camera* mainCamera = nullptr;

    float FOV = 90.0f;
    float aspectRatio = 0.0f;
    float nearPlane = 0.1f;
    float farPlane = 150.0f;

    GLuint colorTexture;
    GLuint depthTexture;
    GLuint normalTexture;
    GLuint fbo;
    GLuint partialBlurTexture;
    GLuint partialBlurfbo;
    GLuint completeBlurTexture;
    GLuint completeBlurFbo;

    QTimer* updateTimer;

    float mouseX = 0;
    float mouseY = 0;
    float motionX = 0;
    float motionY = 0;
    bool isRotating = false;

    RendererMode mode = RendererMode::Lighting;

    Mesh *quadMesh = nullptr;
private:
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;

    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

    float xLightDir = 0;
    float yLightDir = 1;
    float zLightDir = 0;

    QOpenGLTexture *textureForBlur = nullptr;

signals:

public slots:
    void myUpdate();

};

extern QOpenGLFunctions_3_3_Core *gl;

#endif // MYOPENGLWIDGET_H
