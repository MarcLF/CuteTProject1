#include "shaperenderer.h"
#include "ui_shaperenderer.h"
#include <QPainter>

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

QColor ComponentShapeRenderer::GetFillColor()
{
    return fillColor->currentColor();
}

QColor ComponentShapeRenderer::GetStrokeColor()
{
    return strokeColor->currentColor();
}

double ComponentShapeRenderer::GetStrokeThickness()
{
    return ui->StrokeThiccWidget->value();
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

Qt::PenStyle ComponentShapeRenderer::GetPenStyle()
{
    int styleIndex = ui->StrokeStyleWidget->currentIndex();

    switch(styleIndex)
    {
    case 0:
        return Qt::PenStyle::SolidLine;
    case 1:
        return Qt::PenStyle::DashLine;
    case 2:
        return Qt::PenStyle::DotLine;
    default:
        return Qt::PenStyle::SolidLine;
    }
}

int ComponentShapeRenderer::GetShapeIndex()
{
    return ui->ShapeWidget->currentIndex();
}

int ComponentShapeRenderer::GetShapeSize()
{
    return ui->SizeWidget->value();
}

int ComponentShapeRenderer::GetStrokeStyleIndex()
{
    return ui->StrokeStyleWidget->currentIndex();
}

void ComponentShapeRenderer::SetShapeSize(int newShape)
{
    ui->SizeWidget->setValue(newShape);
}

void ComponentShapeRenderer::SetShapeIndex(int newIndex)
{
    ui->ShapeWidget->setCurrentIndex(newIndex);
}

void ComponentShapeRenderer::SetFillColor(int newFillBlueColor, int newFillGreenColor, int newFillRedColor)
{
    QColor newColor = QColor(newFillRedColor, newFillGreenColor, newFillBlueColor);
    fillColor->setCurrentColor(newColor);
}

void ComponentShapeRenderer::SetStrokeColor(int newFillBlueColor, int newFillGreenColor, int newFillRedColor)
{
    QColor newStrokeColor = QColor(newFillRedColor, newFillGreenColor, newFillBlueColor);
    strokeColor->setCurrentColor(newStrokeColor);
}

void ComponentShapeRenderer::SetStrokeStyleIndex(int newStrokeStyleIndex)
{
    ui->StrokeStyleWidget->setCurrentIndex(newStrokeStyleIndex);
}

void ComponentShapeRenderer::SetStrokeThickness(int newStrokeThickness)
{
    ui->StrokeThiccWidget->setValue(newStrokeThickness);
}
