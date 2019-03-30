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

    inspector = new Inspector(this);
    ui->Inspectorwidget->setWidget(inspector);

    hierarchy = new Hierarchy(this);
    ui->Hierarchywidget->setWidget(hierarchy);


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

MainWindow * MainWindow::GetWindow()
{
    return window;
}
