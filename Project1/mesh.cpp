#include "mesh.h"
#include <qfile.h>
#include <qiodevice.h>
#include <iostream>
#include <vertexformat.h>
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "submesh.h"
#include "mainwindow.h"
#include "myopenglwidget.h"

Mesh::Mesh()
{

}

Mesh::~Mesh()
{
    qDeleteAll(subMeshes.begin(), subMeshes.end());
}

void Mesh::update()
{
        for(int i = 0; i < subMeshes.size(); i++)
        {
            subMeshes[i]->update();
        }
}

void Mesh::destroy()
{

}

void Mesh::draw()
{
    for(int i = 0; i < subMeshes.size(); i++)
    {
        subMeshes[i]->draw();
    }
}

void Mesh::addSubmesh(VertexFormat vertexFormat, void *data, int bytes, unsigned int *indexes, int bytesIndexes)
{
    SubMesh* newSubMesh = new SubMesh(vertexFormat, data, bytes, &indexes[0], bytesIndexes);

    subMeshes.push_back(newSubMesh);
}

void Mesh::loadModel(const char *filename)
{
    this->filename = filename;
    QFile f(filename);

    if(!f.open(QIODevice::ReadOnly))
    {
        std::cout << "Not Read Only" << std::endl;
        return;
    }

    //
    QByteArray data = f.readAll();

    Assimp::Importer import;

    const aiScene *scene = import.ReadFile(
                filename,
                aiProcess_Triangulate |
                aiProcess_GenSmoothNormals |
                aiProcess_FixInfacingNormals |
                aiProcess_JoinIdenticalVertices |
                aiProcess_PreTransformVertices |
                aiProcess_FlipUVs |
                aiProcess_OptimizeMeshes);

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        return;
    }

    std::cout << "ProcessNode" << std::endl;
    processNode(scene->mRootNode, scene);

    std::cout << "End Porcess Node" << std::endl;

    f.close();

    return;
}

void Mesh::processNode(aiNode *node, const aiScene *scene)
{
    // process all the node's meshes (if any)
    for(unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        subMeshes.push_back(processMesh(mesh,scene));
    }


    for(unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

SubMesh* Mesh::processMesh(aiMesh *mesh, const aiScene *scene)
{
    QVector<float> vertices;
    QVector<unsigned int> indices;

    bool hasTexCoords = false;
    bool hasTangBitang = false;

    int size = 0;
    if (mesh->HasTangentsAndBitangents()){
        hasTangBitang = true;
        size = mesh->mNumVertices*12;
    }
    else size = mesh->mNumVertices*6;

    for(int i = 0; i < mesh->mNumVertices; i++)
    {
        vertices.push_back(mesh->mVertices[i].x);
        vertices.push_back(mesh->mVertices[i].y);
        vertices.push_back(mesh->mVertices[i].z);
        vertices.push_back(mesh->mNormals[i].x);
        vertices.push_back(mesh->mNormals[i].y);
        vertices.push_back(mesh->mNormals[i].z);

        if(mesh->mTextureCoords[0])
        {
            hasTexCoords = true;
            vertices.push_back(mesh->mTextureCoords[0][i].x);
            vertices.push_back(mesh->mTextureCoords[0][i].y);
        }

        if(hasTangBitang)
        {
            vertices.push_back(mesh->mTangents[i].x);
            vertices.push_back(mesh->mTangents[i].y);
            vertices.push_back(mesh->mTangents[i].z);
            vertices.push_back(mesh->mBitangents[i].x);
            vertices.push_back(mesh->mBitangents[i].y);
            vertices.push_back(mesh->mBitangents[i].z);
        }
    }

    for(int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (int j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    VertexFormat vertexFormat;
    vertexFormat.setVertexAttribute(0,0,3);
    vertexFormat.setVertexAttribute(1,3*sizeof(float), 3);
    if(hasTexCoords)
        vertexFormat.setVertexAttribute(2,6*sizeof(float), 2);
    if(hasTangBitang)
    {
        vertexFormat.setVertexAttribute(3,9*sizeof(float),3);
        vertexFormat.setVertexAttribute(4,12*sizeof(float), 3);

    }
    SubMesh* newSubMesh = new SubMesh(vertexFormat, &vertices[0], vertices.size()*sizeof(float), &indices[0], indices.size());

    if(scene->HasMaterials())
    {
        aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];

        aiString material_path;
        scene->mMaterials[mesh->mMaterialIndex]->GetTexture(aiTextureType_DIFFUSE, 0, &material_path);

        std::string str = filename;
        int pos = str.find_last_of("/");
        str = str.substr(0, pos + 1);
        str += material_path.C_Str();

        newSubMesh->AddTexture(str);
    }

    return newSubMesh;
}


