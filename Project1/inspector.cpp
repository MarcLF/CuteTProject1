#include "inspector.h"
#include "ui_inspector.h"
#include "entity.h"
#include "componenttransform.h"

#include <iostream>

Inspector::Inspector(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Inspector)
{
    ui->setupUi(this);
    compTransWidget = new ComponentTransform();
}

Inspector::~Inspector()
{
    delete ui;
}

void Inspector::SetNewEntity(Entity *selected)
{
    std::cout << "Hi bitches" << std::endl;
    if(selected != nullptr)
    {
        std::cout << "I'm here" << std::endl;
        //compTransWidget = static_cast<ComponentTransform*>(selected->GetComponent(ComponentType::Component_Transform));
        ui->Layout->insertWidget(0,compTransWidget);
        std::cout << "Byeee" << std::endl;
    }
    else
    {
        ui->Layout->removeWidget(compTransWidget);
    }
}
