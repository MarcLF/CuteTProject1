#ifndef COMPONENTTRANSFORM_H
#define COMPONENTTRANSFORM_H

#include <QWidget>
#include "component.h"

namespace Ui {
class ComponentTransform;
}

class ComponentTransform : public Component
{
    Q_OBJECT

public:
    explicit ComponentTransform(QWidget *parent = nullptr);
    ~ComponentTransform();

    void setValues();

    float GetPosX();
    float GetPosY();
    float GetPosZ();

    float GetRotX();
    float GetRotY();
    float GetRotZ();

    float GetScaleX();
    float GetScaleY();
    float GetScaleZ();

public slots:

    void modifyXPos(double value);
    void modifyYPos(double value);
    void modifyZPos(double value);

    void modifyXRot(double value);
    void modifyYRot(double value);
    void modifyZRot(double value);

    void modifyXScale(double value);
    void modifyYScale(double value);
    void modifyZScale(double value);

private:
    Ui::ComponentTransform *ui;
    float posX = 0, posY = 0, posZ = 0;
    float rotX = 0, rotY = 0, rotZ = 0;
    float scaleX = 1, scaleY = 1, scaleZ = 1;
};

#endif // COMPONENTTRANSFORM_H
