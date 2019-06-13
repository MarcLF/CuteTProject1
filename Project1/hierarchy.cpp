#include "hierarchy.h"
#include "ui_hierarchy.h"
#include "entity.h"
#include "mainwindow.h"
#include "inspector.h"
#include "componenttransform.h"
#include "shaperenderer.h"
#include "componentrender.h"

#include <iostream>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QString>
#include <QDebug>
#include <QJsonValueRef>

Hierarchy::Hierarchy(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Hierarchy)
{
    ui->setupUi(this);

    connect(ui->AddEntity, SIGNAL(clicked()), this, SLOT(AddEntity()));
    connect(ui->RemoveEntity, SIGNAL(clicked()), this, SLOT(RemoveEntity()));
    connect(ui->EntityList, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(SelectEntity(QListWidgetItem*)));
}

Hierarchy::~Hierarchy()
{
    while(!entities.empty())
    {
        delete entities.back();
        entities.pop_back();
    }
    delete ui;
}

void Hierarchy::ChangeItemName(Entity *entity, QString name)
{
    for(int i = 0; i < entities.size(); i++)
    {
        if(entities[i] == entity)
        {
            ui->EntityList->item(i)->setText(name);
        }
    }
}

void Hierarchy::UpdateComboBoxes(QString newItem)
{
    for(int i = 0; i < entities.size(); i++)
    {
        ComponentRender* renderComp = (ComponentRender*)entities[i]->GetComponent(Component_Render);
        if(renderComp != nullptr)
        {
            renderComp->AddModelToComboBox(newItem);
        }
    }
}

void Hierarchy::AddEntity()
{
    Entity* newEntity = new Entity(entityID++);

    entities.push_back(newEntity);
    ui->EntityList->addItem(newEntity->GetName().c_str());
}

Entity* Hierarchy::AddEntityWithObj(QString fileName)
{
    Entity* newEntity = new Entity(entityID++);

    entities.push_back(newEntity);
    ui->EntityList->addItem(newEntity->GetName().c_str());
    newEntity->LoadObjModel(fileName);
    return newEntity;
}

Entity* Hierarchy::AddEntityWithMesh(Mesh *toAdd)
{
    Entity* newEntity = new Entity(entityID++);

    entities.push_back(newEntity);
    ui->EntityList->addItem(newEntity->GetName().c_str());

    return newEntity;
}

void Hierarchy::RemoveEntity()
{
    if(selectedItem != nullptr)
    {
        ui->EntityList->removeItemWidget(selectedItem);
        delete selectedItem;
        selectedItem = nullptr;
    }
    if(selected != nullptr)
    {
        entities.erase(entities.begin() + selectedPos);
        delete selected;
        selected = nullptr;
        MainWindow::GetWindow()->GetInspector()->SetNewEntity(nullptr);
    }
}

void Hierarchy::SelectEntity(QListWidgetItem* item)
{
    selectedPos = 0;
    selectedItem = item;
    for(int i = 0; i < ui->EntityList->count(); i++)
    {
        if(ui->EntityList->item(i) == item)
        {
            selected = entities.at(selectedPos);
            break;
        }
        else
        {
           selectedPos++;
        }
    }

    MainWindow::GetWindow()->GetInspector()->SetNewEntity(selected);
}

std::vector<Entity*> Hierarchy::GetEntityList()
{
    return entities;
}
