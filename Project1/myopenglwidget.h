#ifndef MYOPENGLWIDGET_H
#define MYOPENGLWIDGET_H

#define PI 3.14159265358979323846f
#define RADTODEG(angle_in_radiants) (angle_in_radiants*180.0f)/PI
#define DEGTORAD(angle_in_degrees) (angle_in_degrees*PI)/180.0f

#include <QOpenGLWidget>

class MyOpenGLWidget : public QOpenGLWidget
{
    Q_OBJECT
public:
    explicit MyOpenGLWidget(QWidget *parent = nullptr);
    ~MyOpenGLWidget()override;

    void initializeGL() override;
    void resizeGL(int width, int height) override;
    void paintGL() override;

signals:

public slots:

private:
    float aspect_ratio = 0.0f;
    float field_of_view = 90.0f; //radiants
    float near_plane_distance = 0.1f;
    float far_plane_distance = 150.0f;
};

#endif // MYOPENGLWIDGET_H
