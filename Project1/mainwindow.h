#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
class Rendering;
}

class Inspector;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
public slots:

    void openProject();
    void saveProject();

private:
    Ui::MainWindow *ui;
    Ui::Rendering *uiRendering;
    Inspector *inspector;
};

#endif // MAINWINDOW_H
