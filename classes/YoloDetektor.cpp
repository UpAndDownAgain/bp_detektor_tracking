//
// Created by nrg on 3/10/20.
//

#include "YoloDetektor.h"

YoloDetektor::YoloDetektor(std::string &cfg, std::string &weights) {
    net = cv::dnn::readNetFromDarknet(cfg, weights);
    net.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
    net.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);
}

cv::Rect2d YoloDetektor::detectObject(cv::Mat &frame) {

    std::vector<cv::Rect> boxes;
    std::vector<double> confidences;
    auto detections = preprocess(frame);


}

std::vector<cv::Mat> YoloDetektor::preprocess(cv::Mat &frame) {
    std::vector<cv::Mat> outputs;
    cv::Mat blob;
    cv::dnn::blobFromImage(frame, blob, scaleFactor, size,
                           cv::Scalar(0,0,0), true, false), CV_8U;

    net.setInput(blob);
    net.forward(outputs);
    return outputs;
}

cv::Rect YoloDetektor::postProcess(std::vector<cv::Mat> &outs) {
    std::vector<cv::Rect> boxes;
    std::vector<double> confidences;

    if(!outs.empty()){
        for(auto &item : outs){
            auto *data = (float*) item.data;

            for(size_t i = 0; i < item.total(); ++i){
                double conf = data[i+2];
                if(conf > threshold){

                }
            }
        }
    }
    return closestDetection(boxes);
}

cv::Rect YoloDetektor::closestDetection(std::vector<cv::Rect> &detections) {
    cv::Rect closestDetection;
    int closestDetectArea = -1;

    for(auto &box : detections){
        int area = box.width * box.height;
        if(area > closestDetectArea){
            closestDetectArea = area;
            closestDetection = box;
        }
    }
    return closestDetection;

    return cv::Rect();
}
