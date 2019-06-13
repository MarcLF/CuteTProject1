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
    void AddMesh(Mesh* toAdd);
    void Draw();
    void WorkMeshes();

    void AddModelToComboBox(QString modelName, bool isMine = false);

private:
    Ui::ComponentRender *ui;
    Mesh* myMesh = nullptr;

signals:

public slots:
    void ChangeModel(int index);
};

#endif // COMPONENTRENDER_H
