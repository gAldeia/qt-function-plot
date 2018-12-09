#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);

    connect(ui->function1, SIGNAL(clicked()), this, SLOT(changeFunction()));
    connect(ui->function2, SIGNAL(clicked()), this, SLOT(changeFunction()));
    connect(ui->function3, SIGNAL(clicked()), this, SLOT(changeFunction()));
    connect(ui->function4, SIGNAL(clicked()), this, SLOT(changeFunction()));
    connect(ui->function5, SIGNAL(clicked()), this, SLOT(changeFunction()));
    connect(ui->function6, SIGNAL(clicked()), this, SLOT(changeFunction()));
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::initiate(){

    ui->openGLWidget->initiate();
}

void MainWindow::changeFunction(){

    QString name = QObject::sender()->objectName();
    QStringList coords = name.split(QRegExp("[^0-9]"), QString::SkipEmptyParts);

    int i = coords[0].toInt();

    ui->openGLWidget->model->funcIndex = i;
    ui->openGLWidget->showFileOpenDialog(i);

    qDebug("Função selecionada: %d", i);
}
