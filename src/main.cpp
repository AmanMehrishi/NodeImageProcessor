#include <iostream>
#include <QApplication>
#include <filesystem>
#include "MainWindow.h"
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
        cout<<"COMPLETE"<<endl;
        window.show();
        return app.exec();
    }
    catch(const std::exception &ex){
        std::cerr<<"Unhandled exception: "<<ex.what()<<std::endl;
        return -1;
    }
    catch(...){
        std::cerr<<"Unhandled unknown exception."<<std::endl;
        return -1;
    }
}
