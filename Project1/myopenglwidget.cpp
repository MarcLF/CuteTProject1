#include "myopenglwidget.h"
#include "camera.h"
#include "mesh.h"
#include "mainwindow.h"
#include "hierarchy.h"
#include "entity.h"
#include "componentrender.h"

#define PI 3.14159265358979323846f
#define RADTODEG(angle_in_radiants) (angle_in_radiants*180.0f)/PI
#define DEGTORAD(angle_in_degrees) (angle_in_degrees*PI)/180.0f

MyOpenGLWidget::MyOpenGLWidget(QWidget *parent) : QOpenGLWidget(parent)
{
    updateTimer = new QTimer(this);
    connect(updateTimer, SIGNAL(timeout()), this, SLOT(myUpdate()));
    updateTimer->start(60);

    mainCamera = new Camera(width(), height());
    mainCamera->projectionMatrix = QMatrix4x4((1.0f / tan(DEGTORAD(FOV) / 2)) / aspectRatio, 0, 0, 0, 0, (1.0f / tan( DEGTORAD(FOV) / 2)), 0 ,0 ,0 ,0, ((farPlane + nearPlane)/(nearPlane - farPlane)), ((2 * nearPlane * farPlane) / (nearPlane - farPlane)), 0, 0, -1, 0);
}

MyOpenGLWidget::~MyOpenGLWidget()
{

}

void MyOpenGLWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BACK);

    program.create();
    program.addShaderFromSourceFile(QOpenGLShader::Vertex, "C:/Users/Usuario/Documents/GitHub/CuteTProject1/Project1/shaders/VertexShader");
    program.addShaderFromSourceFile(QOpenGLShader::Fragment, "C:/Users/Usuario/Documents/GitHub/CuteTProject1/Project1/shaders/FragmentShader");
    program.link();
    program.bind();
}

void MyOpenGLWidget::resizeGL(int width, int height)
{
    makeCurrent();
}

void MyOpenGLWidget::paintGL()
{

    makeCurrent();
    mainCamera->PrepareMatrices();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glDepthMask(GL_TRUE);

    glClearDepth(1.0f);
    glClearColor(0.0f,0.0f,0.0f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if(program.bind())
    {
        std::vector<Entity*> toDraw = MainWindow::GetWindow()->GetHierarchy()->GetEntityList();

        for(int i = 0; i < toDraw.size(); i++)
        {
            ComponentRender* rendToDraw = static_cast<ComponentRender*>(toDraw[i]->GetComponent(ComponentType::Component_Render));
            if(rendToDraw != nullptr)
            {
                rendToDraw->Draw();
            }
        }
    }
}

void MyOpenGLWidget::myUpdate()
{
    paintGL();
    this->update();
}
