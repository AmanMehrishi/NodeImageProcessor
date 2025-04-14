#include "MainWindow.h"
#include <QGraphicsView>
#include <QVBoxLayout>
#include <QWidget>
#include <QMenuBar>
#include <QStatusBar>
#include <QDockWidget>
#include <QFormLayout>
#include <QLabel>
#include <QSlider>
#include <QPushButton>
#include <QAction>
#include <QFileDialog>
#include <QMessageBox>

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


    createMenu();
    createPropertiesPanel();

    statusBar()->showMessage("Ready");
}

MainWindow::~MainWindow() { }

void MainWindow::createMenu(){
    QMenu *fileMenu = menuBar() -> addMenu("File");

    QAction *openAct = new QAction("Open Image", this);
    QAction *saveAct = new QAction("Save Image", this);

    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);

    connect(openAct, &QAction::triggered, [this](){
        QString filename = QFileDialog::getOpenFileName(this, "Open Image", "", "Images (*.png *.jpg *bmp)");
        if(!filename.isEmpty())
        {
            QMessageBox::information(this, "Open", "Image opened: "+filename);
        }
    });
    connect(saveAct, &QAction::triggered, [this](){
        QString filename = QFileDialog::getSaveFileName(this, "Save Image", "", "Images (*.png *.jpg *bmp)");
        if(!filename.isEmpty())
        {
            QMessageBox::information(this, "Save", "Result saved to: "+filename);
        }
    });
}

void MainWindow::createPropertiesPanel(){
    QDockWidget *propertiesDock = new QDockWidget("Properties", this);
    propertiesWidget = new QWidget(propertiesDock);

    QFormLayout *formLayout = new QFormLayout(propertiesWidget);
    
    QLabel *nodeNameLabel = new QLabel("No node selected", propertiesWidget);
    formLayout->addRow("Node", nodeNameLabel);
    
    QSlider *brightnessSlider = new QSlider(Qt::Horizontal, propertiesWidget);
    brightnessSlider->setRange(-100, 100);
    formLayout->addRow("Brightness", brightnessSlider);
    
    QSlider *contrastSlider = new QSlider(Qt::Horizontal, propertiesWidget);
    contrastSlider->setRange(0, 300); 
    formLayout->addRow("Contrast", contrastSlider);
    
    QPushButton *resetButton = new QPushButton("Reset", propertiesWidget);
    formLayout->addRow(resetButton);

    propertiesWidget->setLayout(formLayout);
    propertiesDock->setWidget(propertiesWidget);
    addDockWidget(Qt::RightDockWidgetArea, propertiesDock);
}