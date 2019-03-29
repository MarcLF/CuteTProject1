#include "component.h"
#include "ui_component.h"

Component::Component(ComponentType type, QWidget *parent) :
    type(type), QWidget(parent)
{
    switch(type)
    {
    case Component_Transform:
        name = "Transform";
        break;
    case Component_ShapeRenderer:
        name = "Shape Renderer";
        break;
    }
}

Component::~Component()
{
    delete ui;
}

ComponentType Component::GetType()
{
    return type;
}
