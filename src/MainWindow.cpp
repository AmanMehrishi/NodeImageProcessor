#include "MainWindow.h"
#include <QGraphicsView>
#include <QVBoxLayout>
#include <QWidget>
#include <QMenuBar>
#include <QStatusBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);
    scene = new QGraphicsScene(this);
    QGraphicsView *view = new QGraphicsView(scene, this);
    view->setRenderHint(QPainter::Antialiasing);
    view->setDragMode(QGraphicsView::RubberBandDrag);
    layout->addWidget(view);
    setCentralWidget(centralWidget);
    QMenu *fileMenu = menuBar()->addMenu("File");
    fileMenu->addAction("Open");
    fileMenu->addAction("Save");
    statusBar()->showMessage("Ready");
}

MainWindow::~MainWindow() { }
