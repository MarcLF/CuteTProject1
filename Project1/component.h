#ifndef COMPONENT_H
#define COMPONENT_H

#include <QWidget>

enum ComponentType
{
    Component_Transform,
    Component_ShapeRenderer
};

namespace Ui {
class Component;
}

class Component : public QWidget
{
    Q_OBJECT

public:
    explicit Component(ComponentType type, QWidget *parent = nullptr);
    ~Component();

    ComponentType GetType();
    std::string GetName();

protected:
    std::string name;
    ComponentType type;

private:
    Ui::Component *ui;
};

#endif // COMPONENT_H
