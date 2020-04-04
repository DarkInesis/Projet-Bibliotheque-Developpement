#ifndef WEBCAM_H
#define WEBCAM_H

#include "opencv2/video/tracking.hpp"
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/objdetect.hpp>
#include <QObject>
#include <cstdio>
#include <iostream>

class Webcam : public QObject{
    Q_OBJECT
     public:
      Webcam(QWidget *parent = nullptr);
      cv::Mat captureMotion();
      int mult_capture();
      void initModel();
      bool getNeedWebcamInitialization();
      void captureOrientation();
      std::string getDirection();
      int getCounterConsecutivesNull();

      void resetAbsurdsDetectionStates();
     private:
      cv::VideoCapture cap;
      cv::CascadeClassifier face_cascade;
      cv::Mat frame;
      cv::Mat oldFrame;
      cv::Mat oldFrame_gray;
      cv::Mat modelFace;
      cv::Mat firstModel;
      cv::Rect templateRect;

      int frameWidth;
      int frameHeight;
      int templateWidth;
      int templateHeight;
      std::string direction="null";
      bool needWebcamInitialization = true;
      int counterConsecutivesAbsurdsDetections = 0;
      int consecutiveAbsurdsDetectionsLimit = 10;
      int counterConsecutivesNull=0;

      //Setter
      void setDirection(std::string);
      void setneedWebcamInitializationState(bool);
      void setCounterConsecutivesNull(int newCount);
    signals:
      void webcamFrameCaptured(cv::Mat*);
      void directionChanged(QString);
      void needToPaint2DLabyrinthe(bool);
      void needWebcamInitializationStateChanged(bool);
    public slots:
      void capture();

};

#endif  // WEBCAM_H
