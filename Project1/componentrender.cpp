#include "componentrender.h"
#include "ui_componentrender.h"
#include "mesh.h"
#include "mainwindow.h"

#define H 32
#define V 16

ComponentRender::ComponentRender(QWidget *parent) :
    Component (ComponentType::Component_Render, parent),
    ui(new Ui::ComponentRender)
{
    ui->setupUi(this);

    connect(ui->modelSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(ChangeModel(int)));
}

ComponentRender::~ComponentRender()
{
    delete ui;
}

void ComponentRender::AddMesh(const char *fileName)
{

    myMesh = new Mesh();
    myMesh->loadModel(fileName);
    QString modelName = QString(fileName).section('/', -1);
    MainWindow::GetWindow()->GetResources()->AddModel(myMesh, modelName);

}

void ComponentRender::AddMesh(Mesh *toAdd)
{
    myMesh = toAdd;
}

void ComponentRender::Draw()
{
    if(myMesh != nullptr)
    {
        myMesh->draw();
    }
}

void ComponentRender::WorkMeshes()
{
    if(myMesh != nullptr)
    {
        myMesh->update();
    }
}

void ComponentRender::AddModelToComboBox(QString modelName, bool isMine)
{
    ui->modelSelector->insertItem(10000, modelName);
    if(isMine)
    {
        ui->modelSelector->setCurrentText(modelName);
    }
}

void ComponentRender::ChangeModel(int index)
{
    if(myMesh != nullptr)
    {
        myMesh = nullptr;
    }
    if(index != 0)
    {
        myMesh = MainWindow::GetWindow()->GetResources()->GetModel(ui->modelSelector->currentText());
    }
}
