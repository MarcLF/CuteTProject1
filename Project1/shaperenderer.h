#ifndef SHAPERENDERER_H
#define SHAPERENDERER_H

#include <QWidget>

namespace Ui {
class ShapeRenderer;
}

class ShapeRenderer : public QWidget
{
    Q_OBJECT

public:
    explicit ShapeRenderer(QWidget *parent = nullptr);
    ~ShapeRenderer();

private:
    Ui::ShapeRenderer *ui;
};

#endif // SHAPERENDERER_H
