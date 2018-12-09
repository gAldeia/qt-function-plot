#ifndef LIGHT_H
#define LIGHT_H

#include <QVector4D>

class Light {

    /*
     * Classe que cria as luzes do modelo. Na aplicação a luz não tem tanta influência.
     * Além disso, como a aplicação é uma ferramenta de visualização, é de interesse
     * uma aparência mais limpa em custo de iluminações elaboradas. Esta classe é importante
     * para alguns tipos de shaders disponíveis para visualização das superfícies.
     */

public:
    Light();

    QVector4D position = QVector4D(1, 1, 1, 0);
    QVector4D ambient  = QVector4D(1, 1, 1, 1);
    QVector4D diffuse  = QVector4D(1, 1, 1, 1);
    QVector4D specular = QVector4D(1, 1, 1, 1);
};

#endif // LIGHT_H
