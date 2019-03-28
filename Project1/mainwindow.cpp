#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_rendering.h"
#include "inspector.h"
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    inspector = new Inspector();
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
}

void MainWindow::saveProject()
{
    std::cout << "Save project" << std::endl;
}
