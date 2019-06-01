#ifndef MESH_H
#define MESH_H

#include "resource.h"
#include "vertexformat.h"
#include "submesh.h"

#include <QVector>
#include <assimp/scene.h>

struct aiNode;
struct aiMesh;
struct aiScene;

class Mesh : public Resource
{
public:
    Mesh();
    ~Mesh() override;
    Mesh *	asMesh(){	return this;	}

    void update() override;
    void destroy() override;

    void addSubmesh(VertexFormat vertexFormat, void *data, int bytes);
    void addSubmesh(VertexFormat vertexFormat, void *data, int bytes, unsigned int *indexes, int bytesIndexes);
    void loadModel(const char *filename);
    QVector<SubMesh*> subMeshes;

    bool needsUpdate = false;

private:
    //Assimp

   void processNode(aiNode *node, const aiScene *scene);
   SubMesh *processMesh(aiMesh *mesh, const aiScene *scene);
   const char *filename = nullptr;

};

#endif // MESH_H
