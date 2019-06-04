#ifndef MYOPENGLWIDGET_H
#define MYOPENGLWIDGET_H

#define PI 3.14159265358979323846f
#define RADTODEG(angle_in_radiants) (angle_in_radiants*180.0f)/PI
#define DEGTORAD(angle_in_degrees) (angle_in_degrees*PI)/180.0f

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>

#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLFramebufferObject>
#include<QtOpenGL/QtOpenGL>

class Camera;

class MyOpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:
    explicit MyOpenGLWidget(QWidget *parent = nullptr);
    ~MyOpenGLWidget()override;

    void initializeGL() override;
    void resizeGL(int width, int height) override;
    void paintGL() override;
    void ChargeTextureToProgram(QOpenGLTexture text);
    void InitBuffers();

public:
    QOpenGLShaderProgram program;

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

    QTimer* updateTimer;

    float mouseX = 0;
    float mouseY = 0;
    float motionX = 0;
    float motionY = 0;
    bool isRotating = false;

private:
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;

    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;


signals:

public slots:
    void myUpdate();

};

extern QOpenGLFunctions_3_3_Core *gl;

#endif // MYOPENGLWIDGET_H
