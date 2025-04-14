#ifndef NODE_H
#define NODE_H


#include <string.h>
#include <opencv2/opencv.hpp>

using namespace std;

class Node{
    public:
        Node(const string& id) : id(id) {}
        virtual ~Node() {}
        virtual void process() = 0;
        virtual void setInput(const cv::Mat& in) { }
        virtual cv::Mat getOutput() const { return cv::Mat(); }
        string getId() const {return id;}

        
    protected:
        string id;
};




#endif