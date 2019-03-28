#include "entity.h"

Entity::Entity(int nameID)
{
    name += " " + std::to_string(nameID);
}

const std::string Entity::GetName()
{
    return name;
}
