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

    Inspector *inspector;
    Hierarchy *hierarchy;
public slots:

    void openProject();
    void saveProject();

    Inspector* GetInspector();


private:
    Ui::MainWindow *ui;
    Ui::Rendering *uiRendering;
    static MainWindow *window;

};

#endif // MAINWINDOW_H
