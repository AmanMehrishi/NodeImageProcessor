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
    void updateAllNodePreviews();
    void updateAllEdges();
    void setBrightnessContrastNode(std::shared_ptr<class BrightnessContrastNode> node);
private slots:
    void resetParameters();
    void updatePropertiesPanel();
    void brightnessChanged(int value);
    void contrastChanged(int value);
    void edgeAlgorithmChanged(int index);
    void lowerThresholdChanged(int value);
    void upperThresholdChanged(int value);
    void kernelSizeChanged(int value);
    void overlayChanged(bool checked);
    void colorGrayscaleChanged(bool checked);
    void blurRadiusChanged(int value);
    void blurDirectionalChanged(bool checked);
    void thresholdChanged(int value);
    void thresholdMethodChanged(int index);
    void blendOpacityChanged(int value);
    void blendModeChanged(int index);
    void noiseScaleChanged(int value);
    void noiseOctavesChanged(int value);
    void noisePersistenceChanged(int value);
    void noiseTypeChanged(int index);
    void noiseAsDisplacementChanged(bool checked);
    void convPresetChanged(int index);
    void addInputNode();
    void addBrightnessContrastNode();
    void addEdgeDetectionNode();
    void addOutputNode();
    void addColorChannelSplitterNode();
    void addBlurNode();
    void addThresholdNode();
    void addBlendNode();
    void addNoiseGenerationNode();
    void addConvolutionFilterNode();
private:
    void createMenu();
    void createPropertiesPanel();
    std::shared_ptr<BrightnessContrastNode> m_bcNode;
    QGraphicsScene *m_scene;
    QDockWidget *m_propertiesDock;
    QWidget *m_propertiesWidget;
    QLabel *m_nodeNameLabel;
    QSlider *m_brightnessSlider;
    QLabel *m_brightnessLabel;
    QSlider *m_contrastSlider;
    QLabel *m_contrastLabel;
    QComboBox *m_edgeAlgoCombo;
    QSlider *m_lowerThresholdSlider;
    QLabel *m_lowerThresholdLabel;
    QSlider *m_upperThresholdSlider;
    QLabel *m_upperThresholdLabel;
    QSlider *m_kernelSizeSlider;
    QLabel *m_kernelSizeLabel;
    QCheckBox *m_overlayCheckBox;
    QCheckBox *m_colorGrayscaleCheckBox;
    QSlider *m_blurRadiusSlider;
    QCheckBox *m_blurDirectionalCheckBox;
    QSlider *m_thresholdSlider;
    QComboBox *m_thresholdMethodCombo;
    QSlider *m_blendOpacitySlider;
    QComboBox *m_blendModeCombo;
    QSlider *m_noiseScaleSlider;
    QSlider *m_noiseOctavesSlider;
    QSlider *m_noisePersistenceSlider;
    QComboBox *m_noiseTypeCombo;
    QCheckBox *m_noiseAsDisplacementCheckBox;
    QComboBox *m_convPresetCombo;
    QPushButton *m_resetButton;
    NodeGraph *m_graph;
    QMap<std::string, NodeItem*> nodeItemMap;
    QMap<std::string, std::shared_ptr<class Node>> nodeSharedMap;
};
#endif