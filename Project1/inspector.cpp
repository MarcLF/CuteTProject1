#include "inspector.h"
#include "ui_inspector.h"
#include "entity.h"
#include "componenttransform.h"
#include "shaperenderer.h"
#include "mainwindow.h"
#include "hierarchy.h"

#include <QGroupBox>
#include <QVBoxLayout>
#include <QLineEdit>
#include <iostream>
#include <QDebug>

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

    entityName = new QLineEdit();
    entityName->setEnabled(false);

    ui->Layout->addWidget(entityName);
    ui->Layout->addWidget(transformBox);
    ui->Layout->addWidget(shapeBox);

    connect(entityName, SIGNAL(editingFinished()), this, SLOT(NameChanged()));
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
            compTransWidget->setValues();
            std::cout << "I'm pretty" << std::endl;
        }

        compShapeRenderer = static_cast<ComponentShapeRenderer*>(selected->GetComponent(ComponentType::Component_ShapeRenderer));
        if(compShapeRenderer != nullptr)
        {
            shapeLayout->addWidget(compShapeRenderer);
            compShapeRenderer->show();
            std::cout << "I'm pretty2" << std::endl;
        }

        entityName->setText(selected->GetName().c_str());
        entityName->setEnabled(true);
    }
    else
    {
        this->selected = nullptr;
        transformLayout->removeWidget(compTransWidget);
        shapeLayout->removeWidget(compShapeRenderer);
        compTransWidget->hide();
        compShapeRenderer->hide();
        entityName->setText("");
        entityName->setEnabled(false);
    }
}

void Inspector::NameChanged()
{
    if(this->selected != nullptr)
    {
        this->selected->SetName(entityName->text().toStdString());
        MainWindow::GetWindow()->GetHierarchy()->ChangeItemName(selected, entityName->text());
    }
}
