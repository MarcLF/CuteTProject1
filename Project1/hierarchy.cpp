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
    saveFile.close();
}

void Hierarchy::loadEntities(QString path)
{
    selected = nullptr;
    selectedItem = nullptr;

    while(!entities.empty())
    {
        delete entities.back();
        entities.pop_back();
    }

    ui->EntityList->clear();

    entityID = 1;

    QFile loadFile(path);

    QString data;
    QJsonDocument loadDocEnt;

    if(loadFile.open(QIODevice::ReadOnly | QIODevice::Text) == false)
    {
        return;
    }

    data = loadFile.readAll();
    loadFile.close();

    loadDocEnt = QJsonDocument::fromJson(data.toUtf8());

    QJsonObject root = loadDocEnt.object();

    qDebug() << root.value("EntitiesData").toArray()[0].toObject().begin().key();

    for(int i = 0; i < root.value("EntitiesData").toArray().size(); i++)
    {
        QString entityName = root.value("EntitiesData").toArray()[i].toObject().begin().key();

        QJsonObject transformComponents = root.value("EntitiesData").toArray()[i].toObject().value(entityName).toArray()[0].toObject().value("Transform").toArray()[0].toObject();

        double jsonPosX = transformComponents.find("posX").value().toDouble();
        double jsonPosY = transformComponents.find("posY").value().toDouble();
        double jsonRotX = transformComponents.find("rotX").value().toDouble();
        double jsonRotY = transformComponents.find("rotY").value().toDouble();
        double jsonScaleX = transformComponents.find("scaleX").value().toDouble();
        double jsonScaleY = transformComponents.find("scaleY").value().toDouble();

        QJsonObject shapeRendererComponents = root.value("EntitiesData").toArray()[i].toObject().value(entityName).toArray()[1].toObject().value("Shape Renderer").toArray()[0].toObject();

        double FCBlueParam = shapeRendererComponents.find("Fill Color Blue Param").value().toDouble();
        double FCGreenParam = shapeRendererComponents.find("Fill Color Green Param").value().toDouble();
        double FCRedParam = shapeRendererComponents.find("Fill Color Red Param").value().toDouble();
        double shapeIndex = shapeRendererComponents.find("Shape Index").value().toDouble();
        double shapeSize = shapeRendererComponents.find("Shape Size").value().toDouble();
        double SCBlueParam = shapeRendererComponents.find("Stroke Color Blue Param").value().toDouble();
        double SCGreenParam = shapeRendererComponents.find("Stroke Color Green Param").value().toDouble();
        double SCRedParam = shapeRendererComponents.find("Stroke Color Red Param").value().toDouble();
        double SStyleIndex = shapeRendererComponents.find("Stroke Style Index").value().toDouble();
        double SThickness = shapeRendererComponents.find("Stroke Thickness").value().toDouble();

        ComponentTransform *componentTrans = new ComponentTransform();

        componentTrans->modifyXPos(jsonPosX);
        componentTrans->modifyYPos(jsonPosY);

        componentTrans->modifyXRot(jsonRotX);
        componentTrans->modifyYRot(jsonRotY);

        componentTrans->modifyXScale(jsonScaleX);
        componentTrans->modifyYScale(jsonScaleY);

        componentTrans->setValues();

        ComponentShapeRenderer *componentShapeRenderer = new ComponentShapeRenderer();

        componentShapeRenderer->SetFillColor(FCBlueParam, FCGreenParam, FCRedParam);
        componentShapeRenderer->SetStrokeColor(SCBlueParam, SCGreenParam, SCRedParam);
        componentShapeRenderer->SetShapeIndex(shapeIndex);
        componentShapeRenderer->SetShapeSize(shapeSize);
        componentShapeRenderer->SetStrokeStyleIndex(SStyleIndex);
        componentShapeRenderer->SetStrokeThickness(SThickness);

        Entity* entity = new Entity(entityID++, componentTrans, componentShapeRenderer);

        entity->SetName(entityName.toStdString());
        entities.push_back(entity);
        ui->EntityList->addItem(entity->GetName().c_str());
    }
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
