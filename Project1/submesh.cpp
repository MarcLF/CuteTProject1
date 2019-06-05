#include "submesh.h"
#include "myopenglwidget.h"

#include <iostream>

SubMesh::SubMesh(VertexFormat newVertexFormat, void *newData, int vSize):ibo(QOpenGLBuffer::IndexBuffer)
{
    glfuncs = QOpenGLContext::currentContext()->functions();

    vertexFormat = newVertexFormat;

    data = new unsigned char[vSize];
    memcpy(data, newData, vSize);
    dataSize = vSize;
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
}

SubMesh::~SubMesh()
{
    destroy();
}

void SubMesh::AddTexture(std::string texName)
{
    if(texture != nullptr)
    {
        delete[] texture;
    }
        texture = new QOpenGLTexture(QImage(texName.c_str()));
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
    if(texture->isCreated())
    {
        texture->bind();
    }

    int numVertices = dataSize/vertexFormat.size;
    vao.bind();
    if(indicesCount > 0)
    {
        glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, nullptr);
    }
    else {
        glDrawArrays(GL_TRIANGLES, 0, numVertices);
    }
    vao.release();
}

void SubMesh::destroy()
{
    if(vbo.isCreated()) vbo.destroy();
    if(ibo.isCreated()) ibo.destroy();
    if(vao.isCreated()) vao.destroy();
}
