#include "openglwidget.h"

OpenGLWidget::OpenGLWidget(QWidget * parent) : QOpenGLWidget(parent){

    connect(& timer, SIGNAL(timeout()), this,SLOT(animate()));
    timer.start(0);
}

OpenGLWidget::~OpenGLWidget(){

    // destroyVBOs();
    // destroyShaders();
}

void OpenGLWidget::initializeGL() {

    initializeOpenGLFunctions();

    qDebug("OpenGL version: %s",glGetString(GL_VERSION));
    qDebug("GLSL %s",glGetString(GL_SHADING_LANGUAGE_VERSION));

    glEnable(GL_DEPTH_TEST);
}

void OpenGLWidget::resizeGL(int w, int h){

    glViewport(0, 0, w, h);

    camera.resizeViewport(w, h);

    if (model)
        model->trackBall.resizeViewport(w, h);

    update();
}

void OpenGLWidget::changeShader(int shaderIndex){

    if (!model)
        return;

    model->shaderIndex = shaderIndex;
    update();
}

void OpenGLWidget::paintGL() {

    glClearColor(1,1,1,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if(!model)
        return;

    GLuint shaderProgramID = model->shaderProgram[model->shaderIndex];
    glUseProgram(shaderProgramID);

    QVector4D ambientProduct = light.ambient * model->material.ambient;
    QVector4D diffuseProduct = light.diffuse *  model->material.diffuse;
    QVector4D specularProduct = light.specular *  model->material.specular;

    GLint locProjection = glGetUniformLocation(shaderProgramID,"projection");
    GLint locView = glGetUniformLocation(shaderProgramID,"view");

    GLint locLightPosition = glGetUniformLocation(shaderProgramID,"lightPosition");
    GLint locAmbientProduct = glGetUniformLocation(shaderProgramID,"ambientProduct");
    GLint locDiffuseProduct = glGetUniformLocation(shaderProgramID,"diffuseProduct");
    GLint locSpecularProduct = glGetUniformLocation(shaderProgramID,"specularProduct");

    glUniformMatrix4fv(locProjection,1,GL_FALSE,camera.projectionMatrix.data());
    glUniformMatrix4fv(locView, 1,GL_FALSE, camera.viewMatrix.data());

    glUniform4fv(locLightPosition, 1,   &(light.position[0]));
    glUniform4fv(locAmbientProduct, 1,  &(ambientProduct[0]));
    glUniform4fv(locDiffuseProduct, 1,  &(diffuseProduct[0]));
    glUniform4fv(locSpecularProduct, 1, &(specularProduct[0]));

    model->drawModel();
}

void OpenGLWidget::initiate(){

    model = std::make_shared<Model>(this);

    model->shaderIndex = 0;
    model->funcIndex = 0;

    model->createFunctionPoints();
    model->trackBall.resizeViewport(width(), height());
}

void OpenGLWidget::showFileOpenDialog(int funcIndex) {

    model->funcIndex = funcIndex;

    model->createFunctionPoints();
    model->trackBall.resizeViewport(width(), height());

    emit statusBarMessage(QString("Vertices: \%1, Faces: \%2").arg(model->numVertices).arg(model->numFaces));
    emit enableComboShaders(true);

    update();
}

void OpenGLWidget::animate(){

    update();
}

void OpenGLWidget::mouseMoveEvent(QMouseEvent *event){

    if (!model)
        return;

    model->trackBall.mouseMove(event->localPos());
}

void OpenGLWidget::mousePressEvent(QMouseEvent *event) {

    if (!model)
        return;

    if (event->button() & Qt::LeftButton)
        model->trackBall.mousePress(event->localPos());
}

void OpenGLWidget::mouseReleaseEvent(QMouseEvent *event) {

    if (!model)
        return;

    if (event->button() & Qt::LeftButton)
        model->trackBall.mouseRelease(event->localPos());
}
