#ifndef HIERARCHY_H
#define HIERARCHY_H

#include <QWidget>
#include <list>
#include <QListWidgetItem>

class Entity;

namespace Ui {
class Hierarchy;
}

class Hierarchy : public QWidget
{
    Q_OBJECT

public:
    explicit Hierarchy(QWidget *parent = 0);
    ~Hierarchy();

public slots:
    void AddEntity();
    void RemoveEntity();
    void SelectEntity(QListWidgetItem* item);

private:
    Ui::Hierarchy *ui;
    std::list<Entity*> entities;
    Entity* selected = nullptr;
    QListWidgetItem* selectedItem = nullptr;
    int entityID = 1;
};

#endif // HIERARCHY_H
