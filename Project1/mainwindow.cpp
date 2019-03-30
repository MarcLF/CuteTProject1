#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_rendering.h"
#include "inspector.h"
#include "hierarchy.h"

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

    connect(ui->actionOpen_Project, SIGNAL(triggered()), this, SLOT(openProject()));
    connect(ui->actionSave_Project, SIGNAL(triggered()), this, SLOT(saveProject()));
    connect(ui->actionGFO, SIGNAL(triggered()), qApp, SLOT(quit()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openProject()
{
    std::cout << "Open project" << std::endl;   

    hierarchy->loadEntities("save.json");

}

void MainWindow::saveProject()
{
    std::cout << "Save project" << std::endl;

    QFile saveFile("save.json");

    hierarchy->saveEntities(saveFile);

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
