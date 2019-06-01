#include "componenttransform.h"
#include "ui_componenttransform.h"

#include <QVector3D>

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

void ComponentTransform::Update()
{
    if(updateTrans == true)
    {
        transformMatrix.setToIdentity();

        transformMatrix.translate(posX, posY, posZ);
        transformMatrix.rotate(rotX, QVector3D(1, 0, 0));
        transformMatrix.rotate(rotY, QVector3D(0, 1, 0));
        transformMatrix.rotate(rotZ, QVector3D(0, 0, 1));
        transformMatrix.scale(scaleX, scaleY, scaleZ);

        updateTrans = false;
    }
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

const QMatrix4x4* ComponentTransform::GetTransMatrix() const
{
    return &transformMatrix;
}

void ComponentTransform::modifyXPos(double value)
{
    posX = value;
    updateTrans = true;
}

void ComponentTransform::modifyYPos(double value)
{
    posY = value;
    updateTrans = true;
}

void ComponentTransform::modifyZPos(double value)
{
    posZ = value;
    updateTrans = true;
}

void ComponentTransform::modifyXRot(double value)
{
    rotX = value;
    updateTrans = true;
}

void ComponentTransform::modifyYRot(double value)
{
    rotY = value;
    updateTrans = true;
}

void ComponentTransform::modifyZRot(double value)
{
    rotZ = value;
    updateTrans = true;
}

void ComponentTransform::modifyXScale(double value)
{
    scaleX = value;
    updateTrans = true;
}

void ComponentTransform::modifyYScale(double value)
{
    scaleY = value;
    updateTrans = true;
}

void ComponentTransform::modifyZScale(double value)
{
    scaleZ = value;
    updateTrans = true;
}


float ComponentTransform::GetPosX()
{
    return posX;
    updateTrans = true;
}

float ComponentTransform::GetPosY()
{
    return posY;
    updateTrans = true;
}

float ComponentTransform::GetPosZ()
{
    return posZ;
    updateTrans = true;
}

float ComponentTransform::GetRotX()
{
    return rotX;
    updateTrans = true;
}

float ComponentTransform::GetRotY()
{
    return rotY;
    updateTrans = true;
}

float ComponentTransform::GetRotZ()
{
    return rotZ;
    updateTrans = true;
}

float ComponentTransform::GetScaleX()
{
    return scaleX;
    updateTrans = true;
}

float ComponentTransform::GetScaleY()
{
    return scaleY;
    updateTrans = true;
}

float ComponentTransform::GetScaleZ()
{
    return scaleZ;
    updateTrans = true;
}


