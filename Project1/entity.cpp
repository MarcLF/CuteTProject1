#include "entity.h"
#include "componenttransform.h"
#include "shaperenderer.h"
#include <iostream>

Entity::Entity(int nameID)
{
    name += " " + std::to_string(nameID);
    ComponentTransform* trans = new ComponentTransform();
    components.push_back(trans);

    ComponentShapeRenderer* renderer = new ComponentShapeRenderer();
    components.push_back(renderer);
}

const std::string Entity::GetName()
{
    return name;
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
