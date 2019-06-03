#ifndef VERTEXFORMAT_H
#define VERTEXFORMAT_H

static const int MAX_VERTEX_ATTRIBUTES = 8;

struct VertexAttribute
{
    bool enabled = false;
    int offset = 0;
    int ncomp = 0;
};

class VertexFormat
{
public:

    void setVertexAttribute(int location, int offset, int ncomp)
    {
        attribute[location].enabled = true;
        attribute[location].offset = offset;
        attribute[location].ncomp = ncomp;
        size += ncomp * sizeof(float);
    }

    VertexAttribute attribute[3];
    int size = 0;

};

#endif // VERTEXFORMAT_H
