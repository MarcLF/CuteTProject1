#include "lightdirection.h"
#include "ui_lightdirection.h"
#include "myopenglwidget.h"

LightDirection::LightDirection(MyOpenGLWidget* openGLPointer ,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LightDirection)
{
    ui->setupUi(this);

    openGLWidgetPointer = openGLPointer;

    connect(ui->XDirLight, SIGNAL(valueChanged(double)), this, SLOT(OnChangeX()));
    connect(ui->YDirLight, SIGNAL(valueChanged(double)), this, SLOT(OnChangeY()));
    connect(ui->ZDirLight, SIGNAL(valueChanged(double)), this, SLOT(OnChangeZ()));

}

LightDirection::~LightDirection()
{
    delete ui;
}

void LightDirection::OnChangeX()
{
    openGLWidgetPointer->SetLightDirection(QVector3D(ui->XDirLight->value(), ui->YDirLight->value(), ui->ZDirLight->value()));
}

void LightDirection::OnChangeY()
{
    openGLWidgetPointer->SetLightDirection(QVector3D(ui->XDirLight->value(), ui->YDirLight->value(), ui->ZDirLight->value()));
}

void LightDirection::OnChangeZ()
{
    openGLWidgetPointer->SetLightDirection(QVector3D(ui->XDirLight->value(), ui->YDirLight->value(), ui->ZDirLight->value()));
}
