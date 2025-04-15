#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QGraphicsScene>
#include <QDockWidget>
#include <QLabel>
#include <QSlider>
#include <QPushButton>
#include <QComboBox>
#include <QCheckBox>
#include "Node.h"
#include <QMap>
#include <memory>
class BrightnessContrastNode;
class NodeGraph;
class NodeItem;
class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QGraphicsScene* getScene() const { return m_scene; }
    void setBrightnessContrastNode(std::shared_ptr<BrightnessContrastNode> node);
private slots:
    void updatePropertiesPanel();
    void brightnessChanged(int value);
    void contrastChanged(int value);
    void resetParameters();
    void edgeAlgorithmChanged(int index);
    void lowerThresholdChanged(int value);
    void upperThresholdChanged(int value);
    void kernelSizeChanged(int value);
    void overlayChanged(bool checked);
    void addInputNode();
    void addBrightnessContrastNode();
    void addEdgeDetectionNode();
    void addOutputNode();
    void updateAllNodePreviews();
    void updateAllEdges();
private:
    void createMenu();
    void createPropertiesPanel();
    QMap<std::string, std::shared_ptr<Node>> nodeSharedMap;
    QGraphicsScene *m_scene;
    QDockWidget *m_propertiesDock;
    QWidget *m_propertiesWidget;
    QLabel *m_nodeNameLabel;
    QLabel *m_brightnessLabel;
    QSlider *m_brightnessSlider;
    QLabel *m_contrastLabel;
    QSlider *m_contrastSlider;
    QComboBox *m_edgeAlgoCombo;
    QLabel *m_lowerThresholdLabel;
    QSlider *m_lowerThresholdSlider;
    QLabel *m_upperThresholdLabel;
    QSlider *m_upperThresholdSlider;
    QLabel *m_kernelSizeLabel;
    QSlider *m_kernelSizeSlider;
    QCheckBox *m_overlayCheckBox;
    QPushButton *m_resetButton;
    std::shared_ptr<BrightnessContrastNode> m_bcNode;
    NodeGraph *m_graph;
    QMap<std::string, NodeItem*> nodeItemMap;
};
#endif
