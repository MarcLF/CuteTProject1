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
    void SetName(std::string setName);

    Component* GetComponent(ComponentType type);
    std::vector<Component*> GetComponents();

protected:
    std::string name = "Entity";
    std::vector<Component*> components;
};

#endif // ENTITY_H
