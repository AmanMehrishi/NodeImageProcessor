#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>


class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QGraphicsScene* getScene() { return scene; }

private:
    void createMenu();
    void createPropertiesPanel();
    QGraphicsScene* scene;
    QWidget *propertiesWidget;
};

#endif 