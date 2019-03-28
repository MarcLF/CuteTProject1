#ifndef COMPONENT_H
#define COMPONENT_H

#include <string>

enum ComponentType
{
    Component_Transform,
    Component_ShapeRenderer
};

class Component
{
public:
    Component(ComponentType type);
    virtual ~Component();

protected:
    std::string name;
    ComponentType type;
};

#endif // COMPONENT_H
