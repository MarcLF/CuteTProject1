#ifndef ENTITY_H
#define ENTITY_H

#include <vector>
#include <string>
#include "component.h"

class Entity
{
public:
    Entity(int nameID);

    const std::string GetName();

    Component* GetComponent(ComponentType type);

protected:
    std::string name = "Entity";
    std::vector<Component*> components;
};

#endif // ENTITY_H
