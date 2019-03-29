#include "hierarchy.h"
#include "ui_hierarchy.h"
#include "entity.h"
#include "mainwindow.h"
#include "inspector.h"

#include <iostream>

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
        static_cast<MainWindow*>(parent())->inspector->SetNewEntity(nullptr);
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

    static_cast<MainWindow*>(parent())->inspector->SetNewEntity(selected);
    std::cout << "Selected item" + selected->GetName() << std::endl;
}
