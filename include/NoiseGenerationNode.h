#ifndef NOISE_GENERATION_NODE_H
#define NOISE_GENERATION_NODE_H
#include "Node.h"
#include <opencv2/opencv.hpp>
enum class NoiseType { PERLIN, SIMPLEX, WORLEY };
class NoiseGenerationNode : public Node {
public:
    NoiseGenerationNode(const std::string &id) : Node(id), scale(1.0), octaves(1), persistence(0.5), noiseType(NoiseType::PERLIN), asDisplacement(false) {}
    void process() override;
    cv::Mat getOutput() const override { return output; }
    void setScale(double s) { scale = s; }
    double getScale() const { return scale; }
    void setOctaves(int o) { octaves = o; }
    int getOctaves() const { return octaves; }
    void setPersistence(double p) { persistence = p; }
    double getPersistence() const { return persistence; }
    void setNoiseType(NoiseType n) { noiseType = n; }
    NoiseType getNoiseType() const { return noiseType; }
    void setAsDisplacement(bool flag) { asDisplacement = flag; }
    bool getAsDisplacement() const { return asDisplacement; }
private:
    cv::Mat output;
    double scale;
    int octaves;
    double persistence;
    NoiseType noiseType;
    bool asDisplacement;
};
#endif
