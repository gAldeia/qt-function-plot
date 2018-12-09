#include "cube.h"

Cube::Cube(float xPos, float yPos, float zPos, float scale){

    /*
     * Constutor, recebe a posição onde o cubo deve estar no espaço,
     * e um fator de escala. Guarda todas as informações geométricas
     * para desenhar um cubo na posição desejada.
     * Parâmetros:
     * - xPos:  posição x onde o cubo deve estar;
     * - yPos:  posição y onde o cubo deve estar;
     * - zPos:  posição z onde o cubo deve estar
     * - scale: fator de redimensionamento.
     */

    QFile file(":/models/cube.off");

    file.open(QIODevice::ReadOnly);
    QTextStream stream(&file);

    QString line;

    line = stream.readLine();

    unsigned int numVertices;
    unsigned int numFaces;

    stream >> numVertices >> numFaces >> line;

    vertices = std::vector<QVector4D>(numVertices);
    indices  = std::vector<unsigned int>(numFaces * 3);

    // Leitura dos vértices do modelo do cubo
    for(unsigned int i=0; i<numVertices; ++i){

        float x, y, z;

        stream >> x >> y >> z;

        vertices.at(i) = QVector4D(x, y, z, 1.0);
    }

    // O cubo estará centrado na posição passada como parâmetro
    this->midPoint = QVector3D(xPos, yPos, zPos);

    // Criação da matriz que fará a escala do cubo
    modelMatrix.setToIdentity();
    modelMatrix.scale(scale, scale, scale);

    // Leitura dos índices do modelo do cubo
    for ( unsigned int i=0;i<numFaces;++i) {

        unsigned int a, b, c;

        stream >> line >> a >> b >> c;

        indices.at(i * 3 + 0) = a;
        indices.at(i * 3 + 1) = b;
        indices.at(i * 3 + 2) = c;
    }

    // Criação do vetor que conterá a posição do cubo
    QVector4D newPos(xPos, yPos, zPos, 1.0);

    for (unsigned int i=0; i<vertices.size(); i++){

        // Aplicando a escala
        vertices.at(i) = vertices.at(i) * modelMatrix;

        //Movendo o cubo para a posição desejada
        vertices.at(i) = vertices.at(i) + newPos;
    }
}

Cube::~Cube(){

    /*
     * Destrutor.
     */

}

std::vector<QVector4D> Cube::get_vertices(){

    /*
     * Função que retorna um vetor contendo os vértices do eixo
     */

    std::vector<QVector4D> cubeVertices = std::vector<QVector4D>();

    for(unsigned int i=0; i<vertices.size(); i++){

        cubeVertices.push_back(vertices.at(i));
    }

    return cubeVertices;
}

std::vector<unsigned int> Cube::get_indices(){

    /*
     * Função que retorna um vetor contendo os índices do eixo
     */

    std::vector<unsigned int> cubeIndices = std::vector<unsigned int>();

    for(unsigned int i=0; i<indices.size(); i++){

        cubeIndices.push_back(indices.at(i));
    }

    return cubeIndices;
}
