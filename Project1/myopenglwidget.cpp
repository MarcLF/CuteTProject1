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

QOpenGLFunctions_3_3_Core * gl = nullptr;

MyOpenGLWidget::MyOpenGLWidget(QWidget *parent) : QOpenGLWidget(parent)
{
    updateTimer = new QTimer(this);
    connect(updateTimer, SIGNAL(timeout()), this, SLOT(myUpdate()));
    updateTimer->start(60);

    gl = this;

    screenWidth = width();
    screenHeight = height();

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
    initializeOpenGLFunctions();

    connect(context(), SIGNAL(aboutToBeDestroyed()), this, SLOT(finalizeGL()));

    program.create();
    program.addShaderFromSourceFile(QOpenGLShader::Vertex, "shaders/vertex_shader.vert");
    program.addShaderFromSourceFile(QOpenGLShader::Fragment, "shaders/fragment_shader.frag");
    program.link();

    InitBuffers();

    blurProgram.create();
    blurProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, "shaders/BlurryVert.vert");
    blurProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, "shaders/BlurryFrag.frag");
    blurProgram.link();
    blurProgram.bind();

    float quadIndUvs[] = {-1.0f,  1.0f,  0.0f, 1.0f, -1.0f, -1.0f,  0.0f, 0.0f, 1.0f, -1.0f,  1.0f, 0.0f,
    -1.0f,  1.0f,  0.0f, 1.0f, 1.0f, -1.0f,  1.0f, 0.0f, 1.0f,  1.0f,  1.0f, 1.0f };

    vboQuadBlur.create();
    vboQuadBlur.bind();
    vboQuadBlur.setUsagePattern((QOpenGLBuffer::UsagePattern::StaticDraw));
    vboQuadBlur.allocate(quadIndUvs, 24 * sizeof(float));
    vaoQuadBlur.create();
    vaoQuadBlur.bind();
    GLint compCount = 2;
    int strideBytes = 4 * sizeof (float);
    int offsetBytes0 = 0;
    int offsetBytes1 = sizeof(float) * 2;
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0,compCount,GL_FLOAT, GL_FALSE, strideBytes, (void*)(offsetBytes0));
    glVertexAttribPointer(1,compCount,GL_FLOAT, GL_FALSE, strideBytes, (void*)(offsetBytes1));
    vaoQuadBlur.release();
    vboQuadBlur.release();

    blurProgram.release();

    depthProgram.create();
    depthProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, "shaders/DepthVert.vert");
    depthProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, "shaders/DepthFrag.frag");
    depthProgram.link();

    GenerateQuad();
}

void MyOpenGLWidget::resizeGL(int width, int height)
{
    resize(width,height);
    mainCamera->viewportWidth = width;
    mainCamera->viewportHeight = height;

    screenWidth = width;
    screenHeight = height;

    glDeleteTextures(1, &colorTexture);
    glDeleteTextures(1, &normalTexture);
    glDeleteTextures(1, &posTexture);
    glDeleteTextures(1, &depthTexture);
    glDeleteFramebuffers(1, &fbo);

    glDeleteTextures(1, &partialBlurTexture);
    glDeleteFramebuffers(1, &partialBlurFbo);
    glDeleteTextures(1, &BlurTexture);
    glDeleteFramebuffers(1, &blurFbo);

    glDeleteTextures(1, &dofTexture);
    glDeleteFramebuffers(1, &dofFbo);

    InitBuffers();
}
void MyOpenGLWidget::paintGL()
{
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

    GLenum buffers[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
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
                if(!blurIsOn && !dofIsOn)
                {
                    makeCurrent();
                }
                rendToDraw->WorkMeshes();
                program.setUniformValue("modelMatrix", static_cast<ComponentTransform*>(toDraw[i]->GetComponent(ComponentType::Component_Transform))->GetTransMatrix());
                program.setUniformValue("worldViewMatrix", cameraTransfrom * static_cast<ComponentTransform*>(toDraw[i]->GetComponent(ComponentType::Component_Transform))->GetTransMatrix());
                rendToDraw->Draw();
            }
        }
        program.release();

        if(blurIsOn)
        {
            blurShader();

            QOpenGLFramebufferObject::bindDefault();

            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            if(quadProgram.bind())
            {
                quadProgram.setUniformValue("colorTexture", 0);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, BlurTexture);

                vao.bind();
                glDrawArrays(GL_TRIANGLES, 0, 6);
                vao.release();
                quadProgram.release();
            }
        }

        else if(dofIsOn)
        {
            blurShader();
            dofShader();

            QOpenGLFramebufferObject::bindDefault();

            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            if(quadProgram.bind())
            {
                quadProgram.setUniformValue("colorTexture", 0);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, dofTexture);

                vao.bind();
                glDrawArrays(GL_TRIANGLES, 0, 6);
                vao.release();
                quadProgram.release();
            }
        }
    }
}

void MyOpenGLWidget::InitBuffers()
{

    glGenTextures(1, &colorTexture);
    glBindTexture(GL_TEXTURE_2D, colorTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, screenWidth, screenHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

    glGenTextures(1, &normalTexture);
    glBindTexture(GL_TEXTURE_2D, normalTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, screenWidth, screenHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

    glGenTextures(1, &posTexture);
    glBindTexture(GL_TEXTURE_2D, posTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, screenWidth, screenHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);    

    glGenTextures(1, &depthTexture);
    glBindTexture(GL_TEXTURE_2D, depthTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, screenWidth, screenHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D, colorTexture,0);
    glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT1,GL_TEXTURE_2D, normalTexture,0);
    glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT2,GL_TEXTURE_2D, posTexture,0);
    glFramebufferTexture2D(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D, depthTexture,0);

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

    glGenTextures(1, &partialBlurTexture);
    glBindTexture(GL_TEXTURE_2D, partialBlurTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, screenWidth, screenHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

    glGenFramebuffers(1, &partialBlurFbo);
    glBindFramebuffer(GL_FRAMEBUFFER, partialBlurFbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D, partialBlurTexture,0);

    glGenTextures(1, &BlurTexture);
    glBindTexture(GL_TEXTURE_2D, BlurTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, screenWidth, screenHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

    glGenFramebuffers(1, &blurFbo);
    glBindFramebuffer(GL_FRAMEBUFFER, blurFbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D, BlurTexture,0);

    glGenTextures(1, &dofTexture);
    glBindTexture(GL_TEXTURE_2D, dofTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, screenWidth, screenHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

    glGenFramebuffers(1, &dofFbo);
    glBindFramebuffer(GL_FRAMEBUFFER, dofFbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D, dofTexture,0);
}

void MyOpenGLWidget::GenerateQuad()
{
    float quadVerts[] = { -1.0, -1.0, 0.0,       1.0, 1.0, 1.0,      0.0f, 0.0f,
                             1.0, 1.0, 0.0,         1.0, 1.0, 1.0,      1.0f, 1.0f,
                            -1.0, 1.0, 0.0,         1.0, 1.0, 1.0,      0.0f, 1.0f,
                            -1.0, -1.0, 0.0,        1.0, 1.0, 1.0,      0.0f, 0.0f,
                            1.0, -1.0, 0.0,         1.0, 1.0, 1.0,      1.0f, 0.0f,
                            1.0, 1.0, 0.0,          1.0, 1.0, 1.0,      1.0f, 1.0f};

    quadProgram.create();
    quadProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, "Shaders/QuadVert.vert");
    quadProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, "Shaders/QuadFrag.frag");
    quadProgram.link();
    quadProgram.bind();

    vbo.create();
    vbo.bind();
    vbo.setUsagePattern((QOpenGLBuffer::UsagePattern::StaticDraw));
    vbo.allocate(quadVerts, 49 * sizeof(float));

    vao.create();
    vao.bind();
    GLint compCount = 3;
    int strideBytes = 8 * sizeof (float);
    int offsetBytes0 = 0;
    int offsetBytes1 = sizeof(float) * 3;
    const int offsetBytes2 = sizeof(float) * 6;
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(0,compCount,GL_FLOAT, GL_FALSE, strideBytes, (void*)(offsetBytes0));
    glVertexAttribPointer(1,compCount,GL_FLOAT, GL_FALSE, strideBytes, (void*)(offsetBytes1));
    glVertexAttribPointer(2,compCount,GL_FLOAT, GL_FALSE, strideBytes, (void*)(offsetBytes2));

    vao.release();
    vbo.release();

    quadProgram.release();
}

void MyOpenGLWidget::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_W)
    {
        mainCamera->Move(QVector3D(0,0,1));
    }
    else if(event->key() == Qt::Key_A)
    {
        mainCamera->Move(QVector3D(-1,0,0));
    }
    else if(event->key() == Qt::Key_S)
    {
        mainCamera->Move(QVector3D(0,0,-1));
    }
    else if(event->key() == Qt::Key_D)
    {
        mainCamera->Move(QVector3D(1,0,0));
    }
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
    {
        mainCamera->Rotate(motionX,motionY);
    }

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

void MyOpenGLWidget::SetNearPlane(float nearPl)
{
    nearPlaneValue = nearPl;
}

void MyOpenGLWidget::SetFarPlane(float farPl)
{
    farPlaneValue = farPl;
}

void MyOpenGLWidget::SetNearFallOff(float nearFO)
{
    nearFallOffValue = nearFO;
}

void MyOpenGLWidget::SetFarFallOff(float farFO)
{
    farFallOffValue = farFO;
}

float MyOpenGLWidget::GetNearPlane()
{
    return nearPlaneValue;
}

float MyOpenGLWidget::GetFarPlane()
{
    return farPlaneValue;
}

float MyOpenGLWidget::GetNearFallOff()
{
    return nearFallOffValue;
}

float MyOpenGLWidget::GetFarFallOff()
{
    return farFallOffValue;
}

void MyOpenGLWidget::SwitchBlur()
{
    blurIsOn = !blurIsOn;
    dofIsOn = false;
}

void MyOpenGLWidget::SwitchDoF()
{
    dofIsOn = !dofIsOn;
    blurIsOn = false;
}

void MyOpenGLWidget::blurShader()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    if(blurProgram.bind())
    {
       glBindFramebuffer(GL_FRAMEBUFFER, partialBlurFbo);
       glDrawBuffer(GL_COLOR_ATTACHMENT0);

       blurProgram.setUniformValue("colorTex", 0 );
       glActiveTexture(GL_TEXTURE0);
       glBindTexture(GL_TEXTURE_2D, colorTexture);

       blurProgram.setUniformValue("texCoordsLala", 1.0/screenWidth, 0);

       vaoQuadBlur.bind();
       glDrawArrays(GL_TRIANGLES, 0, 6);
       vaoQuadBlur.release();

       glBindFramebuffer(GL_FRAMEBUFFER, blurFbo);
       glDrawBuffer(GL_COLOR_ATTACHMENT0);

       blurProgram.setUniformValue("colorTex", 0 );
       glActiveTexture(GL_TEXTURE0);
       glBindTexture(GL_TEXTURE_2D, partialBlurTexture);

       blurProgram.setUniformValue("texCoordsLala", 0, 1.0/screenHeight);
       vaoQuadBlur.bind();
       glDrawArrays(GL_TRIANGLES, 0, 6);
       vaoQuadBlur.release();

       blurProgram.release();
    }
}

void MyOpenGLWidget::dofShader()
{
    glBindFramebuffer(GL_FRAMEBUFFER, dofFbo);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);
    depthProgram.bind();
    depthProgram.setUniformValue("sampleTexture", 0);
    depthProgram.setUniformValue("blurTexture", 1);
    depthProgram.setUniformValue("depthTexture", 2);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, colorTexture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, BlurTexture);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, depthTexture);

    depthProgram.setUniformValue("nearPlane",  nearPlaneValue);
    depthProgram.setUniformValue("farPlane", farPlaneValue);

    depthProgram.setUniformValue("nearFallOff",  nearFallOffValue);
    depthProgram.setUniformValue("farFallOff", farFallOffValue);

    vao.bind();
    glDrawArrays(GL_TRIANGLES, 0, 6);
    vao.release();

    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);
    depthProgram.release();
}
