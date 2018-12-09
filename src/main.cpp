#include <QApplication>
#include<QSurfaceFormat>

#include "mainwindow.h"

int main(int argc, char *argv[]){

    QSurfaceFormat format;
    //forçar o uso do openGL
    QCoreApplication::setAttribute(Qt::AA_UseDesktopOpenGL);

#ifdef Q_OS_MAC
    format.setVersion(4, 1);
    format.setProfile(QSurfaceFormat::CoreProfile);
#endif

    format.setDepthBufferSize(24);
    format.setSamples(4);

    QCoreApplication::setAttribute(Qt::AA_UseDesktopOpenGL);
    //QSurfaceFormat::setDefaultFormat(format);
    QApplication a(argc, argv);

    MainWindow w;
    w.show();
    w.initiate();

    return a.exec();
}
