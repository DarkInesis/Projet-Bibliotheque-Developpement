#ifndef WEBCAM_H
#define WEBCAM_H

#include "opencv2/video/tracking.hpp"
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/objdetect.hpp>

#include <cstdio>
#include <iostream>

class Webcam {
     public:
      Webcam();
      cv::Mat captureMotion();
      cv::Mat captureOrientation();
      int mult_capture();
      cv::Mat initModel();
      bool getNeedWebcamInitialization();
      void resetAbsurdsDetectionStates();

     private:
      cv::VideoCapture cap;
      cv::CascadeClassifier face_cascade;
      cv::Mat oldFrame;
      cv::Mat oldFrame_gray;
      cv::Mat modelFace;
      cv::Mat firstModel;
      cv::Rect templateRect;

      int frameWidth;
      int frameHeight;
      int templateWidth;
      int templateHeight;

      bool needWebcamInitialization = true;
      int counterConsecutivesAbsurdsDetections = 0;
      int consecutiveAbsurdsDetectionsLimit = 10;
};

#endif  // WEBCAM_H
