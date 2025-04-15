#ifndef IMAGE_INPUT_NODE_H
#define IMAGE_INPUT_NODE_H

#include "Node.h"

using namespace std;


class ImageInputNode : public Node {
    public:
        ImageInputNode(const string& id) : Node(id) {}

        void setImagePath(const string& path) {imagePath = path;}

        cv::Mat getOutput() const override {return output;}

        virtual void process() override;

        string getMetadata() const;
    
    private:
        string imagePath;
        cv::Mat output;
};


#endif