#include "MainWindow.h"
#include "NodeItem.h"
#include "ImageInputNode.h"
#include "BrightnessContrastNode.h"
#include <QGraphicsView>
#include <QVBoxLayout>
#include <QMenuBar>
#include <QStatusBar>
#include <QDockWidget>
#include <QFormLayout>
#include <QLabel>
#include <QSlider>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>
#include <iostream>

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
    formLayout->addRow("Properties", m_nodeNameLabel);
    
 
    m_brightnessLabel = new QLabel("Brightness", m_propertiesWidget);
    m_brightnessSlider = new QSlider(Qt::Horizontal, m_propertiesWidget);
    m_brightnessSlider->setRange(-100, 100);
    formLayout->addRow(m_brightnessLabel, m_brightnessSlider);
    
 
    m_contrastLabel = new QLabel("Contrast", m_propertiesWidget);
    m_contrastSlider = new QSlider(Qt::Horizontal, m_propertiesWidget);
    m_contrastSlider->setRange(0, 300);  
    m_contrastSlider->setValue(100);     
    formLayout->addRow(m_contrastLabel, m_contrastSlider);
    
    // Reset button
    m_resetButton = new QPushButton("Reset", m_propertiesWidget);
    formLayout->addRow(m_resetButton);

    m_propertiesWidget->setLayout(formLayout);
    m_propertiesDock->setWidget(m_propertiesWidget);
    addDockWidget(Qt::RightDockWidgetArea, m_propertiesDock);

    connect(m_scene, &QGraphicsScene::selectionChanged, this, &MainWindow::updatePropertiesPanel);
    connect(m_brightnessSlider, &QSlider::valueChanged, this, &MainWindow::brightnessChanged);
    connect(m_contrastSlider, &QSlider::valueChanged, this, &MainWindow::contrastChanged);
    connect(m_resetButton, &QPushButton::clicked, this, &MainWindow::resetParameters);
}

void MainWindow::updatePropertiesPanel(){
    try {
        auto selectedItems = m_scene->selectedItems();
        if (!selectedItems.isEmpty()) {
            NodeItem *nodeItem = dynamic_cast<NodeItem*>(selectedItems.first());
            if (nodeItem && nodeItem->getNode()) {
                // If the selected node is an ImageInputNode, show metadata and hide controls.
                if (auto inputNode = dynamic_cast<ImageInputNode*>(nodeItem->getNode())) {
                    m_nodeNameLabel->setText(QString::fromStdString(inputNode->getMetadata()));
                    m_brightnessLabel->setVisible(false);
                    m_brightnessSlider->setVisible(false);
                    m_contrastLabel->setVisible(false);
                    m_contrastSlider->setVisible(false);
                    m_resetButton->setVisible(false);
                    return;
                }
                // If the selected node is a BrightnessContrastNode, show the sliders.
                else if (auto bcNode = dynamic_cast<BrightnessContrastNode*>(nodeItem->getNode())) {
                    m_nodeNameLabel->setText("Brightness/Contrast Node");
                    m_brightnessLabel->setVisible(true);
                    m_brightnessSlider->setVisible(true);
                    m_contrastLabel->setVisible(true);
                    m_contrastSlider->setVisible(true);
                    m_resetButton->setVisible(true);
                    m_brightnessSlider->blockSignals(true);
                    m_brightnessSlider->setValue(bcNode->getBrightness());
                    m_brightnessSlider->blockSignals(false);
                    m_contrastSlider->blockSignals(true);
                    m_contrastSlider->setValue(static_cast<int>(bcNode->getContrast()*100));
                    m_contrastSlider->blockSignals(false);
                    return;
                }
                // For other node types, show generic info and hide sliders.
                else {
                    m_nodeNameLabel->setText("Selected: " + nodeItem->getTitle());
                    m_brightnessLabel->setVisible(false);
                    m_brightnessSlider->setVisible(false);
                    m_contrastLabel->setVisible(false);
                    m_contrastSlider->setVisible(false);
                    m_resetButton->setVisible(false);
                    return;
                }
            } else {
                m_nodeNameLabel->setText("Selected node - no properties available");
            }
        } else {
            m_nodeNameLabel->setText("No node selected");
        }
        // Hide controls if nothing is selected.
        m_brightnessLabel->setVisible(false);
        m_brightnessSlider->setVisible(false);
        m_contrastLabel->setVisible(false);
        m_contrastSlider->setVisible(false);
        m_resetButton->setVisible(false);
    } catch (const std::exception &ex) {
        m_nodeNameLabel->setText(QString("Error: ") + ex.what());
    } catch (...) {
        m_nodeNameLabel->setText("Unknown error");
    }
}

void MainWindow::brightnessChanged(int value) {
    auto selectedItems = m_scene->selectedItems();
    if (!selectedItems.isEmpty()){
        NodeItem *nodeItem = dynamic_cast<NodeItem*>(selectedItems.first());
        if (nodeItem) {
            if (auto bcNode = dynamic_cast<BrightnessContrastNode*>(nodeItem->getNode())) {
                bcNode->setBrightness(value);
                bcNode->process();
                nodeItem->setPreview(bcNode->getOutput());
            }
        }
    }
}

void MainWindow::contrastChanged(int value) {
    auto selectedItems = m_scene->selectedItems();
    if (!selectedItems.isEmpty()){
        NodeItem *nodeItem = dynamic_cast<NodeItem*>(selectedItems.first());
        if (nodeItem) {
            if (auto bcNode = dynamic_cast<BrightnessContrastNode*>(nodeItem->getNode())) {
                double contrast = value / 100.0;
                bcNode->setContrast(contrast);
                bcNode->process();
                nodeItem->setPreview(bcNode->getOutput());
            }
        }
    }
}

void MainWindow::resetParameters() {
    m_brightnessSlider->setValue(0);
    m_contrastSlider->setValue(100);
}
