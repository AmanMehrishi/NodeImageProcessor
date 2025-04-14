#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QDockWidget>
#include <QLabel>
#include <QSlider>
#include <QPushButton>

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    
    QGraphicsScene* getScene() const { return m_scene; }
    
private slots:
    void updatePropertiesPanel();

private:
    void createMenu();
    void createPropertiesPanel();

    QGraphicsScene *m_scene;
    QDockWidget *m_propertiesDock;
    QWidget *m_propertiesWidget;
    QLabel *m_nodeNameLabel;
    QSlider *m_brightnessSlider;
    QSlider *m_contrastSlider;
    QPushButton *m_resetButton;
};

#endif // MAINWINDOW_H
