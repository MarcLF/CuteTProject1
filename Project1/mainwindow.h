#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QColorDialog>

namespace Ui {
class MainWindow;
class Rendering;
}

class Hierarchy;
class Inspector;
class MyOpenGLWidget;
class LightDirection;
class DoFOptions;
class Resource;


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    static MainWindow* GetWindow();

    Inspector* GetInspector();
    Hierarchy* GetHierarchy();
    Resource *GetResources();
    void       dragEnterEvent(QDragEnterEvent *e);
    void       dropEvent(QDropEvent *e);
public slots:
    void ChangeToAlbedo();
    void ChangeToLighting();
    void ChangeToNormals();
    void ChangeToDepthTest();

    void OpenLightSettings();
    void OpenDoFSettings();

    void onLightColor();
    void changeLightColor();

    void SwitchBlur();
    void SwitchDoF();

private:
    Ui::MainWindow *ui;
    Ui::Rendering *uiRendering;
    static MainWindow *window;
    Inspector *inspector;
    Hierarchy *hierarchy;
    MyOpenGLWidget *openGLWidget;
    LightDirection *lightDirection;
    DoFOptions* dofOptions;
    QColorDialog* lightColorPicker;
    Resource* resources;
};

#endif // MAINWINDOW_H
