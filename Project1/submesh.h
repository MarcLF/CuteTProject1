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

    void AddTexture(std::string texname);

public:
    unsigned char *data;
    size_t dataSize = 0;

    unsigned int *indices;
    size_t indicesCount = 0;

    VertexFormat vertexFormat;
    QOpenGLBuffer vbo;
    QOpenGLBuffer ibo;
    QOpenGLVertexArrayObject vao;

    QOpenGLFunctions *glfuncs = nullptr;

    QOpenGLTexture* texture;

    bool done = false;
};

#endif // SUBMESH_H
