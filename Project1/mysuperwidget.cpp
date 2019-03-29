#include "mysuperwidget.h"
#include <QPainter>
#include "hierarchy.h"
#include "mainwindow.h"
#include "entity.h"
#include "componenttransform.h"
#include "shaperenderer.h"

MySuperWidget::MySuperWidget(QWidget *parent) : QWidget(parent)
{
    setAutoFillBackground(true);
}

QSize MySuperWidget::sizeHint() const
{
    return QSize(256,256);
}

QSize MySuperWidget::minimumSizeHint() const
{
    return QSize(64, 64);
}

void MySuperWidget::paintEvent(QPaintEvent *)
{
    //Painting the background
    QPainter painter(this);
    QBrush brush;
    QPen pen;

    QColor backgroundColor = QColor::fromRgb(255,255,255);
    brush.setColor(backgroundColor);
    brush.setStyle(Qt::BrushStyle::SolidPattern);
    pen.setStyle(Qt::PenStyle::NoPen);
    painter.setBrush(brush);
    painter.setPen(pen);

    painter.drawRect(rect());

    //Iterating entities
    std::vector<Entity*> entityList = MainWindow::GetWindow()->hierarchy->GetEntityList();

    for(int i = 0; i < entityList.size(); i++)
    {
        QRect NewShape;

        ComponentShapeRenderer *shaperenderer =  static_cast<ComponentShapeRenderer*>(entityList[i]->GetComponent(ComponentType::Component_ShapeRenderer));
        brush.setColor(shaperenderer->GetFillColor());
        pen.setWidth(shaperenderer->GetStrokeThickness());
        pen.setColor(shaperenderer->GetStrokeColor());
        pen.setStyle(shaperenderer->GetPenStyle());

        painter.setPen(pen);
        painter.setBrush(brush);

        ComponentTransform *CompTransform =  static_cast<ComponentTransform*>(entityList[i]->GetComponent(ComponentType::Component_Transform));

        painter.translate(CompTransform->GetPosX(),CompTransform->GetPosY());
        painter.rotate(CompTransform->GetRotX());
        painter.scale(CompTransform->GetScaleX(), CompTransform->GetScaleY());

        if(shaperenderer->GetShapeIndex() == 0)
        {
            int r = shaperenderer->GetShapeSize();
            int w = r*2;
            int h = r*2;
            int x = rect().width() / 2 - r;
            int y = rect().height() / 2 - r;
            NewShape.setRect(0,0,w,h);
            painter.drawEllipse(NewShape);
        }
        else
        {
           int size = shaperenderer->GetShapeSize();
           NewShape.setRect(0,0,size,size);
           painter.drawRect(NewShape);
        }
    }




    /*brush.setColor(whiteColor);
    pen.setWidth(4);
    pen.setColor(blackColor);
    pen.setStyle(Qt::PenStyle::DashLine);
    painter.setBrush(brush);
    painter.setPen(pen);

    int r = 64;
    int w = r*2;
    int h = r*2;
    int x = rect().width() / 2 - r;
    int y = rect().height() / 2 - r;
    QRect circleRect(x,y,w,h);
    painter.drawEllipse(circleRect);*/


}
