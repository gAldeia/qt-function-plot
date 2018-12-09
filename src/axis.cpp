#include "axis.h"

Axis::Axis(char label, float scale){

    /*
     * Construtor, que inicializa a classe e o modeo para ser
     * um dos eixos.
     * Parâmetros:
     * - label: pode ser 'x', 'y' ou 'z', indica qual o
     *          eixo que essa classe Axis irá representar, e
     *          com base nisso faz as rotações necessárias
     *          para colocar uma reta do eixo.
     * - scale: dimensão da reta que será colocada. Para
     *          plot de dados em um intervalo muito grande,
     *          podemos ajustar o tamanho da reta com um valor
     *          grande de scale.
     */


    QFile file(":/models/axis.off");

    file.open(QIODevice::ReadOnly);
    QTextStream stream(&file);

    QString line;

    line = stream.readLine();

    unsigned int numVertices;
    unsigned int numFaces;

    stream >> numVertices >> numFaces >> line;

    // Criação dos vetores que guardarão os vértices e índices.
    // Note que não é preciso uma variável para o número de vértices
    // e outra para o número de faces, pois isso pode ser extraído
    // do tamanho do vetor.
    vertices = std::vector<QVector4D>(numVertices);
    indices  = std::vector<unsigned int>(numFaces * 3);

    // Leitura e criação dos vértices
    for(unsigned int i=0; i<numVertices; ++i){

        float x, y, z;

        stream >> x >> y >> z;

        vertices.at(i) = QVector4D(x, y, z, 1.0);
    }

    this->midPoint = QVector3D(0, 0, 0);

    // Leitura das faces do modelo
    for ( unsigned int i=0;i<numFaces;++i) {

        unsigned int a, b, c;

        stream >> line >> a >> b >> c;

        indices.at(i * 3 + 0) = a;
        indices.at(i * 3 + 1) = b;
        indices.at(i * 3 + 2) = c;
    }

    // Ajustando a matriz para redimensionar o modelo
    modelMatrix.setToIdentity();
    modelMatrix.scale(scale, scale, scale);

    // Ajustando a rotação da linha para diferentes eixos
    if (label == 'y')
        modelMatrix.rotate(90, 1, 0, 0);
    if (label == 'z')
        modelMatrix.rotate(90, 0, 1, 0);

    //Aplicando as mudanças de escala e rotação
    for (unsigned int i=0; i<vertices.size(); i++){

        vertices.at(i) = vertices.at(i) * modelMatrix;
    }
}

Axis::~Axis(){

    /*
     * Destrutor
     */
}

std::vector<QVector4D> Axis::get_vertices(){

    /*
     * Função que retorna um vetor contendo os vértices do eixo
     */

    std::vector<QVector4D> axisVertices = std::vector<QVector4D>();

    for(unsigned int i=0; i<vertices.size(); i++){

        axisVertices.push_back(vertices.at(i));
    }

    return axisVertices;
}

std::vector<unsigned int> Axis::get_indices(){

    /*
     * Função que retorna um vetor contendo os índices das faces do eixo
     */

    std::vector<unsigned int> axisIndices = std::vector<unsigned int>();

    for(unsigned int i=0; i<indices.size(); i++){

        axisIndices.push_back(indices.at(i));
    }

    return axisIndices;
}
