#include "componenttransform.h"
#include "ui_componenttransform.h"

ComponentTransform::ComponentTransform(QWidget *parent) :
    Component(ComponentType::Component_Transform, parent),
    ui(new Ui::ComponentTransform)
{
    ui->setupUi(this);

    //Position
    ui->XTranslate->setValue(posX);
    ui->YTranslate->setValue(posY);
    //Rotation
    ui->XRotate->setValue(rotX);
    ui->YRotate->setValue(rotY);
    //Scale
    ui->XScale->setValue(scaleX);
    ui->YScale->setValue(scaleY);


    connect(ui->XTranslate, SIGNAL(valueChanged(double)), this, SLOT(modifyXPos(double)));
    connect(ui->YTranslate, SIGNAL(valueChanged(double)), this, SLOT(modifyYPos(double)));

    connect(ui->XRotate, SIGNAL(valueChanged(double)), this, SLOT(modifyXRot(double)));
    connect(ui->YRotate, SIGNAL(valueChanged(double)), this, SLOT(modifyYRot(double)));

    connect(ui->XScale, SIGNAL(valueChanged(double)), this, SLOT(modifyXScale(double)));
    connect(ui->YScale, SIGNAL(valueChanged(double)), this, SLOT(modifyYScale(double)));
}

ComponentTransform::~ComponentTransform()
{
    delete ui;
}

void ComponentTransform::setValues()
{
    //Position
    ui->XTranslate->setValue(posX);
    ui->YTranslate->setValue(posY);
    //Rotation
    ui->XRotate->setValue(rotX);
    ui->YRotate->setValue(rotY);
    //Scale
    ui->XScale->setValue(scaleX);
    ui->YScale->setValue(scaleY);
}

void ComponentTransform::modifyXPos(double value)
{
    posX = value;
}

void ComponentTransform::modifyYPos(double value)
{
    posY = value;
}

void ComponentTransform::modifyXRot(double value)
{
    rotX = value;
}

void ComponentTransform::modifyYRot(double value)
{
    rotY = value;
}

void ComponentTransform::modifyXScale(double value)
{
    scaleX = value;
}

void ComponentTransform::modifyYScale(double value)
{
    scaleY = value;
}

float ComponentTransform::GetPosX()
{
    return posX;
}

float ComponentTransform::GetPosY()
{
    return posY;
}

float ComponentTransform::GetRotX()
{
    return rotX;
}

float ComponentTransform::GetRotY()
{
    return rotY;
}

float ComponentTransform::GetScaleX()
{
    return scaleX;
}

float ComponentTransform::GetScaleY()
{
    return scaleY;
}


