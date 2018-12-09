#ifndef MODEL_H
#define MODEL_H

#include <QtOpenGL>
#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include <QTextStream>
#include <QFile>

#include <fstream>
#include <limits>
#include <cstdlib>
#include <iostream>
#include <memory>

#include "material.h"
#include "trackball.h"
#include "cube.h"
#include "axis.h"

using namespace std;

class Model : public QOpenGLExtraFunctions {

public:    
    Model(QOpenGLWidget *_glWidget);
    ~Model();

    TrackBall trackBall;

    unique_ptr<QVector4D []>   vertices = nullptr;
    unique_ptr<QVector4D []>   colors   = nullptr;
    unique_ptr<unsigned int[]> indices  = nullptr;
    unique_ptr<QVector3D []>   normals  = nullptr; //lights

    QOpenGLWidget *glWidget;

    unsigned int numVertices = 0;
    unsigned int numFaces    = 0;
    unsigned int shaderIndex = 0;
    unsigned int numShaders  = 0;
    unsigned int numColors   = 0;
    unsigned int funcIndex   = 0;

    GLuint vao         = 0;
    GLuint vboVertices = 0;
    GLuint vboIndices  = 0;
    GLuint vboColors   = 0;
    GLuint vboNormals  = 0;

    //light stuff
    Material material;

    vector<GLuint> shaderProgram;

    QMatrix4x4 modelMatrix;
    QVector3D  midPoint;

    double invDiag;

    vector<Cube> cubes;
    vector<Axis> axis;

    void createNormals();

    void createVBOs();
    void createShaders();

    void destroyVBOs();
    void destroyShaders();

    void drawModel();
    void changeFunction(unsigned int);
    void createFunctionPoints();
};

#endif
//MODEL_H
