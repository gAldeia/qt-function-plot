#include "model.h"

Model::Model(QOpenGLWidget *_glWidget){

    glWidget = _glWidget;
    glWidget->makeCurrent();

    initializeOpenGLFunctions();

    axis = std::vector<Axis>();

    std::vector<char> axis_labels = {'x', 'y', 'z'};

    for(unsigned int i=0; i<axis_labels.size(); i++){
        axis.push_back( Axis(axis_labels.at(i), 0.25) );
    }
}

Model::~Model(){

    destroyVBOs();
    destroyShaders();
}

void Model::destroyVBOs() {

    glDeleteBuffers(1, &vboVertices);
    glDeleteBuffers(1, &vboIndices);
    glDeleteBuffers(1, &vboColors);
    glDeleteBuffers(1, &vboNormals);

    glDeleteVertexArrays(1, &vao);

    vboVertices = 0;
    vboIndices = 0;
    vboNormals = 0;
    vboColors = 0;
    vao = 0;
}

void Model::destroyShaders () {

    for(GLuint shaderProgramID : shaderProgram){

        glDeleteProgram(shaderProgramID);
    }

    shaderProgram.clear();
}

void Model::createVBOs() {

    glWidget->makeCurrent();
    destroyVBOs();

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vboVertices);
    glBindBuffer(GL_ARRAY_BUFFER, vboVertices);
    glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(QVector4D), vertices.get(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &vboNormals);
    glBindBuffer(GL_ARRAY_BUFFER, vboNormals);
    glBufferData(GL_ARRAY_BUFFER, numFaces * sizeof(QVector3D), normals.get(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &vboColors);
    glBindBuffer(GL_ARRAY_BUFFER, vboColors);
    glBufferData(GL_ARRAY_BUFFER, numColors * sizeof(QVector4D), colors.get(), GL_STATIC_DRAW);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, nullptr);

    glEnableVertexAttribArray(2);

    glGenBuffers(1, &vboIndices);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndices);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, numFaces * 3 * sizeof(unsigned int), indices.get(), GL_STATIC_DRAW);
}


void Model::createShaders() {

    // makeCurrent ();

    QString vertexShaderFile[]  = {":/shaders/vshader1.glsl",
                                   ":/shaders/vflat.glsl",
                                   ":/shaders/vgouraud.glsl",
                                   ":/shaders/vphong.glsl",
                                   ":/shaders/vnormal.glsl"
                                  };
    QString fragmentShaderFile[] = {":/shaders/fshader2.glsl",
                                    ":/shaders/fflat.glsl",
                                    ":/shaders/fgouraud.glsl",
                                    ":/shaders/fphong.glsl",
                                    ":/shaders/fnormal.glsl"
                                   };

    numShaders = sizeof(vertexShaderFile)/sizeof(vertexShaderFile[0]);

    destroyShaders();
    shaderProgram.clear();

    for(unsigned int i=0; i<numShaders; ++i){

        qDebug("criando shader n %d", i);

        QFile vs(vertexShaderFile[i]);
        QFile fs(fragmentShaderFile[i]);

        vs.open(QFile::ReadOnly | QFile::Text);
        fs.open(QFile::ReadOnly | QFile::Text);

        QTextStream streamVs(&vs), streamFs(&fs);

        QString qtStringVs = streamVs.readAll();
        QString qtStringFs = streamFs.readAll();

        std::string stdStringVs = qtStringVs.toStdString();
        std::string stdStringFs = qtStringFs.toStdString();

        // Create an empty vertex shader handle
        GLuint vertexShader = 0;
        vertexShader = glCreateShader(GL_VERTEX_SHADER);

        // Send the vertex shader source code to GL
        const GLchar *source = stdStringVs.c_str();
        glShaderSource(vertexShader, 1, &source, nullptr);

        // Compile the vertex shader
        glCompileShader(vertexShader);
        GLint isCompiled = 0;
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);

        if (isCompiled == GL_FALSE){

            GLint maxLength = 0;
            glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);
            qDebug("/%s", &infoLog[0]);

            glDeleteShader(vertexShader);

            return;
        }

        // Create an empty fragment shader handle
        GLuint fragmentShader = 0;
        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

        // Send the fragment shader source code to GL
        // Note that std :: string ’s . c_str is null terminated .
        source = stdStringFs.c_str();
        glShaderSource(fragmentShader, 1, &source, 0);

        // Compile the fragment shader
        glCompileShader(fragmentShader);
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);

        if (isCompiled == GL_FALSE){

            GLint maxLength = 0;
            glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);
            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);
            qDebug("/%s", &infoLog[0]);
            glDeleteShader(fragmentShader);
            glDeleteShader(vertexShader);
            return;
        }

        GLuint shaderProgramID = 0;
        shaderProgramID = glCreateProgram();
        shaderProgram.push_back(shaderProgramID);

        // Attach our shaders to our program
        glAttachShader(shaderProgramID, vertexShader);
        glAttachShader(shaderProgramID, fragmentShader);

        //Link our program
        glLinkProgram(shaderProgramID);

        //Note the different functions here : glGetProgram *
        // instead of glGetShader *.
        GLint isLinked = 0;

        glGetProgramiv(shaderProgramID, GL_LINK_STATUS, (int *)&isLinked);

        if (isLinked == GL_FALSE){

            GLint maxLength = 0;

            glGetProgramiv(shaderProgramID, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(shaderProgramID, maxLength, &maxLength,
            &infoLog[0]);
            qDebug( " % s " ,&infoLog[0]);

            glDeleteProgram(shaderProgramID);
            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);

            return ;
        }

        glDetachShader(shaderProgramID, vertexShader);
        glDetachShader(shaderProgramID, fragmentShader);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        vs.close();
        fs.close();
    }

}

void Model::createNormals() {

    normals = std::make_unique<QVector3D[]>(numFaces);

    for ( unsigned int i=0;i<numFaces;++i){

        QVector3D a = QVector3D(vertices[indices[i * 3 + 0]]);
        QVector3D b = QVector3D(vertices[indices[i * 3 + 1]]);
        QVector3D c = QVector3D(vertices[indices[i * 3 + 2]]);

        // Not normalizing ( weighted average )
        QVector3D faceNormal = QVector3D::crossProduct((b - a), (c - b));

        // Accumulates face normals on the vertices
        normals[indices[i * 3 + 0]] += faceNormal;
        normals[indices[i * 3 + 1]] += faceNormal;
        normals[indices[i * 3 + 2]] += faceNormal;
    }

    for(unsigned int i=0;i<numFaces;++i) {

        normals[i].normalize();
    }
}

void Model::drawModel() {

    modelMatrix.setToIdentity();
    modelMatrix.scale(invDiag, invDiag, invDiag);

    //modelMatrix.translate(-midPoint);

    modelMatrix.rotate(trackBall.getRotation());

    GLuint locModel = 0;
    GLuint locNormalMatrix = 0;
    GLuint locShininess = 0;

    glUseProgram(shaderProgram[shaderIndex]);

    locModel = glGetUniformLocation(shaderProgram[shaderIndex],"model");
    locNormalMatrix = glGetUniformLocation(shaderProgram[shaderIndex],"normalMatrix");
    locShininess = glGetUniformLocation(shaderProgram[shaderIndex],"shininess");

    glBindVertexArray(vao);

    glUniformMatrix4fv(locModel, 1, GL_FALSE, modelMatrix.data());
    glUniformMatrix3fv(locNormalMatrix, 1, GL_FALSE,modelMatrix.normalMatrix().data());
    glUniform1f(locShininess,static_cast<GLfloat>(material.shininess));
    glDrawElements(GL_TRIANGLES, numFaces * 3, GL_UNSIGNED_INT, 0);
}

void Model::changeFunction(unsigned int index){

    funcIndex = index;
    createFunctionPoints();
}

void Model::createFunctionPoints(){

    float xRange = 2.5;
    float zRange = 2.5;
    float step = 0.1;

    cubes = std::vector<Cube>();

    float minLim = std::numeric_limits<float>::lowest();
    float maxLim = std::numeric_limits<float>::max();

    QVector4D max(minLim, minLim, minLim, 1.0);
    QVector4D min(maxLim, maxLim, maxLim, 1.0);


    float y = 0.0;

    // Criação dos cubos, calculando y para cada tupla (x, z)
    // Dentro do intervalo definido
    if (funcIndex != 0){
        for (float x=-xRange+step; x<xRange; x = x+step){
            for (float z=-zRange+step; z<zRange; z = z+step){

                if (funcIndex == 1)
                    y = x*x*z*z/5 - 2;

                else if (funcIndex == 2)
                    y = sin(x) * z;

                else if (funcIndex == 3)

                    y = 1.25*sin(x+z);

                else if (funcIndex == 4)
                    y = 0.5*sin(x*x + z*z);

                else if (funcIndex == 5)
                    y = abs(x) + abs(z) - 2;

                else if (funcIndex == 6)
                    y = sin(x)*cos(z);

                max.setY(std::max(max.y(), y));
                min.setY(std::min(min.y(), y));

                cubes.push_back(Cube(x, y, z, 0.01));
            }
        }
    }

    numVertices = 8*cubes.size() + 8*axis.size();
    numFaces    = 12*cubes.size() + 12*axis.size();
    numColors   = numVertices;

    vertices = std::make_unique<QVector4D[]>(numVertices);
    colors   = std::make_unique<QVector4D[]>(numColors);
    indices  = std::make_unique<unsigned int[]>(numFaces * 3);

    // Carregando informações dos eixos
    for(unsigned int i=0; i<axis.size(); i++){

        std::vector<QVector4D> axisVertices = axis.at(i).get_vertices();
        std::vector<unsigned int> axisIndices = axis.at(i).get_indices();

        for(unsigned int j=0; j<8; j++){

            vertices[i*8 + j] = axisVertices.at(j);
            colors[i*8 + j] = QVector4D(0, 0, 0, 0);
        }

        for(unsigned int j=0; j<12; j++){

            indices[i*36 + j*3 + 0] = axisIndices.at(j*3 + 0) + i*8;
            indices[i*36 + j*3 + 1] = axisIndices.at(j*3 + 1) + i*8;
            indices[i*36 + j*3 + 2] = axisIndices.at(j*3 + 2) + i*8;
        }
    }

    float y_map = max.y() - min.y();

    // Carregando informações dos cubinhos. Somamos 3 ao i
    // para pular os espaços já ocupados pelos eixos.
    for(unsigned int i=0; i<cubes.size(); i++){

        std::vector<QVector4D> cubeVertices = cubes.at(i).get_vertices();
        std::vector<unsigned int> cubeIndices = cubes.at(i).get_indices();

        for(unsigned int j=0; j<8; j++){

            vertices[(i+3)*8 + j] = cubeVertices.at(j);
            colors[(i+3)*8 + j] = QVector4D(0, vertices[(i+3)*8 + j].y()*y_map, 0, 1.0);
        }

        for(unsigned int j=0; j<12; j++){

            indices[(i+3)*36 + j*3 + 0] = cubeIndices.at(j*3 + 0) + i*8;
            indices[(i+3)*36 + j*3 + 1] = cubeIndices.at(j*3 + 1) + i*8;
            indices[(i+3)*36 + j*3 + 2] = cubeIndices.at(j*3 + 2) + i*8;
        }
    }

    this->midPoint = QVector3D(0,0,0);
    this->invDiag = 0.85;

    createNormals();
    createShaders();
    createVBOs();
}
