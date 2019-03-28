#include "component.h"



Component::Component(ComponentType type) : type(type)
{
    switch(type)
    {
    case Component_Transform:
        name = "Transform";
        break;
    case Component_ShapeRenderer:
        name = "Shape Renderer";
        break;
    }
}

Component::~Component()
{

}
