#ifndef INSPECTOR_H
#define INSPECTOR_H

#include <QWidget>

namespace Ui {
class Inspector;
}

class Entity;
class ComponentTransform;
class ComponentShapeRenderer;
class QGroupBox;
class QVBoxLayout;

class Inspector : public QWidget
{
    Q_OBJECT

public:
    explicit Inspector(QWidget *parent = 0);
    ~Inspector();

    void SetNewEntity(Entity* selected);

private:
    Ui::Inspector *ui;
    Entity* selected = nullptr;
    QGroupBox* transformBox = nullptr;
    QGroupBox* shapeBox = nullptr;
    QVBoxLayout* transformLayout = nullptr;
     QVBoxLayout* shapeLayout = nullptr;
    ComponentTransform* compTransWidget = nullptr;
    ComponentShapeRenderer* compShapeRenderer = nullptr;
};

#endif // INSPECTOR_H
