#include "MainWindow.h"
#include "NodeItem.h"
#include "ImageInputNode.h"
#include "BrightnessContrastNode.h"
#include "EdgeDetectionNode.h"
#include "OutputNode.h"
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
    addMenu->addAction(addInput);
    addMenu->addAction(addBC);
    addMenu->addAction(addEdge);
    addMenu->addAction(addOutput);
    connect(addInput, &QAction::triggered, this, &MainWindow::addInputNode);
    connect(addBC, &QAction::triggered, this, &MainWindow::addBrightnessContrastNode);
    connect(addEdge, &QAction::triggered, this, &MainWindow::addEdgeDetectionNode);
    connect(addOutput, &QAction::triggered, this, &MainWindow::addOutputNode);
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
    formLayout->addRow("Algorithm", m_edgeAlgoCombo);
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
    m_overlayCheckBox = new QCheckBox("Overlay Edges", m_propertiesWidget);
    formLayout->addRow(m_overlayCheckBox);
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
                } else if(auto outputNode = dynamic_cast<OutputNode*>(nodeItem->getNode())){
                    m_nodeNameLabel->setText("Output Node");
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
                    m_resetButton->setVisible(false);
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
                    m_resetButton->setVisible(false);
                    return;
                }
            } else { m_nodeNameLabel->setText("Selected node - no properties available"); }
        } else { m_nodeNameLabel->setText("No node selected"); }
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
void MainWindow::edgeAlgorithmChanged(int index) {
    auto selectedItems = m_scene->selectedItems();
    if (!selectedItems.isEmpty()) {
        if (auto nodeItem = dynamic_cast<NodeItem*>(selectedItems.first())) {
            if (auto edgeNode = dynamic_cast<EdgeDetectionNode*>(nodeItem->getNode())) {
                if (index == 0) {
                    edgeNode->setAlgorithm(EdgeDetectionAlgorithm::CANNY);
                } else {
                    edgeNode->setAlgorithm(EdgeDetectionAlgorithm::SOBEL);
                }
                edgeNode->process();
                updateAllNodePreviews();
            }
        }
    }
}
void MainWindow::lowerThresholdChanged(int value) {
    auto selectedItems = m_scene->selectedItems();
    if (!selectedItems.isEmpty()) {
        if (auto nodeItem = dynamic_cast<NodeItem*>(selectedItems.first())) {
            if (auto edgeNode = dynamic_cast<EdgeDetectionNode*>(nodeItem->getNode())) {
                // For Canny, update lower threshold
                if (edgeNode->getAlgorithm() == EdgeDetectionAlgorithm::CANNY) {
                    edgeNode->setCannyParameters(value, edgeNode->getCannyUpperThreshold(), edgeNode->getCannyAperture());
                }
                edgeNode->process();
                updateAllNodePreviews();
            }
        }
    }
}
void MainWindow::upperThresholdChanged(int value) {
    auto selectedItems = m_scene->selectedItems();
    if (!selectedItems.isEmpty()) {
        if (auto nodeItem = dynamic_cast<NodeItem*>(selectedItems.first())) {
            if (auto edgeNode = dynamic_cast<EdgeDetectionNode*>(nodeItem->getNode())) {
                // For Canny, update upper threshold
                if (edgeNode->getAlgorithm() == EdgeDetectionAlgorithm::CANNY) {
                    edgeNode->setCannyParameters(edgeNode->getCannyLowerThreshold(), value, edgeNode->getCannyAperture());
                }
                edgeNode->process();
                updateAllNodePreviews();
            }
        }
    }
}
void MainWindow::kernelSizeChanged(int value) {
    auto selectedItems = m_scene->selectedItems();
    if (!selectedItems.isEmpty()) {
        if (auto nodeItem = dynamic_cast<NodeItem*>(selectedItems.first())) {
            if (auto edgeNode = dynamic_cast<EdgeDetectionNode*>(nodeItem->getNode())) {
                // For Sobel, update kernel size
                if (edgeNode->getAlgorithm() == EdgeDetectionAlgorithm::SOBEL) {
                    edgeNode->setSobelParameters(value);
                    edgeNode->process();
                    updateAllNodePreviews();
                }
            }
        }
    }
}
void MainWindow::overlayChanged(bool checked) {
    auto selectedItems = m_scene->selectedItems();
    if (!selectedItems.isEmpty()) {
        if (auto nodeItem = dynamic_cast<NodeItem*>(selectedItems.first())) {
            if (auto edgeNode = dynamic_cast<EdgeDetectionNode*>(nodeItem->getNode())) {
                edgeNode->setOverlay(checked);
                edgeNode->process();
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
        if(!sourceShared || !targetShared) {
            std::cerr << "Invalid connection: one of the nodes is null" << std::endl;
            return;
        }
        std::cout << "Creating connection: " << src << " -> " << tgt << std::endl;
        m_graph->connect(sourceShared, targetShared);
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
        if(!sourceShared || !targetShared) {
            std::cerr << "Invalid connection: one of the nodes is null" << std::endl;
            return;
        }
        std::cout << "Creating connection: " << src << " -> " << tgt << std::endl;
        m_graph->connect(sourceShared, targetShared);
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
        if(!sourceShared || !targetShared) {
            std::cerr << "Invalid connection: one of the nodes is null" << std::endl;
            return;
        }
        std::cout << "Creating connection: " << src << " -> " << tgt << std::endl;
        m_graph->connect(sourceShared, targetShared);
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
        if(!sourceShared || !targetShared) {
            std::cerr << "Invalid connection: one of the nodes is null" << std::endl;
            return;
        }
        std::cout << "Creating connection: " << src << " -> " << tgt << std::endl;
        m_graph->connect(sourceShared, targetShared);
        updateAllEdges();
    });
    updateAllNodePreviews();
}
