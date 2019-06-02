#ifndef COMPONENTRENDER_H
#define COMPONENTRENDER_H

#include <QWidget>
#include "component.h"

class Mesh;

namespace Ui {
class ComponentRender;
}

class ComponentRender : public Component
{
    Q_OBJECT

public:
    explicit ComponentRender(QWidget *parent = nullptr);
    ~ComponentRender();

    void AddMesh(const char* fileName);
    void Draw();

private:
    Ui::ComponentRender *ui;
    Mesh* myMesh = nullptr;
};

#endif // COMPONENTRENDER_H
