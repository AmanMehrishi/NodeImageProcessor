#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QDockWidget>
#include <QLabel>
#include <QSlider>
#include <QPushButton>
#include <memory>

class BrightnessContrastNode;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QGraphicsScene* getScene() const { return m_scene; }

private slots:
    void updatePropertiesPanel();
    void brightnessChanged(int value);
    void contrastChanged(int value);
    void resetParameters();

private:
    void createMenu();
    void createPropertiesPanel();

    QGraphicsScene *m_scene;
    QDockWidget *m_propertiesDock;
    QWidget *m_propertiesWidget;
    QLabel *m_nodeNameLabel;

    QLabel *m_brightnessLabel;
    QSlider *m_brightnessSlider;
    QLabel *m_contrastLabel;
    QSlider *m_contrastSlider;
    QPushButton *m_resetButton;

    std::shared_ptr<BrightnessContrastNode> m_bcNode;
};

#endif // MAINWINDOW_H