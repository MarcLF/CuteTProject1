#include "shaperenderer.h"
#include "ui_shaperenderer.h"

ShapeRenderer::ShapeRenderer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ShapeRenderer)
{
    ui->setupUi(this);
}

ShapeRenderer::~ShapeRenderer()
{
    delete ui;
}
