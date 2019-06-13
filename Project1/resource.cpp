#include "resource.h"
#include "mesh.h"

Resource::Resource()
{

}

Resource::~Resource()
{
    for(auto it : models.keys())
    {
        delete models[it];
    }
}


void Resource::AddModel(Mesh *mesh, QString fileName)
{
    models.insert(fileName, mesh);
}

Mesh* Resource::GetModel(QString fileName)
{
    return models[fileName];
}

QMap<QString, Mesh *> Resource::GetModelMap()
{
    return models;
}

void Resource::update()
{

}

void Resource::destroy()
{

}
