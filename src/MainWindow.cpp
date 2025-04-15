#include "MainWindow.h"
#include "NodeItem.h"
#include "ImageInputNode.h"
#include "BrightnessContrastNode.h"
#include "EdgeDetectionNode.h"
#include "OutputNode.h"
#include "ColorChannelSplitterNode.h"
#include "BlurNode.h"
#include "ThresholdNode.h"
#include "BlendNode.h"
#include "NoiseGenerationNode.h"
#include "ConvolutionFilterNode.h"
#include "NodeGraph.h"
#include "NodeEdge.h"
#include <QGraphicsView>
#include <QVBoxLayout>
#include <QMenuBar>
#include <QStatusBar>
#include <QDockWidget>
#include <QFormLayout>
#include <QLabel>
#include <QSlider>
#include <QPushButton>
#include <QComboBox>
#include <QCheckBox>
#include <QFileDialog>
#include <QMessageBox>
#include <iostream>
#include <memory>
#include <filesystem>
using namespace std;
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
    m_graph = new NodeGraph();
    createMenu();
    createPropertiesPanel();
    statusBar()->showMessage("Ready");
}
MainWindow::~MainWindow() { }
void MainWindow::createMenu(){
    QMenu *fileMenu = menuBar()->addMenu("File");
    QAction *openAct = new QAction("Open Image", this);
    QAction *saveAct = new QAction("Save Image", this);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    QMenu *addMenu = menuBar()->addMenu("Add Node");
    QAction *addInput = new QAction("Input Node", this);
    QAction *addBC = new QAction("Brightness/Contrast Node", this);
    QAction *addEdge = new QAction("Edge Detection Node", this);
    QAction *addOutput = new QAction("Output Node", this);
    QAction *addColor = new QAction("Color Channel Splitter Node", this);
    QAction *addBlur = new QAction("Blur Node", this);
    QAction *addThreshold = new QAction("Threshold Node", this);
    QAction *addBlend = new QAction("Blend Node", this);
    QAction *addNoise = new QAction("Noise Generation Node", this);
    QAction *addConv = new QAction("Convolution Filter Node", this);
    addMenu->addAction(addInput);
    addMenu->addAction(addBC);
    addMenu->addAction(addEdge);
    addMenu->addAction(addOutput);
    addMenu->addAction(addColor);
    addMenu->addAction(addBlur);
    addMenu->addAction(addThreshold);
    addMenu->addAction(addBlend);
    addMenu->addAction(addNoise);
    addMenu->addAction(addConv);
    connect(addInput, &QAction::triggered, this, &MainWindow::addInputNode);
    connect(addBC, &QAction::triggered, this, &MainWindow::addBrightnessContrastNode);
    connect(addEdge, &QAction::triggered, this, &MainWindow::addEdgeDetectionNode);
    connect(addOutput, &QAction::triggered, this, &MainWindow::addOutputNode);
    connect(addColor, &QAction::triggered, this, &MainWindow::addColorChannelSplitterNode);
    connect(addBlur, &QAction::triggered, this, &MainWindow::addBlurNode);
    connect(addThreshold, &QAction::triggered, this, &MainWindow::addThresholdNode);
    connect(addBlend, &QAction::triggered, this, &MainWindow::addBlendNode);
    connect(addNoise, &QAction::triggered, this, &MainWindow::addNoiseGenerationNode);
    connect(addConv, &QAction::triggered, this, &MainWindow::addConvolutionFilterNode);
    connect(openAct, &QAction::triggered, [this](){
        QString fileName = QFileDialog::getOpenFileName(this, "Open Image", "", "Images (*.png *.jpg *.bmp)");
        if(!fileName.isEmpty()){ QMessageBox::information(this, "Open", "Image opened: " + fileName); }
    });
    connect(saveAct, &QAction::triggered, [this](){
        QString fileName = QFileDialog::getSaveFileName(this, "Save Image", "", "Images (*.png *.jpg *.bmp)");
        if(!fileName.isEmpty()){ QMessageBox::information(this, "Save", "Result saved to: " + fileName); }
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
    m_edgeAlgoCombo = new QComboBox(m_propertiesWidget);
    m_edgeAlgoCombo->addItem("Canny");
    m_edgeAlgoCombo->addItem("Sobel");
    formLayout->addRow("Edge Alg", m_edgeAlgoCombo);
    m_lowerThresholdLabel = new QLabel("Lower Threshold", m_propertiesWidget);
    m_lowerThresholdSlider = new QSlider(Qt::Horizontal, m_propertiesWidget);
    m_lowerThresholdSlider->setRange(0, 255);
    m_lowerThresholdSlider->setValue(50);
    formLayout->addRow(m_lowerThresholdLabel, m_lowerThresholdSlider);
    m_upperThresholdLabel = new QLabel("Upper Threshold", m_propertiesWidget);
    m_upperThresholdSlider = new QSlider(Qt::Horizontal, m_propertiesWidget);
    m_upperThresholdSlider->setRange(0, 255);
    m_upperThresholdSlider->setValue(150);
    formLayout->addRow(m_upperThresholdLabel, m_upperThresholdSlider);
    m_kernelSizeLabel = new QLabel("Kernel Size", m_propertiesWidget);
    m_kernelSizeSlider = new QSlider(Qt::Horizontal, m_propertiesWidget);
    m_kernelSizeSlider->setRange(1, 20);
    m_kernelSizeSlider->setValue(3);
    formLayout->addRow(m_kernelSizeLabel, m_kernelSizeSlider);
    m_overlayCheckBox = new QCheckBox("Overlay", m_propertiesWidget);
    formLayout->addRow(m_overlayCheckBox);
    m_colorGrayscaleCheckBox = new QCheckBox("Grayscale", m_propertiesWidget);
    formLayout->addRow("Color Split", m_colorGrayscaleCheckBox);
    m_blurRadiusSlider = new QSlider(Qt::Horizontal, m_propertiesWidget);
    m_blurRadiusSlider->setRange(1,20);
    m_blurDirectionalCheckBox = new QCheckBox("Directional", m_propertiesWidget);
    formLayout->addRow("Blur Radius", m_blurRadiusSlider);
    formLayout->addRow("Directional", m_blurDirectionalCheckBox);
    m_thresholdSlider = new QSlider(Qt::Horizontal, m_propertiesWidget);
    m_thresholdSlider->setRange(0,255);
    m_thresholdSlider->setValue(128);
    m_thresholdMethodCombo = new QComboBox(m_propertiesWidget);
    m_thresholdMethodCombo->addItem("Binary");
    m_thresholdMethodCombo->addItem("Binary Inv");
    m_thresholdMethodCombo->addItem("Adaptive Mean");
    m_thresholdMethodCombo->addItem("Otsu");
    formLayout->addRow("Threshold", m_thresholdSlider);
    formLayout->addRow("Method", m_thresholdMethodCombo);
    m_blendOpacitySlider = new QSlider(Qt::Horizontal, m_propertiesWidget);
    m_blendOpacitySlider->setRange(0,100);
    m_blendOpacitySlider->setValue(100);
    m_blendModeCombo = new QComboBox(m_propertiesWidget);
    m_blendModeCombo->addItem("Normal");
    m_blendModeCombo->addItem("Multiply");
    m_blendModeCombo->addItem("Screen");
    m_blendModeCombo->addItem("Overlay");
    m_blendModeCombo->addItem("Difference");
    formLayout->addRow("Opacity", m_blendOpacitySlider);
    formLayout->addRow("Blend Mode", m_blendModeCombo);
    m_noiseScaleSlider = new QSlider(Qt::Horizontal, m_propertiesWidget);
    m_noiseScaleSlider->setRange(1,200);
    m_noiseScaleSlider->setValue(100);
    m_noiseOctavesSlider = new QSlider(Qt::Horizontal, m_propertiesWidget);
    m_noiseOctavesSlider->setRange(1,10);
    m_noiseOctavesSlider->setValue(1);
    m_noisePersistenceSlider = new QSlider(Qt::Horizontal, m_propertiesWidget);
    m_noisePersistenceSlider->setRange(0,100);
    m_noisePersistenceSlider->setValue(50);
    m_noiseTypeCombo = new QComboBox(m_propertiesWidget);
    m_noiseTypeCombo->addItem("Perlin");
    m_noiseTypeCombo->addItem("Simplex");
    m_noiseTypeCombo->addItem("Worley");
    m_noiseAsDisplacementCheckBox = new QCheckBox("Displacement", m_propertiesWidget);
    formLayout->addRow("Noise Scale", m_noiseScaleSlider);
    formLayout->addRow("Octaves", m_noiseOctavesSlider);
    formLayout->addRow("Persistence", m_noisePersistenceSlider);
    formLayout->addRow("Noise Type", m_noiseTypeCombo);
    formLayout->addRow("As Displacement", m_noiseAsDisplacementCheckBox);
    m_convPresetCombo = new QComboBox(m_propertiesWidget);
    m_convPresetCombo->addItem("Custom");
    m_convPresetCombo->addItem("Sharpen");
    m_convPresetCombo->addItem("Emboss");
    m_convPresetCombo->addItem("Edge Enhance");
    formLayout->addRow("Conv Preset", m_convPresetCombo);
    m_resetButton = new QPushButton("Reset", m_propertiesWidget);
    formLayout->addRow(m_resetButton);
    m_propertiesWidget->setLayout(formLayout);
    m_propertiesDock->setWidget(m_propertiesWidget);
    addDockWidget(Qt::RightDockWidgetArea, m_propertiesDock);
    connect(m_scene, &QGraphicsScene::selectionChanged, this, &MainWindow::updatePropertiesPanel);
    connect(m_brightnessSlider, &QSlider::valueChanged, this, &MainWindow::brightnessChanged);
    connect(m_contrastSlider, &QSlider::valueChanged, this, &MainWindow::contrastChanged);
    connect(m_resetButton, &QPushButton::clicked, this, &MainWindow::resetParameters);
    connect(m_edgeAlgoCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::edgeAlgorithmChanged);
    connect(m_lowerThresholdSlider, &QSlider::valueChanged, this, &MainWindow::lowerThresholdChanged);
    connect(m_upperThresholdSlider, &QSlider::valueChanged, this, &MainWindow::upperThresholdChanged);
    connect(m_kernelSizeSlider, &QSlider::valueChanged, this, &MainWindow::kernelSizeChanged);
    connect(m_overlayCheckBox, &QCheckBox::toggled, this, &MainWindow::overlayChanged);
    connect(m_colorGrayscaleCheckBox, &QCheckBox::toggled, this, &MainWindow::colorGrayscaleChanged);
    connect(m_blurRadiusSlider, &QSlider::valueChanged, this, &MainWindow::blurRadiusChanged);
    connect(m_blurDirectionalCheckBox, &QCheckBox::toggled, this, &MainWindow::blurDirectionalChanged);
    connect(m_thresholdSlider, &QSlider::valueChanged, this, &MainWindow::thresholdChanged);
    connect(m_thresholdMethodCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::thresholdMethodChanged);
    connect(m_blendOpacitySlider, &QSlider::valueChanged, this, &MainWindow::blendOpacityChanged);
    connect(m_blendModeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::blendModeChanged);
    connect(m_noiseScaleSlider, &QSlider::valueChanged, this, &MainWindow::noiseScaleChanged);
    connect(m_noiseOctavesSlider, &QSlider::valueChanged, this, &MainWindow::noiseOctavesChanged);
    connect(m_noisePersistenceSlider, &QSlider::valueChanged, this, &MainWindow::noisePersistenceChanged);
    connect(m_noiseTypeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::noiseTypeChanged);
    connect(m_noiseAsDisplacementCheckBox, &QCheckBox::toggled, this, &MainWindow::noiseAsDisplacementChanged);
    connect(m_convPresetCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::convPresetChanged);
}
void MainWindow::setBrightnessContrastNode(std::shared_ptr<BrightnessContrastNode> node){
    m_bcNode = node;
}
void MainWindow::updateAllNodePreviews(){
    m_graph->processAll();
    for(auto item : m_scene->items()){
        if(auto nodeItem = dynamic_cast<NodeItem*>(item)){
            if(nodeItem->getNode()){
                nodeItem->setPreview(nodeItem->getNode()->getOutput());
            }
        }
    }
    updateAllEdges();
}
void MainWindow::updateAllEdges(){
    QList<QGraphicsItem*> items = m_scene->items();
    for(auto item : items){
        if(auto edge = dynamic_cast<NodeEdge*>(item)){
            m_scene->removeItem(edge);
            delete edge;
        }
    }
    const auto &connections = m_graph->getConnections();
    for(const auto &conn : connections){
        NodeItem* sourceItem = nodeItemMap.value(conn.source->getId(), nullptr);
        NodeItem* targetItem = nodeItemMap.value(conn.target->getId(), nullptr);
        if(sourceItem && targetItem){
            NodeEdge* edge = new NodeEdge(sourceItem, targetItem);
            m_scene->addItem(edge);
        }
    }
}
void MainWindow::updatePropertiesPanel(){
    try{
        auto selectedItems = m_scene->selectedItems();
        if(!selectedItems.isEmpty()){
            NodeItem *nodeItem = dynamic_cast<NodeItem*>(selectedItems.first());
            if(nodeItem && nodeItem->getNode()){
                if(auto inputNode = dynamic_cast<ImageInputNode*>(nodeItem->getNode())){
                    m_nodeNameLabel->setText(QString::fromStdString(inputNode->getMetadata()));
                    m_brightnessLabel->setVisible(false);
                    m_brightnessSlider->setVisible(false);
                    m_contrastLabel->setVisible(false);
                    m_contrastSlider->setVisible(false);
                    m_edgeAlgoCombo->setVisible(false);
                    m_lowerThresholdLabel->setVisible(false);
                    m_lowerThresholdSlider->setVisible(false);
                    m_upperThresholdLabel->setVisible(false);
                    m_upperThresholdSlider->setVisible(false);
                    m_kernelSizeLabel->setVisible(false);
                    m_kernelSizeSlider->setVisible(false);
                    m_overlayCheckBox->setVisible(false);
                    m_colorGrayscaleCheckBox->setVisible(false);
                    m_blurRadiusSlider->setVisible(false);
                    m_blurDirectionalCheckBox->setVisible(false);
                    m_thresholdSlider->setVisible(false);
                    m_thresholdMethodCombo->setVisible(false);
                    m_blendOpacitySlider->setVisible(false);
                    m_blendModeCombo->setVisible(false);
                    m_noiseScaleSlider->setVisible(false);
                    m_noiseOctavesSlider->setVisible(false);
                    m_noisePersistenceSlider->setVisible(false);
                    m_noiseTypeCombo->setVisible(false);
                    m_noiseAsDisplacementCheckBox->setVisible(false);
                    m_convPresetCombo->setVisible(false);
                    m_resetButton->setVisible(false);
                    return;
                } else if(auto bcNode = dynamic_cast<BrightnessContrastNode*>(nodeItem->getNode())){
                    m_nodeNameLabel->setText("Brightness/Contrast Node");
                    m_brightnessLabel->setVisible(true);
                    m_brightnessSlider->setVisible(true);
                    m_contrastLabel->setVisible(true);
                    m_contrastSlider->setVisible(true);
                    m_edgeAlgoCombo->setVisible(false);
                    m_lowerThresholdLabel->setVisible(false);
                    m_lowerThresholdSlider->setVisible(false);
                    m_upperThresholdLabel->setVisible(false);
                    m_upperThresholdSlider->setVisible(false);
                    m_kernelSizeLabel->setVisible(false);
                    m_kernelSizeSlider->setVisible(false);
                    m_overlayCheckBox->setVisible(false);
                    m_colorGrayscaleCheckBox->setVisible(false);
                    m_blurRadiusSlider->setVisible(false);
                    m_blurDirectionalCheckBox->setVisible(false);
                    m_thresholdSlider->setVisible(false);
                    m_thresholdMethodCombo->setVisible(false);
                    m_blendOpacitySlider->setVisible(false);
                    m_blendModeCombo->setVisible(false);
                    m_noiseScaleSlider->setVisible(false);
                    m_noiseOctavesSlider->setVisible(false);
                    m_noisePersistenceSlider->setVisible(false);
                    m_noiseTypeCombo->setVisible(false);
                    m_noiseAsDisplacementCheckBox->setVisible(false);
                    m_convPresetCombo->setVisible(false);
                    m_resetButton->setVisible(true);
                    m_brightnessSlider->blockSignals(true);
                    m_brightnessSlider->setValue(bcNode->getBrightness());
                    m_brightnessSlider->blockSignals(false);
                    m_contrastSlider->blockSignals(true);
                    m_contrastSlider->setValue(static_cast<int>(bcNode->getContrast()*100));
                    m_contrastSlider->blockSignals(false);
                    return;
                } else if(auto edgeNode = dynamic_cast<EdgeDetectionNode*>(nodeItem->getNode())){
                    m_nodeNameLabel->setText("Edge Detection Node");
                    m_brightnessLabel->setVisible(false);
                    m_brightnessSlider->setVisible(false);
                    m_contrastLabel->setVisible(false);
                    m_contrastSlider->setVisible(false);
                    m_edgeAlgoCombo->setVisible(true);
                    m_lowerThresholdLabel->setVisible(true);
                    m_lowerThresholdSlider->setVisible(true);
                    m_upperThresholdLabel->setVisible(true);
                    m_upperThresholdSlider->setVisible(true);
                    m_kernelSizeLabel->setVisible(true);
                    m_kernelSizeSlider->setVisible(true);
                    m_overlayCheckBox->setVisible(true);
                    m_colorGrayscaleCheckBox->setVisible(false);
                    m_blurRadiusSlider->setVisible(false);
                    m_blurDirectionalCheckBox->setVisible(false);
                    m_thresholdSlider->setVisible(false);
                    m_thresholdMethodCombo->setVisible(false);
                    m_blendOpacitySlider->setVisible(false);
                    m_blendModeCombo->setVisible(false);
                    m_noiseScaleSlider->setVisible(false);
                    m_noiseOctavesSlider->setVisible(false);
                    m_noisePersistenceSlider->setVisible(false);
                    m_noiseTypeCombo->setVisible(false);
                    m_noiseAsDisplacementCheckBox->setVisible(false);
                    m_convPresetCombo->setVisible(false);
                    m_resetButton->setVisible(true);
                    if(edgeNode->getAlgorithm() == EdgeDetectionAlgorithm::CANNY){
                        m_edgeAlgoCombo->setCurrentIndex(0);
                        m_lowerThresholdSlider->blockSignals(true);
                        m_lowerThresholdSlider->setValue(static_cast<int>(edgeNode->getCannyLowerThreshold()));
                        m_lowerThresholdSlider->blockSignals(false);
                        m_upperThresholdSlider->blockSignals(true);
                        m_upperThresholdSlider->setValue(static_cast<int>(edgeNode->getCannyUpperThreshold()));
                        m_upperThresholdSlider->blockSignals(false);
                        m_kernelSizeLabel->setVisible(false);
                        m_kernelSizeSlider->setVisible(false);
                    } else{
                        m_edgeAlgoCombo->setCurrentIndex(1);
                        m_kernelSizeSlider->blockSignals(true);
                        m_kernelSizeSlider->setValue(edgeNode->getSobelKernelSize());
                        m_kernelSizeSlider->blockSignals(false);
                        m_lowerThresholdLabel->setVisible(false);
                        m_lowerThresholdSlider->setVisible(false);
                        m_upperThresholdLabel->setVisible(false);
                        m_upperThresholdSlider->setVisible(false);
                    }
                    m_overlayCheckBox->blockSignals(true);
                    m_overlayCheckBox->setChecked(edgeNode->getOverlay());
                    m_overlayCheckBox->blockSignals(false);
                    return;
                } else if(auto colorNode = dynamic_cast<ColorChannelSplitterNode*>(nodeItem->getNode())){
                    m_nodeNameLabel->setText("Color Channel Splitter Node");
                    m_colorGrayscaleCheckBox->setVisible(true);
                    m_brightnessLabel->setVisible(false);
                    m_brightnessSlider->setVisible(false);
                    m_contrastLabel->setVisible(false);
                    m_contrastSlider->setVisible(false);
                    m_edgeAlgoCombo->setVisible(false);
                    m_lowerThresholdLabel->setVisible(false);
                    m_lowerThresholdSlider->setVisible(false);
                    m_upperThresholdLabel->setVisible(false);
                    m_upperThresholdSlider->setVisible(false);
                    m_kernelSizeLabel->setVisible(false);
                    m_kernelSizeSlider->setVisible(false);
                    m_overlayCheckBox->setVisible(false);
                    m_resetButton->setVisible(true);
                    m_colorGrayscaleCheckBox->blockSignals(true);
                    m_colorGrayscaleCheckBox->setChecked(colorNode->getGrayscale());
                    m_colorGrayscaleCheckBox->blockSignals(false);
                    return;
                } else if(auto blurNode = dynamic_cast<BlurNode*>(nodeItem->getNode())){
                    m_nodeNameLabel->setText("Blur Node");
                    m_blurRadiusSlider->setVisible(true);
                    m_blurDirectionalCheckBox->setVisible(true);
                    m_brightnessLabel->setVisible(false);
                    m_brightnessSlider->setVisible(false);
                    m_contrastLabel->setVisible(false);
                    m_contrastSlider->setVisible(false);
                    m_edgeAlgoCombo->setVisible(false);
                    m_lowerThresholdLabel->setVisible(false);
                    m_lowerThresholdSlider->setVisible(false);
                    m_upperThresholdLabel->setVisible(false);
                    m_upperThresholdSlider->setVisible(false);
                    m_kernelSizeLabel->setVisible(false);
                    m_kernelSizeSlider->setVisible(false);
                    m_overlayCheckBox->setVisible(false);
                    m_colorGrayscaleCheckBox->setVisible(false);
                    m_thresholdSlider->setVisible(false);
                    m_thresholdMethodCombo->setVisible(false);
                    m_blendOpacitySlider->setVisible(false);
                    m_blendModeCombo->setVisible(false);
                    m_noiseScaleSlider->setVisible(false);
                    m_noiseOctavesSlider->setVisible(false);
                    m_noisePersistenceSlider->setVisible(false);
                    m_noiseTypeCombo->setVisible(false);
                    m_noiseAsDisplacementCheckBox->setVisible(false);
                    m_convPresetCombo->setVisible(false);
                    m_resetButton->setVisible(true);
                    m_blurRadiusSlider->blockSignals(true);
                    m_blurRadiusSlider->setValue(blurNode->getRadius());
                    m_blurRadiusSlider->blockSignals(false);
                    m_blurDirectionalCheckBox->blockSignals(true);
                    m_blurDirectionalCheckBox->setChecked(blurNode->getDirectional());
                    m_blurDirectionalCheckBox->blockSignals(false);
                    return;
                } else if(auto threshNode = dynamic_cast<ThresholdNode*>(nodeItem->getNode())){
                    m_nodeNameLabel->setText("Threshold Node");
                    m_thresholdSlider->setVisible(true);
                    m_thresholdMethodCombo->setVisible(true);
                    m_brightnessLabel->setVisible(false);
                    m_brightnessSlider->setVisible(false);
                    m_contrastLabel->setVisible(false);
                    m_contrastSlider->setVisible(false);
                    m_edgeAlgoCombo->setVisible(false);
                    m_lowerThresholdLabel->setVisible(false);
                    m_lowerThresholdSlider->setVisible(false);
                    m_upperThresholdLabel->setVisible(false);
                    m_upperThresholdSlider->setVisible(false);
                    m_kernelSizeLabel->setVisible(false);
                    m_kernelSizeSlider->setVisible(false);
                    m_overlayCheckBox->setVisible(false);
                    m_colorGrayscaleCheckBox->setVisible(false);
                    m_blurRadiusSlider->setVisible(false);
                    m_blurDirectionalCheckBox->setVisible(false);
                    m_blendOpacitySlider->setVisible(false);
                    m_blendModeCombo->setVisible(false);
                    m_noiseScaleSlider->setVisible(false);
                    m_noiseOctavesSlider->setVisible(false);
                    m_noisePersistenceSlider->setVisible(false);
                    m_noiseTypeCombo->setVisible(false);
                    m_noiseAsDisplacementCheckBox->setVisible(false);
                    m_convPresetCombo->setVisible(false);
                    m_resetButton->setVisible(true);
                    m_thresholdSlider->blockSignals(true);
                    m_thresholdSlider->setValue(threshNode->getThreshold());
                    m_thresholdSlider->blockSignals(false);
                    m_thresholdMethodCombo->blockSignals(true);
                    int methodIndex = 0;
                    switch(threshNode->getMethod()){
                        case ThresholdMethod::BINARY: methodIndex = 0; break;
                        case ThresholdMethod::BINARY_INV: methodIndex = 1; break;
                        case ThresholdMethod::ADAPTIVE_MEAN: methodIndex = 2; break;
                        case ThresholdMethod::OTSU: methodIndex = 3; break;
                    }
                    m_thresholdMethodCombo->setCurrentIndex(methodIndex);
                    m_thresholdMethodCombo->blockSignals(false);
                    return;
                } else if(auto blendNode = dynamic_cast<BlendNode*>(nodeItem->getNode())){
                    m_nodeNameLabel->setText("Blend Node");
                    m_blendOpacitySlider->setVisible(true);
                    m_blendModeCombo->setVisible(true);
                    m_brightnessLabel->setVisible(false);
                    m_brightnessSlider->setVisible(false);
                    m_contrastLabel->setVisible(false);
                    m_contrastSlider->setVisible(false);
                    m_edgeAlgoCombo->setVisible(false);
                    m_lowerThresholdLabel->setVisible(false);
                    m_lowerThresholdSlider->setVisible(false);
                    m_upperThresholdLabel->setVisible(false);
                    m_upperThresholdSlider->setVisible(false);
                    m_kernelSizeLabel->setVisible(false);
                    m_kernelSizeSlider->setVisible(false);
                    m_overlayCheckBox->setVisible(false);
                    m_colorGrayscaleCheckBox->setVisible(false);
                    m_blurRadiusSlider->setVisible(false);
                    m_blurDirectionalCheckBox->setVisible(false);
                    m_thresholdSlider->setVisible(false);
                    m_thresholdMethodCombo->setVisible(false);
                    m_noiseScaleSlider->setVisible(false);
                    m_noiseOctavesSlider->setVisible(false);
                    m_noisePersistenceSlider->setVisible(false);
                    m_noiseTypeCombo->setVisible(false);
                    m_noiseAsDisplacementCheckBox->setVisible(false);
                    m_convPresetCombo->setVisible(false);
                    m_resetButton->setVisible(true);
                    m_blendOpacitySlider->blockSignals(true);
                    m_blendOpacitySlider->setValue(blendNode->getOpacity()*100);
                    m_blendOpacitySlider->blockSignals(false);
                    int modeIndex = 0;
                    switch(blendNode->getBlendMode()){
                        case BlendMode::NORMAL: modeIndex = 0; break;
                        case BlendMode::MULTIPLY: modeIndex = 1; break;
                        case BlendMode::SCREEN: modeIndex = 2; break;
                        case BlendMode::OVERLAY: modeIndex = 3; break;
                        case BlendMode::DIFFERENCE: modeIndex = 4; break;
                    }
                    m_blendModeCombo->blockSignals(true);
                    m_blendModeCombo->setCurrentIndex(modeIndex);
                    m_blendModeCombo->blockSignals(false);
                    return;
                } else if(auto noiseNode = dynamic_cast<NoiseGenerationNode*>(nodeItem->getNode())){
                    m_nodeNameLabel->setText("Noise Generation Node");
                    m_noiseScaleSlider->setVisible(true);
                    m_noiseOctavesSlider->setVisible(true);
                    m_noisePersistenceSlider->setVisible(true);
                    m_noiseTypeCombo->setVisible(true);
                    m_noiseAsDisplacementCheckBox->setVisible(true);
                    m_brightnessLabel->setVisible(false);
                    m_brightnessSlider->setVisible(false);
                    m_contrastLabel->setVisible(false);
                    m_contrastSlider->setVisible(false);
                    m_edgeAlgoCombo->setVisible(false);
                    m_lowerThresholdLabel->setVisible(false);
                    m_lowerThresholdSlider->setVisible(false);
                    m_upperThresholdLabel->setVisible(false);
                    m_upperThresholdSlider->setVisible(false);
                    m_kernelSizeLabel->setVisible(false);
                    m_kernelSizeSlider->setVisible(false);
                    m_overlayCheckBox->setVisible(false);
                    m_colorGrayscaleCheckBox->setVisible(false);
                    m_blurRadiusSlider->setVisible(false);
                    m_blurDirectionalCheckBox->setVisible(false);
                    m_thresholdSlider->setVisible(false);
                    m_thresholdMethodCombo->setVisible(false);
                    m_blendOpacitySlider->setVisible(false);
                    m_blendModeCombo->setVisible(false);
                    m_convPresetCombo->setVisible(false);
                    m_resetButton->setVisible(true);
                    m_noiseScaleSlider->blockSignals(true);
                    m_noiseScaleSlider->setValue(static_cast<int>(noiseNode->getScale()*100));
                    m_noiseScaleSlider->blockSignals(false);
                    m_noiseOctavesSlider->blockSignals(true);
                    m_noiseOctavesSlider->setValue(noiseNode->getOctaves());
                    m_noiseOctavesSlider->blockSignals(false);
                    m_noisePersistenceSlider->blockSignals(true);
                    m_noisePersistenceSlider->setValue(static_cast<int>(noiseNode->getPersistence()*100));
                    m_noisePersistenceSlider->blockSignals(false);
                    int typeIndex = 0;
                    switch(noiseNode->getNoiseType()){
                        case NoiseType::PERLIN: typeIndex = 0; break;
                        case NoiseType::SIMPLEX: typeIndex = 1; break;
                        case NoiseType::WORLEY: typeIndex = 2; break;
                    }
                    m_noiseTypeCombo->blockSignals(true);
                    m_noiseTypeCombo->setCurrentIndex(typeIndex);
                    m_noiseTypeCombo->blockSignals(false);
                    m_noiseAsDisplacementCheckBox->blockSignals(true);
                    m_noiseAsDisplacementCheckBox->setChecked(noiseNode->getAsDisplacement());
                    m_noiseAsDisplacementCheckBox->blockSignals(false);
                    return;
                } else if(auto convNode = dynamic_cast<ConvolutionFilterNode*>(nodeItem->getNode())){
                    m_nodeNameLabel->setText("Convolution Filter Node");
                    m_convPresetCombo->setVisible(true);
                    m_brightnessLabel->setVisible(false);
                    m_brightnessSlider->setVisible(false);
                    m_contrastLabel->setVisible(false);
                    m_contrastSlider->setVisible(false);
                    m_edgeAlgoCombo->setVisible(false);
                    m_lowerThresholdLabel->setVisible(false);
                    m_lowerThresholdSlider->setVisible(false);
                    m_upperThresholdLabel->setVisible(false);
                    m_upperThresholdSlider->setVisible(false);
                    m_kernelSizeLabel->setVisible(false);
                    m_kernelSizeSlider->setVisible(false);
                    m_overlayCheckBox->setVisible(false);
                    m_colorGrayscaleCheckBox->setVisible(false);
                    m_blurRadiusSlider->setVisible(false);
                    m_blurDirectionalCheckBox->setVisible(false);
                    m_thresholdSlider->setVisible(false);
                    m_thresholdMethodCombo->setVisible(false);
                    m_blendOpacitySlider->setVisible(false);
                    m_blendModeCombo->setVisible(false);
                    m_noiseScaleSlider->setVisible(false);
                    m_noiseOctavesSlider->setVisible(false);
                    m_noisePersistenceSlider->setVisible(false);
                    m_noiseTypeCombo->setVisible(false);
                    m_noiseAsDisplacementCheckBox->setVisible(false);
                    m_resetButton->setVisible(true);
                    m_convPresetCombo->blockSignals(true);
                    int convIndex = 0;
                    switch(convNode->getPreset()){
                        case ConvolutionPreset::CUSTOM: convIndex = 0; break;
                        case ConvolutionPreset::SHARPEN: convIndex = 1; break;
                        case ConvolutionPreset::EMBOSS: convIndex = 2; break;
                        case ConvolutionPreset::EDGE_ENHANCE: convIndex = 3; break;
                    }
                    m_convPresetCombo->setCurrentIndex(convIndex);
                    m_convPresetCombo->blockSignals(false);
                    return;
                } else {
                    m_nodeNameLabel->setText("Selected: " + nodeItem->getTitle());
                    m_brightnessLabel->setVisible(false);
                    m_brightnessSlider->setVisible(false);
                    m_contrastLabel->setVisible(false);
                    m_contrastSlider->setVisible(false);
                    m_edgeAlgoCombo->setVisible(false);
                    m_lowerThresholdLabel->setVisible(false);
                    m_lowerThresholdSlider->setVisible(false);
                    m_upperThresholdLabel->setVisible(false);
                    m_upperThresholdSlider->setVisible(false);
                    m_kernelSizeLabel->setVisible(false);
                    m_kernelSizeSlider->setVisible(false);
                    m_overlayCheckBox->setVisible(false);
                    m_colorGrayscaleCheckBox->setVisible(false);
                    m_blurRadiusSlider->setVisible(false);
                    m_blurDirectionalCheckBox->setVisible(false);
                    m_thresholdSlider->setVisible(false);
                    m_thresholdMethodCombo->setVisible(false);
                    m_blendOpacitySlider->setVisible(false);
                    m_blendModeCombo->setVisible(false);
                    m_noiseScaleSlider->setVisible(false);
                    m_noiseOctavesSlider->setVisible(false);
                    m_noisePersistenceSlider->setVisible(false);
                    m_noiseTypeCombo->setVisible(false);
                    m_noiseAsDisplacementCheckBox->setVisible(false);
                    m_convPresetCombo->setVisible(false);
                    m_resetButton->setVisible(false);
                    return;
                }
            } else {
                m_nodeNameLabel->setText("Selected node - no properties available");
            }
        } else {
            m_nodeNameLabel->setText("No node selected");
        }
        m_brightnessLabel->setVisible(false);
        m_brightnessSlider->setVisible(false);
        m_contrastLabel->setVisible(false);
        m_contrastSlider->setVisible(false);
        m_edgeAlgoCombo->setVisible(false);
        m_lowerThresholdLabel->setVisible(false);
        m_lowerThresholdSlider->setVisible(false);
        m_upperThresholdLabel->setVisible(false);
        m_upperThresholdSlider->setVisible(false);
        m_kernelSizeLabel->setVisible(false);
        m_kernelSizeSlider->setVisible(false);
        m_overlayCheckBox->setVisible(false);
        m_colorGrayscaleCheckBox->setVisible(false);
        m_blurRadiusSlider->setVisible(false);
        m_blurDirectionalCheckBox->setVisible(false);
        m_thresholdSlider->setVisible(false);
        m_thresholdMethodCombo->setVisible(false);
        m_blendOpacitySlider->setVisible(false);
        m_blendModeCombo->setVisible(false);
        m_noiseScaleSlider->setVisible(false);
        m_noiseOctavesSlider->setVisible(false);
        m_noisePersistenceSlider->setVisible(false);
        m_noiseTypeCombo->setVisible(false);
        m_noiseAsDisplacementCheckBox->setVisible(false);
        m_convPresetCombo->setVisible(false);
        m_resetButton->setVisible(false);
    } catch(const std::exception &ex){
        m_nodeNameLabel->setText(QString("Error: ") + ex.what());
    } catch(...){
        m_nodeNameLabel->setText("Unknown error");
    }
}
void MainWindow::brightnessChanged(int value){
    auto selectedItems = m_scene->selectedItems();
    if(!selectedItems.isEmpty()){
        if(auto nodeItem = dynamic_cast<NodeItem*>(selectedItems.first())){
            if(auto bcNode = dynamic_cast<BrightnessContrastNode*>(nodeItem->getNode())){
                bcNode->setBrightness(value);
                bcNode->process();
                updateAllNodePreviews();
            }
        }
    }
}

void MainWindow::resetParameters(){
    m_brightnessSlider->setValue(0);
    m_contrastSlider->setValue(100);
    m_lowerThresholdSlider->setValue(50);
    m_upperThresholdSlider->setValue(150);
    m_kernelSizeSlider->setValue(3);
    m_edgeAlgoCombo->setCurrentIndex(0);
    m_overlayCheckBox->setChecked(false);
    m_colorGrayscaleCheckBox->setChecked(false);
    m_blurRadiusSlider->setValue(5);
    m_blurDirectionalCheckBox->setChecked(false);
    m_thresholdSlider->setValue(128);
    m_thresholdMethodCombo->setCurrentIndex(0);
    m_blendOpacitySlider->setValue(100);
    m_blendModeCombo->setCurrentIndex(0);
    m_noiseScaleSlider->setValue(100);
    m_noiseOctavesSlider->setValue(1);
    m_noisePersistenceSlider->setValue(50);
    m_noiseTypeCombo->setCurrentIndex(0);
    m_noiseAsDisplacementCheckBox->setChecked(false);
    m_convPresetCombo->setCurrentIndex(0);
}


void MainWindow::contrastChanged(int value){
    auto selectedItems = m_scene->selectedItems();
    if(!selectedItems.isEmpty()){
        if(auto nodeItem = dynamic_cast<NodeItem*>(selectedItems.first())){
            if(auto bcNode = dynamic_cast<BrightnessContrastNode*>(nodeItem->getNode())){
                double contrast = value/100.0;
                bcNode->setContrast(contrast);
                bcNode->process();
                updateAllNodePreviews();
            }
        }
    }
}
void MainWindow::edgeAlgorithmChanged(int index){
    auto selectedItems = m_scene->selectedItems();
    if(!selectedItems.isEmpty()){
        if(auto nodeItem = dynamic_cast<NodeItem*>(selectedItems.first())){
            if(auto edgeNode = dynamic_cast<EdgeDetectionNode*>(nodeItem->getNode())){
                if(index==0) edgeNode->setAlgorithm(EdgeDetectionAlgorithm::CANNY);
                else edgeNode->setAlgorithm(EdgeDetectionAlgorithm::SOBEL);
                edgeNode->process();
                updateAllNodePreviews();
            }
        }
    }
}
void MainWindow::lowerThresholdChanged(int value){
    auto selectedItems = m_scene->selectedItems();
    if(!selectedItems.isEmpty()){
        if(auto nodeItem = dynamic_cast<NodeItem*>(selectedItems.first())){
            if(auto edgeNode = dynamic_cast<EdgeDetectionNode*>(nodeItem->getNode())){
                edgeNode->setCannyParameters(value, edgeNode->getCannyUpperThreshold(), edgeNode->getCannyAperture());
                edgeNode->process();
                updateAllNodePreviews();
            } else if(auto threshNode = dynamic_cast<ThresholdNode*>(nodeItem->getNode())){
                threshNode->setThreshold(value);
                threshNode->process();
                updateAllNodePreviews();
            }
        }
    }
}
void MainWindow::upperThresholdChanged(int value){
    auto selectedItems = m_scene->selectedItems();
    if(!selectedItems.isEmpty()){
        if(auto nodeItem = dynamic_cast<NodeItem*>(selectedItems.first())){
            if(auto edgeNode = dynamic_cast<EdgeDetectionNode*>(nodeItem->getNode())){
                edgeNode->setCannyParameters(edgeNode->getCannyLowerThreshold(), value, edgeNode->getCannyAperture());
                edgeNode->process();
                updateAllNodePreviews();
            }
        }
    }
}
void MainWindow::kernelSizeChanged(int value){
    auto selectedItems = m_scene->selectedItems();
    if(!selectedItems.isEmpty()){
        if(auto nodeItem = dynamic_cast<NodeItem*>(selectedItems.first())){
            if(auto edgeNode = dynamic_cast<EdgeDetectionNode*>(nodeItem->getNode())){
                edgeNode->setSobelParameters(value);
                edgeNode->process();
                updateAllNodePreviews();
            }
        }
    }
}
void MainWindow::overlayChanged(bool checked){
    auto selectedItems = m_scene->selectedItems();
    if(!selectedItems.isEmpty()){
        if(auto nodeItem = dynamic_cast<NodeItem*>(selectedItems.first())){
            if(auto edgeNode = dynamic_cast<EdgeDetectionNode*>(nodeItem->getNode())){
                edgeNode->setOverlay(checked);
                edgeNode->process();
                updateAllNodePreviews();
            }
        }
    }
}
void MainWindow::colorGrayscaleChanged(bool checked){
    auto selectedItems = m_scene->selectedItems();
    if(!selectedItems.isEmpty()){
        if(auto nodeItem = dynamic_cast<NodeItem*>(selectedItems.first())){
            if(auto colorNode = dynamic_cast<ColorChannelSplitterNode*>(nodeItem->getNode())){
                colorNode->setGrayscale(checked);
                colorNode->process();
                updateAllNodePreviews();
            }
        }
    }
}
void MainWindow::blurRadiusChanged(int value){
    auto selectedItems = m_scene->selectedItems();
    if(!selectedItems.isEmpty()){
        if(auto nodeItem = dynamic_cast<NodeItem*>(selectedItems.first())){
            if(auto blurNode = dynamic_cast<BlurNode*>(nodeItem->getNode())){
                blurNode->setRadius(value);
                blurNode->process();
                updateAllNodePreviews();
            }
        }
    }
}
void MainWindow::blurDirectionalChanged(bool checked){
    auto selectedItems = m_scene->selectedItems();
    if(!selectedItems.isEmpty()){
        if(auto nodeItem = dynamic_cast<NodeItem*>(selectedItems.first())){
            if(auto blurNode = dynamic_cast<BlurNode*>(nodeItem->getNode())){
                blurNode->setDirectional(checked);
                blurNode->process();
                updateAllNodePreviews();
            }
        }
    }
}
void MainWindow::thresholdChanged(int value){
    auto selectedItems = m_scene->selectedItems();
    if(!selectedItems.isEmpty()){
        if(auto nodeItem = dynamic_cast<NodeItem*>(selectedItems.first())){
            if(auto threshNode = dynamic_cast<ThresholdNode*>(nodeItem->getNode())){
                threshNode->setThreshold(value);
                threshNode->process();
                updateAllNodePreviews();
            }
        }
    }
}
void MainWindow::thresholdMethodChanged(int index){
    auto selectedItems = m_scene->selectedItems();
    if(!selectedItems.isEmpty()){
        if(auto nodeItem = dynamic_cast<NodeItem*>(selectedItems.first())){
            if(auto threshNode = dynamic_cast<ThresholdNode*>(nodeItem->getNode())){
                ThresholdMethod method = ThresholdMethod::BINARY;
                switch(index){
                    case 0: method = ThresholdMethod::BINARY; break;
                    case 1: method = ThresholdMethod::BINARY_INV; break;
                    case 2: method = ThresholdMethod::ADAPTIVE_MEAN; break;
                    case 3: method = ThresholdMethod::OTSU; break;
                }
                threshNode->setMethod(method);
                threshNode->process();
                updateAllNodePreviews();
            }
        }
    }
}
void MainWindow::blendOpacityChanged(int value){
    auto selectedItems = m_scene->selectedItems();
    if(!selectedItems.isEmpty()){
        if(auto nodeItem = dynamic_cast<NodeItem*>(selectedItems.first())){
            if(auto blendNode = dynamic_cast<BlendNode*>(nodeItem->getNode())){
                blendNode->setOpacity(value/100.0);
                blendNode->process();
                updateAllNodePreviews();
            }
        }
    }
}
void MainWindow::blendModeChanged(int index){
    auto selectedItems = m_scene->selectedItems();
    if(!selectedItems.isEmpty()){
        if(auto nodeItem = dynamic_cast<NodeItem*>(selectedItems.first())){
            if(auto blendNode = dynamic_cast<BlendNode*>(nodeItem->getNode())){
                BlendMode mode = BlendMode::NORMAL;
                switch(index){
                    case 0: mode = BlendMode::NORMAL; break;
                    case 1: mode = BlendMode::MULTIPLY; break;
                    case 2: mode = BlendMode::SCREEN; break;
                    case 3: mode = BlendMode::OVERLAY; break;
                    case 4: mode = BlendMode::DIFFERENCE; break;
                }
                blendNode->setBlendMode(mode);
                blendNode->process();
                updateAllNodePreviews();
            }
        }
    }
}
void MainWindow::noiseScaleChanged(int value){
    auto selectedItems = m_scene->selectedItems();
    if(!selectedItems.isEmpty()){
        if(auto nodeItem = dynamic_cast<NodeItem*>(selectedItems.first())){
            if(auto noiseNode = dynamic_cast<NoiseGenerationNode*>(nodeItem->getNode())){
                noiseNode->setScale(value/100.0);
                noiseNode->process();
                updateAllNodePreviews();
            }
        }
    }
}
void MainWindow::noiseOctavesChanged(int value){
    auto selectedItems = m_scene->selectedItems();
    if(!selectedItems.isEmpty()){
        if(auto nodeItem = dynamic_cast<NodeItem*>(selectedItems.first())){
            if(auto noiseNode = dynamic_cast<NoiseGenerationNode*>(nodeItem->getNode())){
                noiseNode->setOctaves(value);
                noiseNode->process();
                updateAllNodePreviews();
            }
        }
    }
}
void MainWindow::noisePersistenceChanged(int value){
    auto selectedItems = m_scene->selectedItems();
    if(!selectedItems.isEmpty()){
        if(auto nodeItem = dynamic_cast<NodeItem*>(selectedItems.first())){
            if(auto noiseNode = dynamic_cast<NoiseGenerationNode*>(nodeItem->getNode())){
                noiseNode->setPersistence(value/100.0);
                noiseNode->process();
                updateAllNodePreviews();
            }
        }
    }
}
void MainWindow::noiseTypeChanged(int index){
    auto selectedItems = m_scene->selectedItems();
    if(!selectedItems.isEmpty()){
        if(auto nodeItem = dynamic_cast<NodeItem*>(selectedItems.first())){
            if(auto noiseNode = dynamic_cast<NoiseGenerationNode*>(nodeItem->getNode())){
                NoiseType type = NoiseType::PERLIN;
                switch(index){
                    case 0: type = NoiseType::PERLIN; break;
                    case 1: type = NoiseType::SIMPLEX; break;
                    case 2: type = NoiseType::WORLEY; break;
                }
                noiseNode->setNoiseType(type);
                noiseNode->process();
                updateAllNodePreviews();
            }
        }
    }
}
void MainWindow::noiseAsDisplacementChanged(bool checked){
    auto selectedItems = m_scene->selectedItems();
    if(!selectedItems.isEmpty()){
        if(auto nodeItem = dynamic_cast<NodeItem*>(selectedItems.first())){
            if(auto noiseNode = dynamic_cast<NoiseGenerationNode*>(nodeItem->getNode())){
                noiseNode->setAsDisplacement(checked);
                noiseNode->process();
                updateAllNodePreviews();
            }
        }
    }
}
void MainWindow::convPresetChanged(int index){
    auto selectedItems = m_scene->selectedItems();
    if(!selectedItems.isEmpty()){
        if(auto nodeItem = dynamic_cast<NodeItem*>(selectedItems.first())){
            if(auto convNode = dynamic_cast<ConvolutionFilterNode*>(nodeItem->getNode())){
                ConvolutionPreset preset = ConvolutionPreset::CUSTOM;
                switch(index){
                    case 0: preset = ConvolutionPreset::CUSTOM; break;
                    case 1: preset = ConvolutionPreset::SHARPEN; break;
                    case 2: preset = ConvolutionPreset::EMBOSS; break;
                    case 3: preset = ConvolutionPreset::EDGE_ENHANCE; break;
                }
                convNode->setPreset(preset);
                convNode->process();
                updateAllNodePreviews();
            }
        }
    }
}
void MainWindow::addInputNode(){
    QString fileName = QFileDialog::getOpenFileName(this, "Select Image", "", "Images (*.png *.jpg *.bmp)");
    if(fileName.isEmpty()) return;
    auto inputNode = std::make_shared<ImageInputNode>("Input Node " + std::to_string(nodeItemMap.size()+1));
    inputNode->setImagePath(fileName.toStdString());
    inputNode->process();
    m_graph->addNode(inputNode);
    nodeSharedMap.insert(inputNode->getId(),inputNode);
    NodeItem *item = new NodeItem(QString::fromStdString(inputNode->getId()));
    item->setNode(inputNode.get());
    item->setPreview(inputNode->getOutput());
    item->setPos(50, 50 + 120 * nodeItemMap.size());
    m_scene->addItem(item);
    nodeItemMap.insert(inputNode->getId(), item);
    connect(item, &NodeItem::connectionCreated, this, [this](const QString &sourceId, const QString &targetId) {
        std::string src = sourceId.toStdString();
        std::string tgt = targetId.toStdString();
        auto sourceShared = nodeSharedMap.value(src);
        auto targetShared = nodeSharedMap.value(tgt);
        if(sourceShared && targetShared) {
            m_graph->connect(sourceShared, targetShared);
        }
        updateAllEdges();
    });
    updateAllNodePreviews();
}
void MainWindow::addBrightnessContrastNode(){
    auto bcNode = std::make_shared<BrightnessContrastNode>("Brightness/Contrast Node " + std::to_string(nodeItemMap.size()+1));
    bcNode->setBrightness(30);
    bcNode->setContrast(1.1);
    bcNode->process();
    m_graph->addNode(bcNode);
    nodeSharedMap.insert(bcNode->getId(),bcNode);
    NodeItem *item = new NodeItem(QString::fromStdString(bcNode->getId()));
    item->setNode(bcNode.get());
    item->setPreview(bcNode->getOutput());
    item->setPos(250, 50 + 120 * nodeItemMap.size());
    m_scene->addItem(item);
    nodeItemMap.insert(bcNode->getId(), item);
    connect(item, &NodeItem::connectionCreated, this, [this](const QString &sourceId, const QString &targetId) {
        std::string src = sourceId.toStdString();
        std::string tgt = targetId.toStdString();
        auto sourceShared = nodeSharedMap.value(src);
        auto targetShared = nodeSharedMap.value(tgt);
        if(sourceShared && targetShared) {
            m_graph->connect(sourceShared, targetShared);
        }
        updateAllEdges();
    });
    updateAllNodePreviews();
}
void MainWindow::addEdgeDetectionNode(){
    auto edgeNode = std::make_shared<EdgeDetectionNode>("Edge Detection Node " + std::to_string(nodeItemMap.size()+1));
    edgeNode->setAlgorithm(EdgeDetectionAlgorithm::CANNY);
    edgeNode->setCannyParameters(50, 150, 3);
    edgeNode->process();
    m_graph->addNode(edgeNode);
    nodeSharedMap.insert(edgeNode->getId(),edgeNode);
    NodeItem *item = new NodeItem(QString::fromStdString(edgeNode->getId()));
    item->setNode(edgeNode.get());
    item->setPreview(edgeNode->getOutput());
    item->setPos(450, 50 + 120 * nodeItemMap.size());
    m_scene->addItem(item);
    nodeItemMap.insert(edgeNode->getId(), item);
    connect(item, &NodeItem::connectionCreated, this, [this](const QString &sourceId, const QString &targetId) {
        std::string src = sourceId.toStdString();
        std::string tgt = targetId.toStdString();
        auto sourceShared = nodeSharedMap.value(src);
        auto targetShared = nodeSharedMap.value(tgt);
        if(sourceShared && targetShared) {
            m_graph->connect(sourceShared, targetShared);
        }
        updateAllEdges();
    });
    updateAllNodePreviews();
}
void MainWindow::addOutputNode(){
    auto outputNode = std::make_shared<OutputNode>("Output Node " + std::to_string(nodeItemMap.size()+1));
    outputNode->setOutputPath("output/final_results.jpg");
    outputNode->process();
    m_graph->addNode(outputNode);
    nodeSharedMap.insert(outputNode->getId(),outputNode);
    NodeItem *item = new NodeItem(QString::fromStdString(outputNode->getId()));
    item->setNode(outputNode.get());
    item->setPreview(outputNode->getOutput());
    item->setPos(650, 50 + 120 * nodeItemMap.size());
    m_scene->addItem(item);
    nodeItemMap.insert(outputNode->getId(), item);
    connect(item, &NodeItem::connectionCreated, this, [this](const QString &sourceId, const QString &targetId) {
        std::string src = sourceId.toStdString();
        std::string tgt = targetId.toStdString();
        auto sourceShared = nodeSharedMap.value(src);
        auto targetShared = nodeSharedMap.value(tgt);
        if(sourceShared && targetShared) {
            m_graph->connect(sourceShared, targetShared);
        }
        updateAllEdges();
    });
    updateAllNodePreviews();
}
void MainWindow::addColorChannelSplitterNode(){
    auto colorNode = std::make_shared<ColorChannelSplitterNode>("Color Splitter Node " + std::to_string(nodeItemMap.size()+1));
    colorNode->process();
    m_graph->addNode(colorNode);
    nodeSharedMap.insert(colorNode->getId(),colorNode);
    NodeItem *item = new NodeItem(QString::fromStdString(colorNode->getId()));
    item->setNode(colorNode.get());
    item->setPreview(colorNode->getOutput());
    item->setPos(50, 50 + 120 * nodeItemMap.size());
    m_scene->addItem(item);
    nodeItemMap.insert(colorNode->getId(), item);
    connect(item, &NodeItem::connectionCreated, this, [this](const QString &sourceId, const QString &targetId) {
        std::string src = sourceId.toStdString();
        std::string tgt = targetId.toStdString();
        auto sourceShared = nodeSharedMap.value(src);
        auto targetShared = nodeSharedMap.value(tgt);
        if(sourceShared && targetShared) {
            m_graph->connect(sourceShared, targetShared);
        }
        updateAllEdges();
    });
    updateAllNodePreviews();
}
void MainWindow::addBlurNode(){
    auto blurNode = std::make_shared<BlurNode>("Blur Node " + std::to_string(nodeItemMap.size()+1));
    blurNode->setRadius(5);
    blurNode->setDirectional(false);
    blurNode->process();
    m_graph->addNode(blurNode);
    nodeSharedMap.insert(blurNode->getId(), blurNode);
    NodeItem *item = new NodeItem(QString::fromStdString(blurNode->getId()));
    item->setNode(blurNode.get());
    item->setPreview(blurNode->getOutput());
    item->setPos(250, 50 + 120 * nodeItemMap.size());
    m_scene->addItem(item);
    nodeItemMap.insert(blurNode->getId(), item);
    connect(item, &NodeItem::connectionCreated, this, [this](const QString &sourceId, const QString &targetId) {
        std::string src = sourceId.toStdString();
        std::string tgt = targetId.toStdString();
        auto sourceShared = nodeSharedMap.value(src);
        auto targetShared = nodeSharedMap.value(tgt);
        if(sourceShared && targetShared) { m_graph->connect(sourceShared, targetShared); }
        updateAllEdges();
    });
    updateAllNodePreviews();
}
void MainWindow::addThresholdNode(){
    auto threshNode = std::make_shared<ThresholdNode>("Threshold Node " + std::to_string(nodeItemMap.size()+1));
    threshNode->setThreshold(128);
    threshNode->setMaxValue(255);
    threshNode->setMethod(ThresholdMethod::BINARY);
    threshNode->process();
    m_graph->addNode(threshNode);
    nodeSharedMap.insert(threshNode->getId(), threshNode);
    NodeItem *item = new NodeItem(QString::fromStdString(threshNode->getId()));
    item->setNode(threshNode.get());
    item->setPreview(threshNode->getOutput());
    item->setPos(450, 50 + 120 * nodeItemMap.size());
    m_scene->addItem(item);
    nodeItemMap.insert(threshNode->getId(), item);
    connect(item, &NodeItem::connectionCreated, this, [this](const QString &sourceId, const QString &targetId) {
        std::string src = sourceId.toStdString();
        std::string tgt = targetId.toStdString();
        auto sourceShared = nodeSharedMap.value(src);
        auto targetShared = nodeSharedMap.value(tgt);
        if(sourceShared && targetShared) { m_graph->connect(sourceShared, targetShared); }
        updateAllEdges();
    });
    updateAllNodePreviews();
}
void MainWindow::addBlendNode(){
    auto blendNode = std::make_shared<BlendNode>("Blend Node " + std::to_string(nodeItemMap.size()+1));
    blendNode->setOpacity(1.0);
    blendNode->setBlendMode(BlendMode::NORMAL);
    blendNode->process();
    m_graph->addNode(blendNode);
    nodeSharedMap.insert(blendNode->getId(), blendNode);
    NodeItem *item = new NodeItem(QString::fromStdString(blendNode->getId()));
    item->setNode(blendNode.get());
    item->setPreview(blendNode->getOutput());
    item->setPos(650, 50 + 120 * nodeItemMap.size());
    m_scene->addItem(item);
    nodeItemMap.insert(blendNode->getId(), item);
    connect(item, &NodeItem::connectionCreated, this, [this](const QString &sourceId, const QString &targetId) {
        std::string src = sourceId.toStdString();
        std::string tgt = targetId.toStdString();
        auto sourceShared = nodeSharedMap.value(src);
        auto targetShared = nodeSharedMap.value(tgt);
        if(sourceShared && targetShared) { m_graph->connect(sourceShared, targetShared); }
        updateAllEdges();
    });
    updateAllNodePreviews();
}
void MainWindow::addNoiseGenerationNode(){
    auto noiseNode = std::make_shared<NoiseGenerationNode>("Noise Generation Node " + std::to_string(nodeItemMap.size()+1));
    noiseNode->setScale(1.0);
    noiseNode->setOctaves(1);
    noiseNode->setPersistence(0.5);
    noiseNode->setNoiseType(NoiseType::PERLIN);
    noiseNode->setAsDisplacement(false);
    noiseNode->process();
    m_graph->addNode(noiseNode);
    nodeSharedMap.insert(noiseNode->getId(), noiseNode);
    NodeItem *item = new NodeItem(QString::fromStdString(noiseNode->getId()));
    item->setNode(noiseNode.get());
    item->setPreview(noiseNode->getOutput());
    item->setPos(50, 50 + 120 * nodeItemMap.size());
    m_scene->addItem(item);
    nodeItemMap.insert(noiseNode->getId(), item);
    connect(item, &NodeItem::connectionCreated, this, [this](const QString &sourceId, const QString &targetId) {
        std::string src = sourceId.toStdString();
        std::string tgt = targetId.toStdString();
        auto sourceShared = nodeSharedMap.value(src);
        auto targetShared = nodeSharedMap.value(tgt);
        if(sourceShared && targetShared) { m_graph->connect(sourceShared, targetShared); }
        updateAllEdges();
    });
    updateAllNodePreviews();
}
void MainWindow::addConvolutionFilterNode(){
    auto convNode = std::make_shared<ConvolutionFilterNode>("Convolution Filter Node " + std::to_string(nodeItemMap.size()+1));
    convNode->setPreset(ConvolutionPreset::CUSTOM);
    convNode->process();
    m_graph->addNode(convNode);
    nodeSharedMap.insert(convNode->getId(), convNode);
    NodeItem *item = new NodeItem(QString::fromStdString(convNode->getId()));
    item->setNode(convNode.get());
    item->setPreview(convNode->getOutput());
    item->setPos(250, 50 + 120 * nodeItemMap.size());
    m_scene->addItem(item);
    nodeItemMap.insert(convNode->getId(), item);
    connect(item, &NodeItem::connectionCreated, this, [this](const QString &sourceId, const QString &targetId) {
        std::string src = sourceId.toStdString();
        std::string tgt = targetId.toStdString();
        auto sourceShared = nodeSharedMap.value(src);
        auto targetShared = nodeSharedMap.value(tgt);
        if(sourceShared && targetShared) { m_graph->connect(sourceShared, targetShared); }
        updateAllEdges();
    });
    updateAllNodePreviews();
}