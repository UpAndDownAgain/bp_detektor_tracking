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
    cv::Rect detection = postProcess(frame, detections);



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

cv::Rect YoloDetektor::postProcess(cv::Mat &frame, std::vector<cv::Mat> &outs) {
    std::vector<cv::Rect> boxes;
    std::vector<double> confidences;
    std::vector<int> outLayers = net.getUnconnectedOutLayers();
    std::vector<int> classIDs;
    std::string outLayerType = net.getLayer(outLayers[0])->type;

    if(!outs.empty()){
        for(auto &i : outs){
            auto *data = (float*) i.data;

            for(size_t j = 0; j < i.rows; ++j, data += i.cols){
                cv::Mat scores = i.row(j).colRange(5,i.cols);
                cv::Point classIdPoint;
                double confidence;
                cv::minMaxLoc(scores, nullptr, &confidence, nullptr, &classIdPoint);

                if(confidence > threshold){
                    int centerX = (int)(data[0] * frame.cols);
                    int centerY = (int)(data[1] * frame.rows);
                    int width = (int)(data[2] * frame.cols);
                    int height = (int)(data[3] * frame.rows);
                    int left = centerX - width / 2;
                    int top = centerY - height / 2;

                    boxes.emplace_back(left, top, width, height);
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
}
