#ifndef MYOPENGLWIDGET_H
#define MYOPENGLWIDGET_H

#define PI 3.14159265358979323846f
#define RADTODEG(angle_in_radiants) (angle_in_radiants*180.0f)/PI
#define DEGTORAD(angle_in_degrees) (angle_in_degrees*PI)/180.0f

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>

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

public:
    QOpenGLShaderProgram program;

private:
    Camera* mainCamera = nullptr;

    float FOV = 90.0f;
    float aspectRatio = 0.0f;
    float nearPlane = 0.1f;
    float farPlane = 150.0f;

        QTimer* updateTimer;


signals:

public slots:
    void myUpdate();

};

#endif // MYOPENGLWIDGET_H
