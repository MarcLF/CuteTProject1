#include "inspector.h"
#include "ui_inspector.h"
#include "entity.h"
#include "componenttransform.h"
#include "shaperenderer.h"

#include <iostream>

Inspector::Inspector(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Inspector)
{
    ui->setupUi(this);
}

Inspector::~Inspector()
{
    delete ui;
}

void Inspector::SetNewEntity(Entity *selected)
{
    if(this->selected == selected)
    {
        return;
    }

    if(selected != nullptr)
    {
        if(this->selected != nullptr)
        {
            ui->Layout->removeWidget(compTransWidget);
            compTransWidget->hide();
            ui->Layout->removeWidget(compShapeRenderer);
            compShapeRenderer->hide();
        }

        this->selected = selected;
        compTransWidget = static_cast<ComponentTransform*>(selected->GetComponent(ComponentType::Component_Transform));
        if(compTransWidget != nullptr)
        {
            ui->Layout->addWidget(compTransWidget);
            compTransWidget->show();
            //compTransWidget->setValues();
            std::cout << "I'm pretty" << std::endl;
        }

        compShapeRenderer = static_cast<ComponentShapeRenderer*>(selected->GetComponent(ComponentType::Component_ShapeRenderer));
        if(compShapeRenderer != nullptr)
        {
            ui->Layout->addWidget(compShapeRenderer);
            compShapeRenderer->show();
            std::cout << "I'm pretty2" << std::endl;
        }
    }
    else
    {
        this->selected = nullptr;
        ui->Layout->removeWidget(compTransWidget);
        ui->Layout->removeWidget(compShapeRenderer);
        compTransWidget->hide();
        compShapeRenderer->hide();
    }
}
