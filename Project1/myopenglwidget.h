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
    void GenerateQuad();

    void SetRendererDisplay(int mode);

    void SetLightDirection(QVector3D lightDir);
    void SetLightColor(QVector3D lightColor);

    void SetNearPlane(float nearPL);
    void SetFarPlane(float farPL);
    void SetNearFallOff(float nearFO);
    void SetFarFallOff(float farFO);

    float GetNearPlane();
    float GetFarPlane();
    float GetNearFallOff();
    float GetFarFallOff();

    void SwitchBlur();
    void SwitchDoF();

    void blurShader();
    void dofShader();

public:
    QOpenGLShaderProgram program;
    QOpenGLShaderProgram quadProgram;
    QOpenGLShaderProgram blurProgram;
    QOpenGLShaderProgram depthProgram;

    QOpenGLBuffer vboQuadBlur;
    QOpenGLVertexArrayObject vaoQuadBlur;

private:
    Camera* mainCamera = nullptr;

    float FOV = 90.0f;
    float aspectRatio = 0.0f;
    float nearPlane = 0.1f;
    float farPlane = 150.0f;

    GLuint colorTexture;
    GLuint depthTexture;
    GLuint normalTexture;
    GLuint posTexture;
    GLuint fbo;
    GLuint partialBlurTexture;
    GLuint BlurTexture;
    GLuint dofTexture;
    GLuint partialBlurFbo;
    GLuint blurFbo;
    GLuint dofFbo;

    QOpenGLBuffer vbo;
    QOpenGLVertexArrayObject vao;

    QTimer* updateTimer;

    float mouseX = 0;
    float mouseY = 0;
    float motionX = 0;
    float motionY = 0;
    bool isRotating = false;

    RendererMode mode = RendererMode::Lighting;
    bool blurIsOn = false;
    bool dofIsOn = false;

    Mesh *quadMesh = nullptr;

    int screenWidth, screenHeight;
private:
    void keyPressEvent(QKeyEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

    float xLightDir = 0;
    float yLightDir = 1;
    float zLightDir = 0;

    float nearPlaneValue = 0.83f;
    float farPlaneValue = 0.92f;
    float nearFallOffValue = 0.86f;
    float farFallOffValue = 0.9f;

    QVector3D lightColor;

signals:

public slots:
    void myUpdate();
};

extern QOpenGLFunctions_3_3_Core *gl;

#endif // MYOPENGLWIDGET_H
