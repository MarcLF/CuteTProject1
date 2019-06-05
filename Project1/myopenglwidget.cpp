#include "myopenglwidget.h"
#include "camera.h"
#include "mesh.h"
#include "mainwindow.h"
#include "hierarchy.h"
#include "entity.h"
#include "componentrender.h"
#include "componenttransform.h"
#include "vertexformat.h"
#include "submesh.h"

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

    lightColor.setX(1.0f);
    lightColor.setY(1.0f);
    lightColor.setZ(1.0f);
}

MyOpenGLWidget::~MyOpenGLWidget()
{

}

void MyOpenGLWidget::initializeGL()
{
    float vertex_attributes[20] = {
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
            1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
            -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            1.0f, -1.0f, 0.0f, 1.0f, 0.0f
        };

        unsigned int indices[6] = {
          0, 1, 2,
          0, 3, 1
        };

        VertexFormat format;
        format.setVertexAttribute(0, 0, 3);
        format.setVertexAttribute(1, 3 * sizeof(float), 2);

        quadMesh = new Mesh();
        SubMesh* newSubMesh = new SubMesh(format, &vertex_attributes[0], 20 * sizeof(float), &indices[0], 6);
        quadMesh->subMeshes.push_back(newSubMesh);
        quadMesh->update();

    initializeOpenGLFunctions();

    connect(context(), SIGNAL(aboutToBeDestroyed()), this, SLOT(finalizeGL()));

    program.create();
    program.addShaderFromSourceFile(QOpenGLShader::Vertex, "shaders/vertex_shader.vert");
    program.addShaderFromSourceFile(QOpenGLShader::Fragment, "shaders/fragment_shader.frag");
    program.link();
    InitBuffers();
    program.bind();
    program.release();

    blurProgram.create();
    blurProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, "shaders/vertex_shader_blur.vert");
    blurProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, "shaders/fragment_shader_blur.frag");
    blurProgram.link();
    blurProgram.bind();
    blurProgram.release();

    glGenTextures(1, &partialBlurTexture);
    glBindTexture(GL_TEXTURE_2D, partialBlurTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width(), height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

    glGenFramebuffers(1, &partialBlurfbo);
    glBindFramebuffer(GL_FRAMEBUFFER, partialBlurfbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D, partialBlurTexture,0);

    glGenTextures(1, &completeBlurTexture);
    glBindTexture(GL_TEXTURE_2D, completeBlurTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width(), height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

    glGenFramebuffers(1, &completeBlurFbo);
    glBindFramebuffer(GL_FRAMEBUFFER, completeBlurFbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D, completeBlurTexture,0);

    float quad[] = {
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };

    vboblur.create();
    vboblur.bind();
    vboblur.setUsagePattern((QOpenGLBuffer::UsagePattern::StaticDraw));
    vboblur.allocate(quad, 24*sizeof(float));

    vaoblur.create();
    vaoblur.bind();
    GLint compCount = 2;
    int strideBytes = 4*sizeof (float);
    int offsetBytes0 = 0;
    int offsetBytes1 = sizeof(float) * 2;
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0,compCount,GL_FLOAT, GL_FALSE, strideBytes, (void*)(offsetBytes0));
    glVertexAttribPointer(1,compCount,GL_FLOAT, GL_FALSE, strideBytes, (void*)(offsetBytes1));

    vaoblur.release();
    vboblur.release();
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

    if(mode == 3)
    {
        glClearColor(0.36f, 0.75f, 0.72f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    else
    {
        glClearColor(1.0f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }


    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    GLenum buffers[] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1,buffers);

    if(program.bind())
    {
        program.setUniformValue("projectionMatrix", mainCamera->projectionMatrix);
        QMatrix4x4 cameraTransfrom = mainCamera->viewMatrix;

        program.setUniformValue("rendererMode", mode);

        program.setUniformValue("albedoTexture", 0 );
        glBindTexture(GL_TEXTURE_2D, 0);

        program.setUniformValue("lightDir", QVector3D(xLightDir, yLightDir, zLightDir));
        program.setUniformValue("lightColor", lightColor);

        std::vector<Entity*> toDraw = MainWindow::GetWindow()->GetHierarchy()->GetEntityList();

        for(int i = 0; i < toDraw.size(); i++)
        {
            ComponentRender* rendToDraw = static_cast<ComponentRender*>(toDraw[i]->GetComponent(ComponentType::Component_Render));
            if(rendToDraw != nullptr)
            {
                makeCurrent();
                rendToDraw->WorkMeshes();
                program.setUniformValue("modelMatrix", static_cast<ComponentTransform*>(toDraw[i]->GetComponent(ComponentType::Component_Transform))->GetTransMatrix());
                program.setUniformValue("worldViewMatrix", cameraTransfrom * static_cast<ComponentTransform*>(toDraw[i]->GetComponent(ComponentType::Component_Transform))->GetTransMatrix());
                rendToDraw->Draw();
            }
        }
        program.release();

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        if(blurProgram.bind())
        {

           glBindFramebuffer(GL_FRAMEBUFFER,partialBlurfbo);
           glDrawBuffer(GL_COLOR_ATTACHMENT0);

           blurProgram.setUniformValue("albedoTexture", 0 );
           glActiveTexture(GL_TEXTURE0);
           glBindTexture(GL_TEXTURE_2D, colorTexture);

           blurProgram.setUniformValue("blurActivated", 1);
           blurProgram.setUniformValue("blurDir", QVector2D(1,1));
           blurProgram.setUniformValue("radius", 10);
           blurProgram.setUniformValue("resolution", 100);

           vaoblur.bind();
           qDebug("golis");
           makeCurrent();
           //quadMesh->draw();
           qDebug("fiu");
           vaoblur.release();

           blurProgram.release();
        }
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
    case GL_FRAMEBUFFER_COMPLETE:
        qDebug() << "Framebuffer is good :D";
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

void MyOpenGLWidget::SetRendererDisplay(int mode)
{
    this->mode = RendererMode(mode);
}

void MyOpenGLWidget::SetLightDirection(QVector3D lightDir)
{
    xLightDir = lightDir.x();
    yLightDir = lightDir.y();
    zLightDir = lightDir.z();
}

void MyOpenGLWidget::SetLightColor(QVector3D lightColor)
{
    this->lightColor = lightColor;
}
