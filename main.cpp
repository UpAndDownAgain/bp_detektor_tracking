#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>

#include "classes/YoloDetektor.h"

int main(int argc, char **argv) {

    std::cout << "OpenCV ver: " << CV_VERSION << std::endl;

    if(argc < 4){
        return -1;
    }

    cv::VideoCapture videoCapture(argv[1]);
    if(!videoCapture.isOpened()){
        std::cerr << "nelze otevrit video" << std::endl;
        return -1;
    }
    cv::namedWindow("Video", cv::WINDOW_GUI_NORMAL);
    //YoloDetektor detektor(argv[2], argv[3]);

    cv::Mat frame;
    double scale = 640.0 / videoCapture.get(cv::CAP_PROP_FRAME_HEIGHT);
    while(videoCapture.read(frame)){
        if(frame.empty()){
            break;
        }

        cv::resize(frame, frame, cv::Size(), scale, scale, cv::INTER_AREA);
        cv::imshow("Video", frame);
        int keyPress = cv::waitKey(1);
        if(keyPress == 27){
            break;
        }
    }


    return 0;
}
