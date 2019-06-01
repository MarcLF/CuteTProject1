#ifndef MYSUPERWIDGET_H
#define MYSUPERWIDGET_H

#define PI 3.14159265358979323846f
#define RADTODEG(angle_in_radiants) (angle_in_radiants*180.0f)/PI
#define DEGTORAD(angle_in_degrees) (angle_in_degrees*PI)/180.0f

#include <QWidget>
#include "camera.h"

class MySuperWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MySuperWidget(QWidget *parent = nullptr);

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

signals:

public slots:
    void paintEvent(QPaintEvent *event) override;
    void myUpdate();

private:
    QTimer* updateTimer;

    Camera* mainCamera = nullptr;

    float aspect_ratio = 0.0f;
    float field_of_view = 90.0f; //radiants
    float near_plane_distance = 0.1f;
    float far_plane_distance = 150.0f;
};

#endif // MYSUPERWIDGET_H
