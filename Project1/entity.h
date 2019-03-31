#ifndef ENTITY_H
#define ENTITY_H

#include <vector>
#include <string>
#include "component.h"
#include "componenttransform.h"
#include "shaperenderer.h"

class Entity
{
public:
    Entity(int nameID);
    Entity(int nameID, ComponentTransform* newCompTrans, ComponentShapeRenderer* newCompShapeRend);

    const std::string GetName();
    void SetName(std::string setName);

    Component* GetComponent(ComponentType type);
    std::vector<Component*> GetComponents();
    void AddComponent(Component newComponent);

protected:
    std::string name = "Entity";
    std::vector<Component*> components;
};

#endif // ENTITY_H
