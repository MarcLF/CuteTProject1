#include "shaperenderer.h"
#include "ui_shaperenderer.h"

ComponentShapeRenderer::ComponentShapeRenderer(QWidget *parent) :
    Component(ComponentType::Component_ShapeRenderer, parent),
    ui(new Ui::ShapeRenderer)
{
    ui->setupUi(this);

    fillColor = new QColorDialog();

    strokeColor = new QColorDialog();

    connect(ui->FillColorButton, SIGNAL(clicked()), this, SLOT(onFillColor()));
    connect(fillColor, SIGNAL(accepted()), this, SLOT(changeFillColor()));

    connect(ui->StrokeColorButton, SIGNAL(clicked()), this, SLOT(onStrokeColor()));
    connect(strokeColor, SIGNAL(accepted()), this, SLOT(changeStrokeColor()));
}

ComponentShapeRenderer::~ComponentShapeRenderer()
{
    delete ui;
}

void ComponentShapeRenderer::onFillColor()
{
    fillColor->show();
}

void ComponentShapeRenderer::changeFillColor()
{
    QString s("background: " + fillColor->currentColor().name() + ";");
    ui->FillColorButton->setStyleSheet(s);
    ui->FillColorButton->update();
}

void ComponentShapeRenderer::onStrokeColor()
{
    strokeColor->show();
}

void ComponentShapeRenderer::changeStrokeColor()
{
    QString s("background: " + strokeColor->currentColor().name() + ";");
    ui->StrokeColorButton->setStyleSheet(s);
    ui->StrokeColorButton->update();
}
