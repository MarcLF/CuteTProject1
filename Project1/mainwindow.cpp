#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_rendering.h"
#include "inspector.h"
#include "hierarchy.h"
#include "myopenglwidget.h"
#include "lightdirection.h"
#include "dofoptions.h"

#include <QFile>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <iostream>

MainWindow *MainWindow::window;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    window = this;

    hierarchy = new Hierarchy(this);
    ui->Hierarchywidget->setWidget(hierarchy);

    inspector = new Inspector(this);
    ui->Inspectorwidget->setWidget(inspector);

    openGLWidget = new MyOpenGLWidget(this);
    ui->Scene->setWidget(openGLWidget);

    lightDirection = new LightDirection(openGLWidget);

    lightColorPicker = new QColorDialog();

    connect(ui->actionLightColor, SIGNAL(triggered()), this, SLOT(onLightColor()));
    connect(lightColorPicker, SIGNAL(accepted()), this, SLOT(changeLightColor()));

    dofOptions = new DoFOptions(openGLWidget);

    connect(ui->actionOpen_Project, SIGNAL(triggered()), this, SLOT(openProject()));
    connect(ui->actionSave_Project, SIGNAL(triggered()), this, SLOT(saveProject()));
    connect(ui->actionQuit, SIGNAL(triggered()), qApp, SLOT(quit()));

    connect(ui->actionAlbedo, SIGNAL(triggered()), this, SLOT(ChangeToAlbedo()));
    connect(ui->actionLighting, SIGNAL(triggered()), this, SLOT(ChangeToLighting()));
    connect(ui->actionNormals, SIGNAL(triggered()), this, SLOT(ChangeToNormals()));
    connect(ui->actionDepth_test, SIGNAL(triggered()), this, SLOT(ChangeToDepthTest()));
    connect(ui->actionBlur, SIGNAL(triggered()), this, SLOT(SwitchBlur()));
    connect(ui->actionDepth_of_Field, SIGNAL(triggered()), this, SLOT(SwitchDoF()));

    connect(ui->actionLight_Settings, SIGNAL(triggered()), this, SLOT(OpenLightSettings()));
    connect(ui->actionOptions, SIGNAL(triggered()), this, SLOT(OpenDoFSettings()));

    setAcceptDrops(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openProject()
{
    hierarchy->loadEntities("save.json");
}

void MainWindow::saveProject()
{
    QFile saveFile("save.json");

    hierarchy->saveEntities(saveFile);
}

void MainWindow::ChangeToAlbedo()
{
    openGLWidget->SetRendererDisplay(0);
}

void MainWindow::ChangeToLighting()
{
    openGLWidget->SetRendererDisplay(1);
}

void MainWindow::ChangeToNormals()
{
    openGLWidget->SetRendererDisplay(2);
}

void MainWindow::ChangeToDepthTest()
{
    openGLWidget->SetRendererDisplay(3);
}

void MainWindow::OpenLightSettings()
{
    lightDirection->show();
}

void MainWindow::OpenDoFSettings()
{
    dofOptions->show();
}

void MainWindow::onLightColor()
{
    lightColorPicker->show();
}

void MainWindow::changeLightColor()
{
    openGLWidget->SetLightColor(QVector3D(lightColorPicker->currentColor().red()/255.0f, lightColorPicker->currentColor().green()/255.0f, lightColorPicker->currentColor().blue()/255.0f));
}

void MainWindow::SwitchBlur()
{
    openGLWidget->SwitchBlur();
}

void MainWindow::SwitchDoF()
{
    openGLWidget->SwitchDoF();
}

Inspector *MainWindow::GetInspector()
{
    return inspector;
}

Hierarchy *MainWindow::GetHierarchy()
{
    return hierarchy;
}

MainWindow * MainWindow::GetWindow()
{
    return window;
}

void MainWindow::dragEnterEvent(QDragEnterEvent *e)
{
    if (e->mimeData()->hasUrls()) {
        e->acceptProposedAction();
    }
}

void MainWindow::dropEvent(QDropEvent *e)
{
    foreach (const QUrl &url, e->mimeData()->urls()) {
        QString fileName = url.toLocalFile();
        QString extension = fileName.section('.', -1);

        if(extension == "obj")
        {
         hierarchy->AddEntityWithObj(fileName);
        }
    }
}


