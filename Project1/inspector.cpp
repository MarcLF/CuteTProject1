#include "inspector.h"
#include "ui_inspector.h"
#include "entity.h"
#include "componenttransform.h"
#include "componentrender.h"
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
    renderBox = new QGroupBox(tr("Render"));

    transformLayout = new QVBoxLayout();
    renderLayout = new QVBoxLayout();

    transformBox->setLayout(transformLayout);
    renderBox->setLayout(renderLayout);

    renderBox->setMaximumSize(300, 100);

    entityName = new QLineEdit();
    entityName->setEnabled(false);

    ui->Layout->addWidget(entityName);
    ui->Layout->addWidget(transformBox);
    ui->Layout->addWidget(renderBox);

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
            renderLayout->removeWidget(compRenderer);
            compRenderer->hide();
        }

        this->selected = selected;

        compTransWidget = static_cast<ComponentTransform*>(selected->GetComponent(ComponentType::Component_Transform));

        if(compTransWidget != nullptr)
        {
            transformLayout->addWidget(compTransWidget);
            compTransWidget->show();           
        }

        compRenderer = static_cast<ComponentRender*>(selected->GetComponent(ComponentType::Component_Render));
        if(compRenderer != nullptr)
        {
            renderLayout->addWidget(compRenderer);
            compRenderer->show();
        }

        entityName->setText(selected->GetName().c_str());
        entityName->setEnabled(true);
    }
    else
    {
        this->selected = nullptr;
        transformLayout->removeWidget(compTransWidget);
        renderLayout->removeWidget(compRenderer);
        compTransWidget->hide();
        compRenderer->hide();
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
