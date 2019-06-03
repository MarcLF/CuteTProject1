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
    InitBuffers();
    program.bind();

    program.release();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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
                program.setUniformValue("worldViewMatrix", cameraTransfrom * static_cast<ComponentTransform*>(toDraw[i]->GetComponent(ComponentType::Component_Transform))->GetTransMatrix());
                rendToDraw->Draw();
            }
        }

        program.release();
    }

    QOpenGLFramebufferObject::bindDefault();
    //glBindFramebuffer(GL_FRAMEBUFFER,0);
}

void MyOpenGLWidget::InitBuffers()
{
    QOpenGLExtraFunctions* gl_functions = QOpenGLContext::currentContext()->extraFunctions();

           glGenTextures(1, &colorTexture);
           glBindTexture(GL_TEXTURE_2D, colorTexture);
           glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
           glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
           glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
           glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
           glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width(), height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

           /*glGenTextures(1, &normalTexture);
           glBindTexture(GL_TEXTURE_2D, normalTexture);
           glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
           glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
           glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
           glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
           glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width(), height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);*/

           glGenTextures(1, &depthTexture);
           glBindTexture(GL_TEXTURE_2D, depthTexture);
           glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
           glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
           glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
           glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
           glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width(), height(), 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

           gl_functions->glGenFramebuffers(1, &fbo);
           gl_functions->glBindFramebuffer(GL_FRAMEBUFFER, fbo);
           gl_functions->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexture, 0);
           gl_functions->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);
           glDrawBuffer(GL_COLOR_ATTACHMENT0);

           //GLenum buffers[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
           //gl_functions->glDrawBuffers(2, buffers);


           GLenum status = gl_functions->glCheckFramebufferStatus(GL_FRAMEBUFFER);
           switch(status)
           {
                case GL_FRAMEBUFFER_COMPLETE: // Everything's OK
                break;
                case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
                qDebug("Framebuffer ERROR: GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT"); break;
                case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
                qDebug("Framebuffer ERROR: GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT"); break;
                case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
                qDebug("Framebuffer ERROR: GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER"); break;
                case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
                qDebug("Framebuffer ERROR: GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER"); break;
                case GL_FRAMEBUFFER_UNSUPPORTED:
                qDebug("Framebuffer ERROR: GL_FRAMEBUFFER_UNSUPPORTED"); break;
                default:
                qDebug("Framebuffer ERROR: Unknown ERROR");
           }
}

void MyOpenGLWidget::myUpdate()
{
    paintGL();
    this->update();
}
