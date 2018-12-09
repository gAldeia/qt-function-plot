#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include<QWidget>
#include<QOpenGLWidget>
#include<QOpenGLExtraFunctions>

#include "model.h"
#include "light.h"
#include "camera.h"

class OpenGLWidget : public QOpenGLWidget, protected QOpenGLExtraFunctions {

    Q_OBJECT

public:
    OpenGLWidget(QWidget *parent = 0);
    ~OpenGLWidget();

    void mouseMoveEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);

    QTimer timer;

    std::shared_ptr<Model> model = nullptr;

    Camera camera;
    Light light;

    QMatrix4x4 lightModelMatrix;

protected:
    void initializeGL();
    void resizeGL(int width, int height);

signals:
    void statusBarMessage(QString);
    void enableComboShaders(bool);

public slots:
    void initiate();
    void showFileOpenDialog(int);
    void changeShader(int);
    void paintGL();
    void animate();
};

#endif
//OPENGLWIDGET_H
