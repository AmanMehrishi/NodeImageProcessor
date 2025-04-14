#include <iostream>
#include <QApplication>
#include <filesystem>
#include "MainWindow.h"
#include "ImageInputNode.h"
#include "EdgeDetectionNode.h"
#include "OutputNode.h"
#include "BrightnessContrastNode.h"
#include "NodeGraph.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    MainWindow window;
    window.setWindowTitle("Node-Based Image Processor");
    window.resize(1000, 700);
    QGraphicsScene *scene = window.getScene();

    NodeGraph graph;

    auto inputNode = make_shared<ImageInputNode>("input_node");
    inputNode->setImagePath("resources/test.jpg");
    graph.addNode(inputNode);
    


    auto bcNode = make_shared<BrightnessContrastNode>("brightness_contrast_node");
    bcNode->setInput(inputNode->getOutput());
    bcNode->setBrightness(30);
    bcNode->setContrast(1.1);
    graph.addNode(bcNode);

    auto edgeNode = make_shared<EdgeDetectionNode>("edge_detection_node");
    edgeNode->setInput(bcNode->getOutput());
    edgeNode->setAlgorithm(EdgeDetectionAlgorithm::CANNY);
    edgeNode->setCannyParameters(50, 150, 3);
    graph.addNode(edgeNode);

    auto outputNode = make_shared<OutputNode>("output_node");
    outputNode->setInput(edgeNode->getOutput());
    outputNode->setOutputPath("output/final_results.jpg");
    graph.addNode(outputNode);

    graph.connect(inputNode, bcNode);

    graph.connect(bcNode, edgeNode);

    graph.connect(edgeNode, outputNode);

    graph.processAll();

    if(!std::filesystem::exists("output")) {
        std::filesystem::create_directory("output");
    }


    cout<<"COMPLETE"<<endl;


    window.show();
    return app.exec();
    return 0;
}