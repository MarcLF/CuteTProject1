#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
class Rendering;
}

class Hierarchy;
class Inspector;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    static MainWindow* GetWindow();

    Inspector* GetInspector();
    Hierarchy* GetHierarchy();
public slots:

    void openProject();
    void saveProject();


private:
    Ui::MainWindow *ui;
    Ui::Rendering *uiRendering;
    static MainWindow *window;
    Inspector *inspector;
    Hierarchy *hierarchy;

};

#endif // MAINWINDOW_H
