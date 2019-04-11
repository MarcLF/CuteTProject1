#ifndef MESH_H
#define MESH_H

#include "resource.h"
#include "vertexformat.h"
#include "submesh.h"

#include <QVector>

struct aiNode;
struct aiMesh;
struct aiScene;

class Mesh : public Resource
{
public:
    Mesh();
    ~Mesh() override;

    void update() override;
    void destroy() override;

    void addSubmesh(VertexFormat vertexFormat, void *data, int bytes);
    void addSubmesh(VertexFormat vertexFormat, void *data, int bytes, unsigned int *indexes, int bytesIndexes);

    QVector<SubMesh*> subMeshes;

private:
    //Assimp

   void processNode(aiNode *node, const aiScene *scene);
   SubMesh *processMesh(aiMesh *mesh, const aiScene *scene);

};

#endif // MESH_H
