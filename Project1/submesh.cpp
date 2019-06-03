#include "submesh.h"


SubMesh::SubMesh(VertexFormat newvertexFormat, void *newdata, int size)
{
    vertexFormat = newvertexFormat;
    data = static_cast<unsigned char*>(newdata);
    memcpy(data, newdata, size);
    dataSize = static_cast<size_t>(size);
    glfuncs = QOpenGLContext::currentContext()->functions();
}

SubMesh::SubMesh(VertexFormat newvertexFormat, void *newdata, int size, unsigned int *indices, int indices_count)
{
    glfuncs = QOpenGLContext::currentContext()->functions();

    vertexFormat = newvertexFormat;

    dataSize = static_cast<size_t>(size);
    data = new unsigned char[dataSize];
    std::memcpy(data, newdata, dataSize);

    indicesCount = static_cast<size_t>(indices_count);
    this->indices = new unsigned int[indicesCount];
    std::memcpy(this->indices, indices, indicesCount * sizeof(unsigned int));
}

void SubMesh::update()
{
    if(done == true)
        return;

    qDebug("Size of indices %u", sizeof(&indices));

    vao.create();
    vao.bind();

    vbo.create();
    vbo.bind();
    vbo.setUsagePattern(QOpenGLBuffer::UsagePattern::StaticDraw);
    vbo.allocate(data, int(dataSize));

    delete[] data;
    data = nullptr;

    if(indices != nullptr)
    {
        ibo.create();
        ibo.bind();
        ibo.setUsagePattern(QOpenGLBuffer::UsagePattern::StaticDraw);
        ibo.allocate(indices, int(indicesCount * sizeof(unsigned int)));

        delete [] indices;
        indices = nullptr;
    }

    for(int location = 0; location < MAX_VERTEX_ATTRIBUTES; location++)
    {
        VertexAttribute &attr = vertexFormat.attribute[location];

        if(attr.enabled)
        {
            glfuncs->glEnableVertexAttribArray(GLuint(location));
            glfuncs->glVertexAttribPointer(GLuint(location), attr.ncomp, GL_FLOAT,GL_FALSE
                                           , vertexFormat.size, (void *)(attr.offset));
        }
    }

    vao.release();
    vbo.release();
    if(ibo.isCreated())
    {
        ibo.release();
    }

    done = true;
}

void SubMesh::draw()
{
    int num_vertices = dataSize / vertexFormat.size;
    update();
    vao.bind();
    if(indicesCount > 0)
    {
        glfuncs->glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, nullptr);
    }
    else
    {
        glfuncs->glDrawArrays(GL_TRIANGLES, 0 , num_vertices);
    }

    vao.release();
}

void SubMesh::destroy()
{
    if(vbo.isCreated()) {vbo.destroy();}
    if(ibo.isCreated()) {ibo.destroy();}
    if(vao.isCreated()) {vao.destroy();}
}
