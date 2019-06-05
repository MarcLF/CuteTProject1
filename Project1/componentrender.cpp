#include "componentrender.h"
#include "ui_componentrender.h"
#include "mesh.h"

#define H 32
#define V 16

ComponentRender::ComponentRender(QWidget *parent) :
    Component (ComponentType::Component_Render, parent),
    ui(new Ui::ComponentRender)
{
    ui->setupUi(this);

    connect(ui->modelSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(ChangeModel(int)));
}

ComponentRender::~ComponentRender()
{
    delete ui;
}

void ComponentRender::AddMesh(const char *fileName)
{
    if(QString::compare(fileName, "Sphere", Qt::CaseInsensitive) == 0)
    {
        static const float pi = 3.1416f;
        struct Vertex {QVector3D pos; QVector3D norm;};

        Vertex sphere[H][V+1];
        for(int h = 0; h < H; ++h)
        {
            for(int v = 0; v < V + 1; ++v)
            {
                float nh = float(h) / H;
                float nv = float(v) / V;
                float angleh = 2 * pi * nh;
                float anglev = - pi * nv;
                sphere[h][v].pos.setX(sinf(angleh) * cosf(anglev));
                sphere[h][v].pos.setY(-sinf(anglev));
                sphere[h][v].pos.setZ(cosf(angleh) * cosf(anglev));
                sphere[h][v].norm = sphere[h][v].pos;
            }
        }

        unsigned int sphereIndices[H][V][6];
        for(unsigned int h = 0; h < H; ++h)
        {
            for(unsigned int v = 0; v < V; ++v)
            {
                sphereIndices[h][v][0] = (h+0) * (V+1) + v;
                sphereIndices[h][v][1] = ((h+1)%H) * (V+1) + v;
                sphereIndices[h][v][2] = ((h+1)%H) * (V+1) + v + 1;
                sphereIndices[h][v][3] = (h+0) * (V+1) + v;
                sphereIndices[h][v][4] = ((h+1)%H) * (V+1) + v + 1;
                sphereIndices[h][v][5] = (h+0) * (V+1) + v + 1;
            }
        }

        VertexFormat vertexForm;
        vertexForm.setVertexAttribute(0, 0, 3);
        vertexForm.setVertexAttribute(1, sizeof(QVector3D), 3);

        myMesh = new Mesh();
        SubMesh* sub = new SubMesh(vertexForm, sphere, sizeof(sphere), &sphereIndices[0][0][0], H*V*6);
        myMesh->subMeshes.push_back(sub);
    }
    else
    {
        //myMesh = new Mesh();
        //myMesh->loadModel("Models/Patrick.obj");
    }
}

void ComponentRender::Draw()
{
    if(myMesh != nullptr)
    {
        myMesh->draw();
    }
}

void ComponentRender::WorkMeshes()
{
    if(myMesh != nullptr)
    {
        myMesh->update();
    }
}

void ComponentRender::ChangeModel(int index)
{
    if(myMesh != nullptr)
    {
        delete myMesh;
    }
    myMesh = new Mesh();
    switch(index)
    {
    case 0:
        return;
        break;
    case 1:
        myMesh->loadModel("Models/Patrick.obj");
        break;
    case 2:
        myMesh->loadModel("Models/sponza.obj");
    case 3:
        myMesh->loadModel("Models/PalmTree.obj");
    }
}
