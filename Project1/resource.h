#ifndef RESOURCE_H
#define RESOURCE_H

#include <QMap>
#include <string>

class Mesh;

class Resource
{
public:
    Resource();
    virtual ~Resource();

    void AddModel(Mesh* mesh, QString fileName);
    Mesh* GetModel(QString fileName);
    QMap<QString, Mesh*> GetModelMap();
    virtual void update();
    virtual void destroy();

private:
    QMap<QString, Mesh*> models;
};

#endif // RESOURCE_H
