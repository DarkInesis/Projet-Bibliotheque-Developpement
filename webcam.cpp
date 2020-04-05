#include "webcam.h"

using namespace std;
using namespace cv;

Webcam::Webcam() {
    // Fait en sorte que les slots de ce thread soit exécutés dans ce thread
    // moveToThread ne marche que sur les QObject sans parent
    setParent(0);
    moveToThread(this);

    cap = VideoCapture(0);  // open the default camera
    cout << "width :" << cap.get(CAP_PROP_FRAME_WIDTH) << endl;
    cout << "height :" << cap.get(CAP_PROP_FRAME_HEIGHT) << endl;
    cap.set(CAP_PROP_FRAME_WIDTH, frameWidth);
    cap.set(CAP_PROP_FRAME_HEIGHT, frameHeight);
    if (!cap.isOpened())  // check if we succeeded
    {
        cerr << "Error openning the default camera" << endl;
        return;
    }

    if (!face_cascade.load(
            "../Projet-Bibliotheque-Developpement/resources/haarcascade_frontalface_alt.xml")) {
        cerr << "Error loading haarcascade" << endl;
    }
}

void Webcam::detectMotion() {
    int templateWidth = 25;
    int templateHeight = 25;

    Rect templateRect((face.width - templateWidth) / 2, (face.height - templateHeight) / 2,
                      templateWidth, templateHeight);

    Mat frame1, frame2, frameRect1, frameRect2;

    // Get frame1
    frame1 = init_frame.clone();
    // Mirror effect
    cv::flip(frame1, frame1, 1);
    // Extract rect1 and convert to gray
    cv::cvtColor(Mat(frame1, face), frameRect1, COLOR_BGR2GRAY);
    // Mat(frame1,rect).copyTo(frameRect1);

    // Create the matchTemplate image result
    Mat resultImage;  // to store the matchTemplate result
    int result_cols = frame1.cols - templateWidth + 1;
    int result_rows = frame1.rows - templateHeight + 1;
    resultImage.create(result_cols, result_rows, CV_32FC1);

    // Get frame2
    frame2 = current_frame.clone();

    // Mirror effect
    cv::flip(frame2, frame2, 1);
    // Extract working rect in frame2 and convert to gray
    cv::cvtColor(Mat(frame2, face), frameRect2, COLOR_BGR2GRAY);

    // Extract template image in frame1
    Mat templateImage(frameRect1, templateRect);
    // Do the Matching between the working rect in frame2 and the templateImage in frame1
    matchTemplate(frameRect2, templateImage, resultImage, TM_CCORR_NORMED);
    // Localize the best match with minMaxLoc
    double minVal;
    double maxVal;
    Point minLoc;
    Point maxLoc;
    minMaxLoc(resultImage, &minVal, &maxVal, &minLoc, &maxLoc);
    // Compute the translation vector between the origin and the matching rect
    vect = Point(maxLoc.x - templateRect.x, maxLoc.y - templateRect.y);

    // Draw green rectangle and the translation vector
    p = Point(face_center.x + vect.x, face_center.y + vect.y);

    // std::cout << "vect.x = " << vect.x << "  vect.y = " << vect.y << endl;
}

void Webcam::detectFace() {
    Mat frame, frame_gray;
    std::vector<Rect> faces;

    // Get frame
    cap >> init_frame;

    current_frame = init_frame.clone();

    frame = init_frame.clone();

    // Mirror effect
    cv::flip(frame, frame, 1);
    // Convert to gray
    cv::cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
    // Equalize graylevels
    //        equalizeHist( frame_gray, frame_gray );
    //-- Detect faces
    face_cascade.detectMultiScale(frame_gray, faces, 1.1, 4, 0, Size(60, 60));

    if (faces.size() == 1) {
        face = faces[0];
        face_center = Point(face.x + face.width / 2, face.y + face.height / 2);
        isUpdated = true;
    } else {
        resetInitFace();
    }
}

void Webcam::getDirection() {
    Move new_direction;
    if (vect.x > 50 || vect.x < -50 || vect.y > 50 || vect.y < -50) {
        counterAbsurde++;
        if (counterAbsurde > 15) resetInitFace();
        new_direction = NEUTRE;
    } else if (vect.x > 15 && vect.y < 15 && vect.y > -15)
        new_direction = DROITE;
    else if (vect.x < -15 && vect.y < 15 && vect.y > -15)
        new_direction = GAUCHE;
    else if (vect.y > 15 && vect.x < 15 && vect.x > -15)
        new_direction = ARRIERE;
    else if (vect.y < -15 && vect.x < 15 && vect.x > -15)
        new_direction = AVANT;
    else
        new_direction = NEUTRE;

    //    if (new_direction == DROITE)
    //        cout << "Direction : DROTIE" << endl;
    //    else if (new_direction == GAUCHE)
    //        cout << "Direction : GAUCHE" << endl;
    //    else if (new_direction == ARRIERE)
    //        cout << "Direction : ARRIERE" << endl;
    //    else if (new_direction == AVANT)
    //        cout << "Direction : AVANT" << endl;
    //    else
    //        cout << "Direction : NEUTRE" << endl;

    if (new_direction != direction) {
        counterChangeDirection++;
        if (counterChangeDirection > 5 || new_direction == NEUTRE) {
            emit directionChanged(new_direction);
            counterChangeDirection = 0;
            direction = new_direction;
        }
    }
}

void Webcam::resetInitFace() {
    // Reupdate init face
    face = Rect(0, 0, 0, 0);
    face_center = Point(0, 0);
    isUpdated = false;
    counterAbsurde = 0;
}

void Webcam::run() {
    // Init output window
    // namedWindow("WebCam", 1);

    while (waitKey(5) < 0) {
        cap >> current_frame;

        if (!isUpdated)
            detectFace();
        else {
            detectMotion();
            getDirection();
        }
        // Display current_frame
        Mat* display_frame = new Mat(current_frame);
        cv::flip(*display_frame, *display_frame, 1);

        if (true) {
            rectangle(*display_frame, face, Scalar(0, 255, 0), 2);
            arrowedLine(*display_frame, face_center, p, Scalar(255, 255, 255), 2);
            emit webcamFrameCaptured(display_frame);
        }

        // imshow("WebCam", display_frame);
    }
}
