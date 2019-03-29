#ifndef HIERARCHY_H
#define HIERARCHY_H

#include <QWidget>
#include <vector>
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
    std::vector<Entity*> GetEntityList();

private:
    Ui::Hierarchy *ui;
    std::vector<Entity*> entities;
    Entity* selected = nullptr;
    int selectedPos = 0;
    QListWidgetItem* selectedItem = nullptr;
    int entityID = 1;
};

#endif // HIERARCHY_H
