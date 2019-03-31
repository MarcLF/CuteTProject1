#include "entity.h"
#include "componenttransform.h"
#include "shaperenderer.h"

#include <iostream>
#include <QDebug>

Entity::Entity(int nameID)
{
    name += " " + std::to_string(nameID);
    ComponentTransform* trans = new ComponentTransform();
    components.push_back(trans);

    ComponentShapeRenderer* renderer = new ComponentShapeRenderer();
    components.push_back(renderer);
}

Entity::Entity(int nameID, ComponentTransform* newCompTrans, ComponentShapeRenderer* newCompShapeRend)
{
    name += " " + std::to_string(nameID);

    components.push_back(newCompTrans);
    components.push_back(newCompShapeRend);
}

const std::string Entity::GetName()
{
    return name;
}

void Entity::SetName(std::string setName)
{
    name = setName;
}

Component *Entity::GetComponent(ComponentType type)
{
    for(int i = 0; i < components.size(); i++)
    {
        if(components[i]->GetType() == type)
        {
            std::cout << "LALALA" << std::endl;
            return components[i];
        }
    }

    std::cout << "trololo" << std::endl;
    return nullptr;
}

std::vector<Component *> Entity::GetComponents()
{
    return components;
}
