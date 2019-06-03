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
    transformMatrix.setToIdentity();

    transformMatrix.translate(posX, posY, posZ);
    transformMatrix.rotate(rotX, QVector3D(1, 0, 0));
    transformMatrix.rotate(rotY, QVector3D(0, 1, 0));
    transformMatrix.rotate(rotZ, QVector3D(0, 0, 1));
    transformMatrix.scale(scaleX, scaleY, scaleZ);
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

QMatrix4x4 ComponentTransform::GetTransMatrix()
{
    Update();
    return transformMatrix;
}

void ComponentTransform::modifyXPos(double value)
{
    posX = value;
    Update();
}

void ComponentTransform::modifyYPos(double value)
{
    posY = value;
    Update();
}

void ComponentTransform::modifyZPos(double value)
{
    posZ = value;
    Update();
}

void ComponentTransform::modifyXRot(double value)
{
    rotX = value;
    Update();
}

void ComponentTransform::modifyYRot(double value)
{
    rotY = value;
    Update();
}

void ComponentTransform::modifyZRot(double value)
{
    rotZ = value;
    Update();
}

void ComponentTransform::modifyXScale(double value)
{
    scaleX = value;
    Update();
}

void ComponentTransform::modifyYScale(double value)
{
    scaleY = value;
    Update();
}

void ComponentTransform::modifyZScale(double value)
{
    scaleZ = value;
    Update();
}


float ComponentTransform::GetPosX()
{
    return posX;
    Update();
}

float ComponentTransform::GetPosY()
{
    return posY;
    Update();
}

float ComponentTransform::GetPosZ()
{
    return posZ;
    Update();
}

float ComponentTransform::GetRotX()
{
    return rotX;
    Update();
}

float ComponentTransform::GetRotY()
{
    return rotY;
    Update();
}

float ComponentTransform::GetRotZ()
{
    return rotZ;
    Update();
}

float ComponentTransform::GetScaleX()
{
    return scaleX;
    Update();
}

float ComponentTransform::GetScaleY()
{
    return scaleY;
    Update();
}

float ComponentTransform::GetScaleZ()
{
    return scaleZ;
    Update();
}


