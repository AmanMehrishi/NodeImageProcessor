#include <iostream>
#include <QApplication>
#include <filesystem>
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
#include <memory>

using namespace std;


int main(int argc, char *argv[]){
    try{
        QApplication app(argc, argv);
        MainWindow window;
        window.setWindowTitle("Node-Based Image Processor");
        window.resize(1000, 700);
        if(!std::filesystem::exists("output")){
            std::filesystem::create_directory("output");
        }
        std::cout<<"COMPLETE"<<std::endl;
        window.show();
        return app.exec();
    } catch(const std::exception &ex){
        std::cerr<<"Unhandled exception: "<<ex.what()<<std::endl;
        return -1;
    } catch(...){
        std::cerr<<"Unhandled unknown exception."<<std::endl;
        return -1;
    }
}

