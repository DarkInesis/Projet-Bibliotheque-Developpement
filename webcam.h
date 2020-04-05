#ifndef WEBCAM_H
#define WEBCAM_H

#include "opencv2/video/tracking.hpp"
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/objdetect.hpp>
#include <QObject>
#include <QThread>
#include <cstdio>
#include <iostream>

class Webcam : public QThread {
    Q_OBJECT

   public:
    Webcam();
    void detectMotion();
    void detectFace();
    void getDirection();
    void resetInitFace();
    void needUpdate() { isUpdated = false; };

    enum Move { AVANT, ARRIERE, DROITE, GAUCHE, NEUTRE };

   protected:
    void run() override;

   private:
    bool isUpdated = false;
    cv::CascadeClassifier face_cascade;
    int frameWidth = 640;
    int frameHeight = 480;
    cv::VideoCapture cap;
    cv::Rect face;
    cv::Mat init_frame, current_frame;
    cv::Point p, vect, face_center;
    Move direction;
    int counterAbsurde = 0, counterChangeDirection = 0;

   signals:
    void webcamFrameCaptured(cv::Mat*);
    void directionChanged(Webcam::Move);
};

#endif  // WEBCAM_H
