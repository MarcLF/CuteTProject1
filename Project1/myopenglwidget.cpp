#include "myopenglwidget.h"
#include "camera.h"
#include "mesh.h"
#include "mainwindow.h"
#include "hierarchy.h"
#include "entity.h"
#include "componentrender.h"
#include "componenttransform.h"

#define PI 3.14159265358979323846f
#define RADTODEG(angle_in_radiants) (angle_in_radiants*180.0f)/PI
#define DEGTORAD(angle_in_degrees) (angle_in_degrees*PI)/180.0f

MyOpenGLWidget::MyOpenGLWidget(QWidget *parent) : QOpenGLWidget(parent)
{

    updateTimer = new QTimer(this);
    connect(updateTimer, SIGNAL(timeout()), this, SLOT(myUpdate()));
    updateTimer->start(60);

    this->setMouseTracking(true);
    this->setFocus();
    this->setFocusPolicy(Qt::FocusPolicy::StrongFocus);

    mainCamera = new Camera(width(), height());
}

MyOpenGLWidget::~MyOpenGLWidget()
{

}

void MyOpenGLWidget::initializeGL()
{
    initializeOpenGLFunctions();

    connect(context(), SIGNAL(aboutToBeDestroyed()), this, SLOT(finalizeGL()));

    program.create();
    program.addShaderFromSourceFile(QOpenGLShader::Vertex, "C:/Users/DANIEL H/Documents/GitHub/CuteTProject1/Project1/shaders/vertex_shader.vert");
    program.addShaderFromSourceFile(QOpenGLShader::Fragment, "C:/Users/DANIEL H/Documents/GitHub/CuteTProject1/Project1/shaders/fragment_shader.frag");
    program.link();
    InitBuffers();
    program.bind();
    program.release();
}

void MyOpenGLWidget::resizeGL(int width, int height)
{
    resize(width,height);
    mainCamera->viewportWidth = width;
    mainCamera->viewportHeight = height;

    glDeleteTextures(1, &colorTexture);
    glDeleteTextures(1, &depthTexture);
    glDeleteFramebuffers(1, &fbo);

    InitBuffers();
}

void MyOpenGLWidget::paintGL()
{
    makeCurrent();

    mainCamera->PrepareMatrices();

    glBindFramebuffer(GL_FRAMEBUFFER,fbo);
    glClearDepth(1.0);
    glClearColor(1.0f,0.5f,0.5f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    GLenum buffers[] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1,buffers);

    if(program.bind())
    {
        program.setUniformValue("projectionMatrix", mainCamera->projectionMatrix);
        QMatrix4x4 cameraTransfrom = mainCamera->viewMatrix;

        program.setUniformValue("albedoTexture", 0 );
        glBindTexture(GL_TEXTURE_2D, 0);

        std::vector<Entity*> toDraw = MainWindow::GetWindow()->GetHierarchy()->GetEntityList();

        for(int i = 0; i < toDraw.size(); i++)
        {
            ComponentRender* rendToDraw = static_cast<ComponentRender*>(toDraw[i]->GetComponent(ComponentType::Component_Render));
            if(rendToDraw != nullptr)
            {
                makeCurrent();
                rendToDraw->WorkMeshes();
                program.setUniformValue("worldViewMatrix", cameraTransfrom * static_cast<ComponentTransform*>(toDraw[i]->GetComponent(ComponentType::Component_Transform))->GetTransMatrix());
                rendToDraw->Draw();
            }
        }

        program.release();
    }
}

void MyOpenGLWidget::InitBuffers()
{
    //std::cout << "Hieght:" << screen_height << " Width:" << screen_width << std::endl;
    //Render to texture init
    glGenTextures(1, &colorTexture);
    glBindTexture(GL_TEXTURE_2D, colorTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width(), height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

    glGenTextures(1, &depthTexture);
    glBindTexture(GL_TEXTURE_2D, depthTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width(), height(), 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D, colorTexture,0);
    glFramebufferTexture2D(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D, depthTexture,0);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);



    //GLenum buffers[] = {GL_COLOR_ATTACHMENT0};
    //glDrawBuffers(1,buffers);

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    switch (status)
    {
    case GL_FRAMEBUFFER_COMPLETE://Everything'sOK
        qDebug() << "Framebuffer is Veri gut patates amb suc";
        break;
    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
        qDebug() << "FramebufferERROR: GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT";
        break;
    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
        qDebug() << "Framebuffer ERROR:GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT";
        break;
    case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
        qDebug()<<"Framebuffer ERROR:GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER";
        break;
    case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
        qDebug()<<"Framebuffer ERROR:GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER";
        break;
    case GL_FRAMEBUFFER_UNSUPPORTED:
        qDebug()<<"Framebuffer ERROR:GL_FRAMEBUFFER_UNSUPPORTED";
        break;
    default:
        qDebug() << "Framebuffer ERROR: Unknown ERROR";
        break;
    }
}

void MyOpenGLWidget::keyPressEvent(QKeyEvent *event)
{
    qDebug("Hi felicia");
    if(event->key() == Qt::Key_W)
    {
        mainCamera->Move(QVector3D(0,0,1));
    }
    if(event->key() == Qt::Key_S)
    {
        mainCamera->Move(QVector3D(0,0,-1));
    }
    if(event->key() == Qt::Key_A)
    {
        mainCamera->Move(QVector3D(-1,0,0));
    }
    if(event->key() == Qt::Key_D)
    {
        mainCamera->Move(QVector3D(1,0,0));
    }
}

void MyOpenGLWidget::keyReleaseEvent(QKeyEvent *event)
{

}

void MyOpenGLWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == 1)
    {
        isRotating = true;
    }
}

void MyOpenGLWidget::mouseMoveEvent(QMouseEvent *event)
{
    motionX = event->x() - mouseX;
    motionY = event->y() - mouseY;
    mouseX = event->x();
    mouseY = event->y();

    if(isRotating)
        mainCamera->Rotate(motionX,motionY);

}

void MyOpenGLWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == 1)
    {
        isRotating = false;
    }

}

void MyOpenGLWidget::myUpdate()
{
    this->update();
}
