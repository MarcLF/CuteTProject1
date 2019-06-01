#ifndef SUBMESH_H
#define SUBMESH_H

#include "vertexformat.h"

#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLFramebufferObject>
#include<QtOpenGL/QtOpenGL>

class SubMesh
{
public:
    SubMesh(VertexFormat newvertexFormat, void *newdata, int size);
    SubMesh(VertexFormat newvertexFormat, void *newdata, int size, unsigned int *indices, int indices_count);
    ~SubMesh();

    void update();
    void draw();
    void destroy();

private:
    unsigned char *data = nullptr;
    size_t dataSize = 0;

    unsigned int *indices = nullptr;
    size_t indicesCount = 0;

    VertexFormat vertexFormat;
    QOpenGLBuffer vbo;
    QOpenGLBuffer ibo;
    QOpenGLVertexArrayObject vao;

    QOpenGLFunctions *glfuncs = nullptr;
};

#endif // SUBMESH_H
