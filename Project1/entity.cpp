#include "entity.h"
#include "componenttransform.h"
#include "shaperenderer.h"
#include "componentrender.h"

#include <iostream>
#include <QDebug>

Entity::Entity(int nameID)
{
    name += " " + std::to_string(nameID);
    ComponentTransform* trans = new ComponentTransform();
    components.push_back(trans);

    ComponentRender* render = new ComponentRender();
    components.push_back(render);
}

Entity::Entity(int nameID, ComponentTransform* newCompTrans, ComponentRender* newCompRender)
{
    name += " " + std::to_string(nameID);

    components.push_back(newCompTrans);
    components.push_back(newCompRender);
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
            return components[i];
        }
    }
    return nullptr;
}

std::vector<Component *> Entity::GetComponents()
{
    return components;
}

void Entity::LoadObjModel(QString fileName)
{
    ComponentRender* renderComp = (ComponentRender*)GetComponent(Component_Render);

    renderComp->AddMesh(fileName.toLocal8Bit().data());
}

void Entity::SetMesh(Mesh *toAdd)
{
    ComponentRender* renderComp = (ComponentRender*)GetComponent(Component_Render);
    renderComp->AddMesh(toAdd);
}
