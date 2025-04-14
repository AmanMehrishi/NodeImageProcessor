#include <iostream>
#include <QApplication>
#include <filesystem>
#include "MainWindow.h"
#include "ImageInputNode.h"
#include "NodeItem.h"
#include "EdgeDetectionNode.h"
#include "OutputNode.h"
#include "BrightnessContrastNode.h"
#include "NodeGraph.h"

using namespace std;

int main(int argc, char *argv[]) {
    try {
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
        bcNode->setBrightness(30);
        bcNode->setContrast(1.1);
        graph.addNode(bcNode);

        // auto edgeNode = make_shared<EdgeDetectionNode>("edge_detection_node");
        // edgeNode->setInput(bcNode->getOutput());
        // edgeNode->setAlgorithm(EdgeDetectionAlgorithm::CANNY);
        // edgeNode->setCannyParameters(50, 150, 3);
        // graph.addNode(edgeNode);

        // auto outputNode = make_shared<OutputNode>("output_node");
        // outputNode->setInput(edgeNode->getOutput());
        // outputNode->setOutputPath("output/final_results.jpg");
        // graph.addNode(outputNode);

        // graph.connect(inputNode, bcNode);
        // graph.connect(bcNode, edgeNode);
        // graph.connect(edgeNode, outputNode);

        graph.processAll();

        
        if (!std::filesystem::exists("output")) {
            std::filesystem::create_directory("output");
        }

        NodeItem *inputItem = new NodeItem(QString::fromStdString(inputNode->getId()));
        inputItem->setPreview(inputNode->getOutput());
        inputItem->setPos(50, 50);
        scene->addItem(inputItem);
        inputItem->setNode(inputNode.get());

        NodeItem *bcItem = new NodeItem(QString::fromStdString(bcNode->getId()));
        bcItem->setPreview(bcNode->getOutput());
        bcItem->setPos(250, 50);
        bcItem->setNode(bcNode.get());
        scene->addItem(bcItem);


        // NodeItem *edgeItem = new NodeItem(QString::fromStdString(edgeNode->getId()));
        // edgeItem->setPreview(edgeNode->getOutput());
        // edgeItem->setPos(450, 50);
        // scene->addItem(edgeItem);

        // NodeItem *outputItem = new NodeItem(QString::fromStdString(outputNode->getId()));
        // outputItem->setPreview(outputNode->getOutput());
        // outputItem->setPos(650, 50);
        // scene->addItem(outputItem);


        cout << "COMPLETE" << endl;

        window.show();
        return app.exec();
    } catch (const std::exception &ex) {
        std::cerr << "Unhandled exception: " << ex.what() << std::endl;
        return -1;
    } catch (...) {
        std::cerr << "Unhandled unknown exception." << std::endl;
        return -1;
    }
}
