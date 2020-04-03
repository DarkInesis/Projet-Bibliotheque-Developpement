#include "webcam.h"

using namespace cv;
using namespace std;

Webcam::Webcam() {
      // data
      frameWidth = 640;
      frameHeight = 480;
      templateWidth = 40;
      templateHeight = 40;
      // Init state variable
      needWebcamInitialization = true;
      this->counterConsecutivesAbsurdsDetections = 0;
      // Init webcam
      cap.open(0);
      cap.set(CAP_PROP_FRAME_WIDTH, frameWidth);
      cap.set(CAP_PROP_FRAME_HEIGHT, frameHeight);
      if (!cap.isOpened())  // check if we succeeded
      {
            cerr << "Error openning the default camera" << endl;
      }
      if (!face_cascade.load(
              "../Projet-Bibliotheque-Developpement/resources/haarcascade_frontalface_alt2.xml")) {
            cerr << "Error loading haarcascade" << endl;
      }
      // get the firt capt
      cap >> oldFrame;
      // Mirror effect
      cv::flip(oldFrame, oldFrame, 1);
      // Convert to gray
      cv::cvtColor(oldFrame, oldFrame_gray, COLOR_BGR2GRAY);
}

Mat Webcam::captureOrientation() {
      Mat frame, frame_gray;
      std::vector<Rect> faces;
      // Get frame
      cap >> frame;
      // Mirror effect
      cv::flip(frame, frame, 1);
      // Convert to gray
      cv::cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
      // Equalize graylevels
      equalizeHist(frame_gray, frame_gray);

      // Create the matchTemplate image result
      Mat resultImage;  // to store the matchTemplate result
      int result_cols = frame.cols - templateWidth + 1;
      int result_rows = frame.rows - templateHeight + 1;
      resultImage.create(result_cols, result_rows, CV_32FC1);
      //-- Detect faces
      face_cascade.detectMultiScale(frame_gray, faces, 1.1, 4, 0, Size(60, 60));
      // init direction
      direction="no_detection";
      if (faces.size() == 1) {
            Rect workingRect = faces[0];
            templateRect = Rect(workingRect.x + (workingRect.width - templateWidth) / 2,
                                workingRect.y + (workingRect.height - templateHeight) / 2,
                                templateWidth, templateHeight);
            cv::Point workingCenter(workingRect.x + workingRect.width / 2,
                                    workingRect.y + workingRect.height / 2);
            if (modelFace.empty()) {
                  initModel();
            }

            // Do the Matching between the working rect in frame2 and the templateImage in frame1
            matchTemplate(frame_gray, modelFace, resultImage, TM_CCORR_NORMED);
            // Localize the best match with minMaxLoc
            double minVal;
            double maxVal;
            cv::Point minLoc;
            cv::Point maxLoc;
            minMaxLoc(resultImage, &minVal, &maxVal, &minLoc, &maxLoc);
            // Compute the translation vector between the origin and the matching rect
            cv::Point vect(maxLoc.x - templateRect.x, maxLoc.y - templateRect.y);
            // Draw green rectangle and the translation vector
            rectangle(frame, workingRect, Scalar(0, 255, 0), 2);
            rectangle(frame, templateRect, Scalar(255, 0, 0), 2);
            cv::Point p(workingCenter.x + vect.x, workingCenter.y + vect.y);
            arrowedLine(frame, workingCenter, p, Scalar(255, 255, 255), 2);
            // Cas d'un mouvement raisonnable
            if (abs(vect.x) < 50 && abs(vect.y) < 50) {
                  int motionDetect_minLimit_vertical = 3;
                  int motionDetect_minLimit_horizontal = 7;
                  // Cas d'un mouvement léger
                  Rect MatchedZone(p.x - templateWidth / 2, p.y - templateHeight / 2, templateWidth,
                                   templateHeight);
                  if (vect.x > motionDetect_minLimit_horizontal) {
                        direction = "droite";
                        this->counterConsecutivesNull=0;
                  } else if (vect.x < -motionDetect_minLimit_horizontal) {
                        direction = "gauche";
                        this->counterConsecutivesNull=0;
                  } else if (vect.y > motionDetect_minLimit_vertical) {
                        direction = "bas";
                        this->counterConsecutivesNull=0;
                  } else if (vect.y < -motionDetect_minLimit_vertical) {
                        direction = "haut";
                        this->counterConsecutivesNull=0;
                  } else {
                        direction = "null";
                        this->counterConsecutivesNull+=1;
                        // Redefinition du model ( à revoir)
                        this->modelFace = Mat(frame_gray, MatchedZone);
                  }
                  std::cout << direction << endl;

                  // Dessin du rectangle
                  rectangle(frame, MatchedZone, Scalar(255, 255, 0), 2);
            } else {
                  this->counterConsecutivesAbsurdsDetections += 1;
                  if (this->counterConsecutivesAbsurdsDetections >=
                      this->consecutiveAbsurdsDetectionsLimit) {
                        this->needWebcamInitialization = true;
                  }
            }
      }
      // Swap matrixes
      swap(oldFrame_gray, frame_gray);
      return frame;
}

cv::Mat Webcam::initModel() {
      Mat frame, frame_gray;
      std::vector<Rect> faces;
      // Get frame
      cap >> frame;
      // Mirror effect
      cv::flip(frame, frame, 1);
      // Convert to gray
      cv::cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
      // Equalize graylevels
      equalizeHist(frame_gray, frame_gray);

      // Create the matchTemplate image result
      Mat resultImage;  // to store the matchTemplate result
      int result_cols = frame.cols - templateWidth + 1;
      int result_rows = frame.rows - templateHeight + 1;
      resultImage.create(result_cols, result_rows, CV_32FC1);
      //-- Detect faces
      face_cascade.detectMultiScale(frame_gray, faces, 1.1, 4, 0, Size(60, 60));

      if (faces.size() == 1) {
            Rect workingRect = faces[0];
            templateRect = Rect(workingRect.x + (workingRect.width - templateWidth) / 2,
                                workingRect.y + (workingRect.height - templateHeight) / 2,
                                templateWidth, templateHeight);
            cv::Point workingCenter(workingRect.x + workingRect.width / 2,
                                    workingRect.y + workingRect.height / 2);
            rectangle(frame, workingRect, Scalar(0, 255, 0), 2);
            rectangle(frame, templateRect, Scalar(255, 0, 0), 2);
            this->modelFace = Mat(frame_gray, templateRect);
      }

      // Swap matrixes
      swap(oldFrame_gray, frame_gray);
      return frame;
}

bool Webcam::getNeedWebcamInitialization() { return this->needWebcamInitialization; }

void Webcam::resetAbsurdsDetectionStates() {
      this->needWebcamInitialization = false;
      this->counterConsecutivesAbsurdsDetections = 0;
}
std::string Webcam::getDirection(){
    return this->direction;
}
int Webcam::getCounterConsecutivesNull(){
    return this->counterConsecutivesNull;
}
