#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/tracking.hpp>

#include "classes/YoloDetektor.h"



int main(int argc, char **argv) {

    std::cout << "OpenCV ver: " << CV_VERSION << std::endl;

    size_t counter = 0;

    if(argc < 4){
        return -1;
    }

    std::string inFileName = argv[1];

    cv::VideoCapture videoCapture(inFileName);

    if(!videoCapture.isOpened()){
        std::cerr << "nelze otevrit video" << std::endl;
        return -1;
    }

    double frameWidth = videoCapture.get(cv::CAP_PROP_FRAME_WIDTH);
    double frameHeight = videoCapture.get(cv::CAP_PROP_FRAME_HEIGHT);
    double fps = videoCapture.get(cv::CAP_PROP_FPS);
    auto fourcc = cv::VideoWriter::fourcc('M', 'J', 'P','G');


    //cv::namedWindow("Video", cv::WINDOW_GUI_NORMAL);

    auto* detektor = new YoloDetektor(argv[2], argv[3]);
    cv::Ptr<cv::Tracker> tracker = cv::TrackerMOSSE::create();

    std::vector<cv::Point> barPath;
    bool isTrackerInitialized = false;
    cv::Rect2d detection;
    cv::Mat frame;
    double scale = 640.0 / frameHeight;

    cv::VideoWriter videoWriter("output.avi", fourcc, fps, cv::Size(frameWidth * scale,frameHeight * scale));

    if(!videoWriter.isOpened()){
        std::cerr << "Error zapisu souboru" << std::endl;
        return -1;
    }

    while(true){
        videoCapture >> frame;
        if(frame.empty()){
            break; //konec videa
        }

        std::cout << "Frame nr. " << ++counter << std::endl;

        cv::resize(frame, frame, cv::Size(), scale, scale, cv::INTER_AREA);

        if(!isTrackerInitialized) {
            //uvodni frame inicializace trackeru pomoci detektoru
            detection = detektor->detectObject(frame);
            tracker->init(frame, detection);
            isTrackerInitialized = true;
        }else{
            bool ok = tracker->update(frame, detection);

            if(!ok){
                //tracker selhal-> nova detekce pomoci detektoru
                std::cout << "Tracker failed" << std::endl;
                detection = detektor->detectObject(frame);
                tracker->init(frame, detection);
            }
        }
        //pridani bodu drahy osy jako prostredku ohranicujiciho boxu
        barPath.emplace_back(cv::Point(detection.x + (detection.width/2), detection.y + (detection.height /2)));

        //vykresleni drahy
        for(size_t i = 1; i < barPath.size(); ++i){
            cv::line(frame, barPath[i-1], barPath[i], cv::Scalar(255,255,0), 4);
        }

        //vykresleni boxu ohranicujici detekci
        cv::rectangle(frame, detection, cv::Scalar(255,0,255));

        videoWriter.write(frame);
/*
        cv::imshow("Video", frame);

        int keyPress = cv::waitKey(1);
        if(keyPress == 27){
            break;
        }
*/
    }

    videoWriter.release();
    videoCapture.release();
    //cv::destroyAllWindows();
    tracker.release();
    delete detektor;

    return 0;
}
