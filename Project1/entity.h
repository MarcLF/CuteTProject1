#ifndef ENTITY_H
#define ENTITY_H

#include <vector>
#include <string>

class Component;

class Entity
{
public:
    Entity(int nameID);

    const std::string GetName();

protected:
    std::string name = "Entity";
    std::vector<Component*> components;
};

#endif // ENTITY_H
