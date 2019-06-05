#ifndef LIGHTDIRECTION_H
#define LIGHTDIRECTION_H

#include <QWidget>

namespace Ui {
class LightDirection;
}
class MyOpenGLWidget;

class LightDirection : public QWidget
{
    Q_OBJECT

public:
    explicit LightDirection(MyOpenGLWidget* pointer,QWidget *parent = nullptr);
    ~LightDirection();


private:
    Ui::LightDirection *ui;


    MyOpenGLWidget* openGLWidgetPointer;

signals:

public slots:
    void OnChangeX();
    void OnChangeY();
    void OnChangeZ();
};

#endif // LIGHTDIRECTION_H
