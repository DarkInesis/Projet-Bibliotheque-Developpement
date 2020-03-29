#ifndef WEBCAM_H
#define WEBCAM_H

#include "opencv2/video/tracking.hpp"
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/objdetect.hpp>

#include <cstdio>
#include <iostream>
#include "window.h"

class webcam {
     public:
      webcam(class Window*);
      cv::Mat capture();
      int mult_capture();

     private:
      class Window* window;
      cv::VideoCapture cap;
      cv::CascadeClassifier face_cascade;
      cv::Mat oldFrame;
      cv::Mat oldFrame_gray;
      cv::Mat modelFace;

      int frameWidth;
      int frameHeight;
      int templateWidth;
      int templateHeight;
};

#endif  // WEBCAM_H
