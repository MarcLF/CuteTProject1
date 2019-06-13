#ifndef DOFOPTIONS_H
#define DOFOPTIONS_H

#include <QWidget>

namespace Ui
{
    class DoFOptions;
}
class MyOpenGLWidget;

class DoFOptions : public QWidget
{
    Q_OBJECT

public:
    explicit DoFOptions(MyOpenGLWidget* pointer,QWidget *parent = nullptr);
    ~DoFOptions();


private:
    Ui::DoFOptions *ui;


    MyOpenGLWidget* openGLWidgetPointer;

signals:

public slots:
    void OnChangeFarPlane();
    void OnChangeNearPlane();
    void OnChangeFarFallOff();
    void OnChangeNearFallOff();
};

#endif // DOFOPTIONS_H
