#include "mysuperwidget.h"
#include <QPainter>
#include "hierarchy.h"
#include "mainwindow.h"
#include "entity.h"
#include "componenttransform.h"
#include "shaperenderer.h"

#include <QTimer>
#include <QDebug>

MySuperWidget::MySuperWidget(QWidget *parent) : QWidget(parent)
{
    setAutoFillBackground(true);

    updateTimer = new QTimer(this);
    connect(updateTimer, SIGNAL(timeout()), this, SLOT(update()));
    updateTimer->start(60);

    //mainCamera = new Camera(100, 100);
    //mainCamera->projectionMatrix = QMatrix4x4((1.0f/tan(DEGTORAD(field_of_view)/2))/aspect_ratio, 0, 0, 0, 0, (1.0f/tan( DEGTORAD(field_of_view)/2)), 0 ,0 ,0 ,0, ((far_plane_distance + near_plane_distance)/(near_plane_distance - far_plane_distance)), ((2*near_plane_distance*far_plane_distance)/(near_plane_distance - far_plane_distance)),0,0,-1,0);
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
    std::vector<Entity*> entityList = MainWindow::GetWindow()->GetHierarchy()->GetEntityList();

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

        painter.resetTransform();
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
}

void MySuperWidget::myUpdate()
{
    this->update();
}
