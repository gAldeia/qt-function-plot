#ifndef AXIS_H
#define AXIS_H

#include <vector>
#include <QVector4D>
#include <QFile>
#include <QtOpenGL>
#include <QTextStream>
#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include <string>

class Axis{

    /*
     * Classe para armazenar informações pertinentes a uma linha de
     * uma das coordenadas. No espaço 3D temos 3 eixos: x, y, z.
     * Para uma melhor visualização, essa classe guarda um modelo
     * utilizado para indicar um dos eixos.
     */

    std::vector<QVector4D>    vertices;
    std::vector<unsigned int> indices;

    QMatrix4x4 modelMatrix;
    QVector3D  midPoint;

public:
    Axis(char label, float scale);
    ~Axis();

    std::vector<QVector4D>    get_vertices();
    std::vector<unsigned int> get_indices();
};

#endif // AXIS_H
