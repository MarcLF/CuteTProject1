#ifndef SHAPERENDERER_H
#define SHAPERENDERER_H

#include <QWidget>
#include <QColorDialog>
#include "component.h"

namespace Ui {
class ShapeRenderer;
}

class ComponentShapeRenderer : public Component
{
    Q_OBJECT

public:
    explicit ComponentShapeRenderer(QWidget *parent = nullptr);
    ~ComponentShapeRenderer();
    QColor GetFillColor();
    QColor GetStrokeColor();

    double GetStrokeThickness();
    Qt::PenStyle GetPenStyle();

    int GetShapeIndex();

    int GetShapeSize();
    int GetStrokeStyleIndex();
public slots:
    void onFillColor();
    void changeFillColor();
    void onStrokeColor();
    void changeStrokeColor();

private:
    Ui::ShapeRenderer *ui;
    QColorDialog* fillColor = nullptr;
    QColorDialog* strokeColor = nullptr;
};

#endif // SHAPERENDERER_H
