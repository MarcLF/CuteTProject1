#include "dofoptions.h"
#include "ui_dofoptions.h"
#include "myopenglwidget.h"

DoFOptions::DoFOptions(MyOpenGLWidget* openGLPointer ,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DoFOptions)
{
    ui->setupUi(this);

    openGLWidgetPointer = openGLPointer;

    ui->nearPlane->setValue(openGLWidgetPointer->GetNearPlane());
    ui->farPlane->setValue(openGLWidgetPointer->GetFarPlane());
    ui->nearFallOff->setValue(openGLWidgetPointer->GetNearFallOff());
    ui->farFallOff->setValue(openGLWidgetPointer->GetFarFallOff());

    connect(ui->nearPlane, SIGNAL(valueChanged(double)), this, SLOT(OnChangeNearPlane()));
    connect(ui->farPlane, SIGNAL(valueChanged(double)), this, SLOT(OnChangeFarPlane()));
    connect(ui->nearFallOff, SIGNAL(valueChanged(double)), this, SLOT(OnChangeNearFallOff()));
    connect(ui->farFallOff, SIGNAL(valueChanged(double)), this, SLOT(OnChangeFarFallOff()));
}

DoFOptions::~DoFOptions()
{
    delete ui;
}

void DoFOptions::OnChangeNearPlane()
{
    openGLWidgetPointer->SetNearPlane(float(ui->nearPlane->value()));
}

void DoFOptions::OnChangeFarPlane()
{
    openGLWidgetPointer->SetFarPlane(float(ui->farPlane->value()));
}

void DoFOptions::OnChangeNearFallOff()
{
    openGLWidgetPointer->SetNearFallOff(float(ui->nearFallOff->value()));
}

void DoFOptions::OnChangeFarFallOff()
{
    openGLWidgetPointer->SetFarFallOff(float(ui->farFallOff->value()));
}
