#include "hierarchy.h"
#include "ui_hierarchy.h"
#include "entity.h"
#include "mainwindow.h"
#include "inspector.h"
#include "componenttransform.h"
#include "shaperenderer.h"

#include <iostream>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QString>

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

void Hierarchy::saveEntities(QFile &saveFile)
{
    QJsonArray dataArray;
    QJsonArray compArray;
    QJsonArray emptyArray;

    QJsonArray componentNameArray;
    QJsonArray transfDataArray;

    QJsonObject entNameObj;
    QJsonObject emptyObj;

    QJsonObject entCompNameObj;
    QJsonObject entTransfDataObj;


    for(uint i = 0; i < entities.size(); i++)
    {
        std::vector<Component*> components = entities[i]->GetComponents();

        for(uint j = 0; j < components.size(); j++)
        {
            if(components[j]->GetType() == (ComponentType)Component_Transform)
            {
                ComponentTransform *componentTrans = static_cast<ComponentTransform*>(components[j]);
                QJsonObject TransformComponent;

                TransformComponent["posX"] = componentTrans->GetPosX();
                TransformComponent["posY"] = componentTrans->GetPosY();

                TransformComponent["rotX"] = componentTrans->GetRotX();
                TransformComponent["rotY"] = componentTrans->GetRotY();

                TransformComponent["scaleX"] = componentTrans->GetScaleX();
                TransformComponent["scaleY"] = componentTrans->GetScaleY();

                transfDataArray.append(TransformComponent);
            }
            else if(components[j]->GetType() == (ComponentType)Component_ShapeRenderer)
            {
                ComponentShapeRenderer *componentShapeRenderer = static_cast<ComponentShapeRenderer*>(components[j]);
                QJsonObject ShapeRendererComp;

                ShapeRendererComp["Shape Index"] = componentShapeRenderer->GetShapeIndex();
                ShapeRendererComp["Shape Size"] = componentShapeRenderer->GetShapeSize();

                ShapeRendererComp["Fill Color Red Param"] = componentShapeRenderer->GetFillColor().red();
                ShapeRendererComp["Fill Color Green Param"] = componentShapeRenderer->GetFillColor().green();
                ShapeRendererComp["Fill Color Blue Param"] = componentShapeRenderer->GetFillColor().blue();

                ShapeRendererComp["Stroke Color Red Param"] = componentShapeRenderer->GetStrokeColor().red();
                ShapeRendererComp["Stroke Color Green Param"] = componentShapeRenderer->GetStrokeColor().green();
                ShapeRendererComp["Stroke Color Blue Param"] = componentShapeRenderer->GetStrokeColor().blue();
                ShapeRendererComp["Stroke Thickness"] = componentShapeRenderer->GetStrokeThickness();
                ShapeRendererComp["Stroke Style Index"] = componentShapeRenderer->GetStrokeStyleIndex();

                transfDataArray.append(ShapeRendererComp);
            }

            entTransfDataObj.insert(QString(entities[i]->GetComponents()[j]->GetName().c_str()), QJsonValue(transfDataArray));
            componentNameArray.push_back((entTransfDataObj));

            transfDataArray = emptyArray;
            entTransfDataObj = emptyObj;
        }

        entCompNameObj.insert(QString(entities[i]->GetName().c_str()), QJsonValue(componentNameArray));
        dataArray.push_back((entCompNameObj));

        componentNameArray = emptyArray;
        entCompNameObj = emptyObj;
    }

    entNameObj.insert(QString("EntitiesData"), QJsonValue(dataArray));

    QJsonDocument saveDocEnt(entNameObj);
    saveFile.open(QIODevice::WriteOnly | QIODevice::Text);
    saveFile.write(saveDocEnt.toJson());
}

void Hierarchy::AddEntity()
{
    Entity* newEntity = new Entity(entityID++);

    entities.push_back(newEntity);
    ui->EntityList->addItem(newEntity->GetName().c_str());
}

void Hierarchy::RemoveEntity()
{
    if(selectedItem != nullptr)
    {
        ui->EntityList->removeItemWidget(selectedItem);
        delete selectedItem;
        selectedItem = nullptr;
        std::cout << "item removed" << std::endl;
    }
    if(selected != nullptr)
    {
        std::cout << "Deleting from pos " + std::to_string(selectedPos) << std::endl;
        entities.erase(entities.begin() + selectedPos);
        delete selected;
        selected = nullptr;
        MainWindow::GetWindow()->inspector->SetNewEntity(nullptr);
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

    MainWindow::GetWindow()->inspector->SetNewEntity(selected);
    std::cout << "Selected item" + selected->GetName() << std::endl;
}

std::vector<Entity*> Hierarchy::GetEntityList()
{
    return entities;
}
