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
    ui->ZTranslate->setValue(posZ);
    //Rotation
    ui->XRotate->setValue(rotX);
    ui->YRotate->setValue(rotY);
    ui->ZRotate->setValue(rotZ);
    //Scale
    ui->XScale->setValue(scaleX);
    ui->YScale->setValue(scaleY);
    ui->ZScale->setValue(scaleZ);


    connect(ui->XTranslate, SIGNAL(valueChanged(double)), this, SLOT(modifyXPos(double)));
    connect(ui->YTranslate, SIGNAL(valueChanged(double)), this, SLOT(modifyYPos(double)));
    connect(ui->ZTranslate, SIGNAL(valueChanged(double)), this, SLOT(modifyZPos(double)));

    connect(ui->XRotate, SIGNAL(valueChanged(double)), this, SLOT(modifyXRot(double)));
    connect(ui->YRotate, SIGNAL(valueChanged(double)), this, SLOT(modifyYRot(double)));
    connect(ui->ZRotate, SIGNAL(valueChanged(double)), this, SLOT(modifyZRot(double)));

    connect(ui->XScale, SIGNAL(valueChanged(double)), this, SLOT(modifyXScale(double)));
    connect(ui->YScale, SIGNAL(valueChanged(double)), this, SLOT(modifyYScale(double)));
    connect(ui->ZScale, SIGNAL(valueChanged(double)), this, SLOT(modifyZScale(double)));
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
    ui->ZScale->setValue(posZ);
    //Rotation
    ui->XRotate->setValue(rotX);
    ui->YRotate->setValue(rotY);
    ui->ZRotate->setValue(rotZ);
    //Scale
    ui->XScale->setValue(scaleX);
    ui->YScale->setValue(scaleY);
    ui->ZScale->setValue(scaleZ);
}

void ComponentTransform::modifyXPos(double value)
{
    posX = value;
}

void ComponentTransform::modifyYPos(double value)
{
    posY = value;
}

void ComponentTransform::modifyZPos(double value)
{
    posZ = value;
}

void ComponentTransform::modifyXRot(double value)
{
    rotX = value;
}

void ComponentTransform::modifyYRot(double value)
{
    rotY = value;
}

void ComponentTransform::modifyZRot(double value)
{
    rotZ = value;
}

void ComponentTransform::modifyXScale(double value)
{
    scaleX = value;
}

void ComponentTransform::modifyYScale(double value)
{
    scaleY = value;
}

void ComponentTransform::modifyZScale(double value)
{
    scaleZ = value;
}


float ComponentTransform::GetPosX()
{
    return posX;
}

float ComponentTransform::GetPosY()
{
    return posY;
}

float ComponentTransform::GetPosZ()
{
    return posZ;
}

float ComponentTransform::GetRotX()
{
    return rotX;
}

float ComponentTransform::GetRotY()
{
    return rotY;
}

float ComponentTransform::GetRotZ()
{
    return rotZ;
}

float ComponentTransform::GetScaleX()
{
    return scaleX;
}

float ComponentTransform::GetScaleY()
{
    return scaleY;
}

float ComponentTransform::GetScaleZ()
{
    return scaleZ;
}


