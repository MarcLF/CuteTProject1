#include "submesh.h"

#include <iostream>

SubMesh::SubMesh(VertexFormat newVertexFormat, void *newData, int vSize):ibo(QOpenGLBuffer::IndexBuffer)
{
    glfuncs = QOpenGLContext::currentContext()->functions();

    vertexFormat = newVertexFormat;

    data = new unsigned char[vSize];
    memcpy(data, newData, vSize);
    dataSize = vSize;

    texture = new QOpenGLTexture(QImage(QString("images/test.png")));
}

SubMesh::SubMesh(VertexFormat newVertexFormat, void *newData, int vSize, unsigned int *newIndices, int iSize) : ibo(QOpenGLBuffer::IndexBuffer)
{
    glfuncs = QOpenGLContext::currentContext()->functions();

    vertexFormat = newVertexFormat;

    data = new unsigned char[vSize];
    memcpy(data, newData, vSize);
    dataSize = vSize;

    indices = new unsigned int[iSize];
    memcpy(indices, newIndices, iSize * sizeof (unsigned int));
    indicesCount = iSize;

    texture = new QOpenGLTexture(QImage(QString("images/test.png")));
}

SubMesh::~SubMesh()
{
    destroy();
}

void SubMesh::update()
{
    if(vao.isCreated())
    {
        return;
    }
    vao.create();
    vao.bind();

    vbo.create();
    vbo.bind();
    vbo.setUsagePattern(QOpenGLBuffer::UsagePattern::StaticDraw);
    vbo.allocate(data,int(dataSize));

    delete[] data;
    data = nullptr;

    if(indices != nullptr)
    {
        ibo.create();
        ibo.bind();
        ibo.setUsagePattern(QOpenGLBuffer::UsagePattern::StaticDraw);
        ibo.allocate(indices, int(indicesCount*sizeof(unsigned int)));

        delete[] indices;
        indices = nullptr;
    }

    for(int location = 0;  location < MAX_VERTEX_ATTRIBUTES; location++)
    {
        VertexAttribute &attr = vertexFormat.attribute[location];

        if(attr.enabled)
        {
            glfuncs->glEnableVertexAttribArray(GLuint(location));
            glfuncs->glVertexAttribPointer(GLuint(location), attr.ncomp, GL_FLOAT, GL_FALSE, vertexFormat.size, (void*)(attr.offset));
        }
    }

    vao.release();
    vbo.release();
    if(ibo.isCreated())
        ibo.release();
}

void SubMesh::draw()
{
    if(!vao.isCreated())
    {
        return;
    }

    texture->bind();

    glfuncs = QOpenGLContext::currentContext()->functions();

    int numVertices = dataSize/vertexFormat.size;
    vao.bind();
    if(indicesCount > 0)
    {
        glfuncs->glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, nullptr);
    }
    else {
        glfuncs->glDrawArrays(GL_TRIANGLES, 0, numVertices);
    }
    vao.release();

    glfuncs = QOpenGLContext::currentContext()->functions();
}

void SubMesh::destroy()
{
    if(vbo.isCreated()) vbo.destroy();
    if(ibo.isCreated()) ibo.destroy();
    if(vao.isCreated()) vao.destroy();
}
