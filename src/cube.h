#ifndef CUBE_H
#define CUBE_H

#include <vector>
#include <QVector4D>
#include <QFile>
#include <QtOpenGL>
#include <QTextStream>
#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include <string>

class Cube {

    /*
     * Classe que representa um cubo 3D. Para plotar a função,
     * são gerados vários cubos 3D no espaço, um em cada ponto,
     * indicando o valor da função (y) em relação às coordenadas
     * x e z. No espaço 3D o y é a coordenada que define a altura
     * do cubo, por isso a função é f(x, z) = y.
     */

private:
    std::vector<QVector4D>    vertices;
    std::vector<unsigned int> indices;

    QMatrix4x4 modelMatrix;
    QVector3D  midPoint;

public:
    Cube(float xPos, float yPos, float zPos, float scale);
    ~Cube();

    std::vector<QVector4D> get_vertices();
    std::vector<unsigned int> get_indices();
};

#endif // CUBE_H
