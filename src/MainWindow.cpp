#include "MainWindow.h"
#include <QGraphicsView>
#include <QVBoxLayout>
#include <QWidget>
#include <QMenuBar>
#include <QStatusBar>
#include <QDockWidget>
#include <QFormLayout>
#include <QFileDialog>
#include <QMessageBox>
#include "NodeItem.h" 

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);
    m_scene = new QGraphicsScene(this);
    QGraphicsView *view = new QGraphicsView(m_scene, this);
    view->setRenderHint(QPainter::Antialiasing);
    view->setDragMode(QGraphicsView::RubberBandDrag);
    layout->addWidget(view);
    setCentralWidget(centralWidget);

    createMenu();
    createPropertiesPanel();

    statusBar()->showMessage("Ready");
}

MainWindow::~MainWindow() {}

void MainWindow::createMenu(){
    QMenu *fileMenu = menuBar()->addMenu("File");

    QAction *openAct = new QAction("Open Image", this);
    QAction *saveAct = new QAction("Save Image", this);

    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);

    connect(openAct, &QAction::triggered, [this](){
        QString fileName = QFileDialog::getOpenFileName(this, "Open Image", "", "Images (*.png *.jpg *.bmp)");
        if(!fileName.isEmpty()){
            QMessageBox::information(this, "Open", "Image opened: " + fileName);
        }
    });
    connect(saveAct, &QAction::triggered, [this](){
        QString fileName = QFileDialog::getSaveFileName(this, "Save Image", "", "Images (*.png *.jpg *.bmp)");
        if(!fileName.isEmpty()){
            QMessageBox::information(this, "Save", "Result saved to: " + fileName);
        }
    });
}

void MainWindow::createPropertiesPanel(){
    m_propertiesDock = new QDockWidget("Properties", this);
    m_propertiesWidget = new QWidget(m_propertiesDock);

    QFormLayout *formLayout = new QFormLayout(m_propertiesWidget);
    
    m_nodeNameLabel = new QLabel("No node selected", m_propertiesWidget);
    formLayout->addRow("Node", m_nodeNameLabel);
    
    m_brightnessSlider = new QSlider(Qt::Horizontal, m_propertiesWidget);
    m_brightnessSlider->setRange(-100, 100);
    formLayout->addRow("Brightness", m_brightnessSlider);
    
    m_contrastSlider = new QSlider(Qt::Horizontal, m_propertiesWidget);

    m_contrastSlider->setRange(0, 300); 
    m_contrastSlider->setValue(100); 
    formLayout->addRow("Contrast", m_contrastSlider);
    
    m_resetButton = new QPushButton("Reset", m_propertiesWidget);
    formLayout->addRow(m_resetButton);

    m_propertiesWidget->setLayout(formLayout);
    m_propertiesDock->setWidget(m_propertiesWidget);
    addDockWidget(Qt::RightDockWidgetArea, m_propertiesDock);

    connect(m_scene, &QGraphicsScene::selectionChanged, this, &MainWindow::updatePropertiesPanel);

    connect(m_resetButton, &QPushButton::clicked, [this](){
        m_brightnessSlider->setValue(0);
        m_contrastSlider->setValue(100);
    });
}

void MainWindow::updatePropertiesPanel(){
    auto selectedItems = m_scene->selectedItems();
    if (!selectedItems.isEmpty()) {
        NodeItem *nodeItem = dynamic_cast<NodeItem*>(selectedItems.first());
        if (nodeItem) {
            m_nodeNameLabel->setText("Selected: " + nodeItem->getTitle());
        }
    } else {
        m_nodeNameLabel->setText("No node selected");
        m_brightnessSlider->setValue(0);
        m_contrastSlider->setValue(100);
    }
}
