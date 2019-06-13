#ifndef ENTITY_H
#define ENTITY_H

#include <vector>
#include <string>
#include "component.h"

class ComponentTransform;
class ComponentRender;
class Mesh;

class Entity
{
public:
    Entity(int nameID);
    Entity(int nameID, ComponentTransform* newCompTrans, ComponentRender* newCompRender);

    const std::string GetName();
    void SetName(std::string setName);

    Component* GetComponent(ComponentType type);
    std::vector<Component*> GetComponents();
    void AddComponent(Component newComponent);
    void LoadObjModel(QString fileName);
    void SetMesh(Mesh* toAdd);

protected:
    std::string name = "Entity";
    std::vector<Component*> components;
};

#endif // ENTITY_H
