#include "inspector.h"
#include "ui_inspector.h"
#include "entity.h"
#include "componenttransform.h"
#include "shaperenderer.h"

#include <QGroupBox>
#include <QVBoxLayout>
#include <iostream>

Inspector::Inspector(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Inspector)
{
    ui->setupUi(this);

    transformBox = new QGroupBox(tr("Transform"));
    shapeBox = new QGroupBox(tr("Shape Renderer"));

    transformLayout = new QVBoxLayout();
    shapeLayout = new QVBoxLayout();

    transformBox->setMaximumSize(235, 130);
    shapeBox->setMaximumSize(235, 250);

    transformBox->setLayout(transformLayout);
    shapeBox->setLayout(shapeLayout);

    ui->Layout->addWidget(transformBox);
    ui->Layout->addWidget(shapeBox);
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
            transformLayout->removeWidget(compTransWidget);
            compTransWidget->hide();
            shapeLayout->removeWidget(compShapeRenderer);
            compShapeRenderer->hide();
        }

        this->selected = selected;
        compTransWidget = static_cast<ComponentTransform*>(selected->GetComponent(ComponentType::Component_Transform));
        if(compTransWidget != nullptr)
        {
            transformLayout->addWidget(compTransWidget);
            compTransWidget->show();
            //compTransWidget->setValues();
            std::cout << "I'm pretty" << std::endl;
        }

        compShapeRenderer = static_cast<ComponentShapeRenderer*>(selected->GetComponent(ComponentType::Component_ShapeRenderer));
        if(compShapeRenderer != nullptr)
        {
            shapeLayout->addWidget(compShapeRenderer);
            compShapeRenderer->show();
            std::cout << "I'm pretty2" << std::endl;
        }
    }
    else
    {
        this->selected = nullptr;
        transformLayout->removeWidget(compTransWidget);
        shapeLayout->removeWidget(compShapeRenderer);
        compTransWidget->hide();
        compShapeRenderer->hide();
    }
}
