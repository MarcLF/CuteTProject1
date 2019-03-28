#include "hierarchy.h"
#include "ui_hierarchy.h"
#include "entity.h"

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
        entities.remove(selected);
        delete selected;
    }
}

void Hierarchy::SelectEntity(QListWidgetItem* item)
{
    if(selectedItem != nullptr)
    {
        selectedItem->setSelected(false);
    }
    item->setSelected(true);
    selectedItem = item;
    std::cout << "Selected item" + item->text().toStdString() << std::endl;
}
