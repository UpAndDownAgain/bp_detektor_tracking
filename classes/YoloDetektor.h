//
// Created by nrg on 3/10/20.
//

#ifndef BP_DETEKTOR_TRACKING_YOLODETEKTOR_H
#define BP_DETEKTOR_TRACKING_YOLODETEKTOR_H


#include <string>

#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>

class YoloDetektor {
private:
    double threshold;
    cv::dnn::Net net;
    double scaleFactor = 1.0/255;
    cv::Size size = cv::Size(480, 480);
    std::vector<cv::Mat> preprocess(cv::Mat &frame);
    cv::Rect postProcess(std::vector<cv::Mat> &outs);
    cv::Rect closestDetection(std::vector<cv::Rect> &detections);

public:
    YoloDetektor(std::string &cfg, std::string &weights);
    cv::Rect2d detectObject(cv::Mat &frame);
};


#endif //BP_DETEKTOR_TRACKING_YOLODETEKTOR_H
