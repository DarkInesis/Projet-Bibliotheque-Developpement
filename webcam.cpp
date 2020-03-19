#include "webcam.h"

using namespace cv;
using namespace std;


webcam::webcam(class Window* window)
{
    // data
    frameWidth=640;
    frameHeight=480;
    templateWidth=40;
    templateHeight=40;
    // Init webcam
    cap.open(0);
    cap.set(CAP_PROP_FRAME_WIDTH,frameWidth);
    cap.set(CAP_PROP_FRAME_HEIGHT,frameHeight);
    if(!cap.isOpened())  // check if we succeeded
    {
        cerr<<"Error openning the default camera"<<endl;
    }
    if( !face_cascade.load( "../Projet-Bibliotheque-Developpement/resources/haarcascade_frontalface_alt.xml" ) )
    {
        cerr<<"Error loading haarcascade"<<endl;
    }
    // get the firt capt
    cap >> oldFrame;
    // Mirror effect
    cv::flip(oldFrame,oldFrame,1);
    // Convert to gray
    cv::cvtColor(oldFrame,oldFrame_gray,COLOR_BGR2GRAY);
    this->window=window;
}

Mat webcam::captureMotion(){
    std::string direction;

    Mat frame,frame_gray;
    std::vector<Rect> faces;
    // Get frame
    cap >> frame;
    // Mirror effect
    cv::flip(frame,frame,1);
    // Convert to gray
    cv::cvtColor(frame,frame_gray,COLOR_BGR2GRAY);
    // Equalize graylevels
    equalizeHist( frame_gray, frame_gray );



    // Create the matchTemplate image result
    Mat resultImage;    // to store the matchTemplate result
    int result_cols =  frame.cols-templateWidth  + 1;
    int result_rows = frame.rows-templateHeight + 1;
    resultImage.create( result_cols, result_rows, CV_32FC1 );
    //-- Detect faces
    face_cascade.detectMultiScale( frame_gray, faces, 1.1, 4, 0,Size(60, 60) );

    if (faces.size()==1)
    {
         Rect workingRect=faces[0];
         if(templateRect.empty()){
             templateRect=Rect(workingRect.x+(workingRect.width-templateWidth)/2,workingRect.y+(workingRect.height-templateHeight)/2,templateWidth,templateHeight);
         }
         // Rect templateRect(workingRect.x+(workingRect.width-templateWidth)/2,workingRect.y+(workingRect.height-templateHeight)/2,templateWidth,templateHeight);
         Point workingCenter(workingRect.x+workingRect.width/2,workingRect.y+workingRect.height/2);
         if(modelFace.empty()){
             modelFace=cv::Mat(oldFrame_gray,templateRect);
         }
         // Extract template image in oldFrame
         // Do the Matching between the working rect in frame2 and the templateImage in frame1
         matchTemplate( frame_gray, modelFace, resultImage, TM_CCORR_NORMED );
         // Localize the best match with minMaxLoc
         double minVal; double maxVal; Point minLoc; Point maxLoc;
         minMaxLoc( resultImage, &minVal, &maxVal, &minLoc, &maxLoc);
         // Compute the translation vector between the origin and the matching rect
         Point vect(maxLoc.x-templateRect.x,maxLoc.y-templateRect.y);
         // Draw green rectangle and the translation vector
         rectangle(frame,workingRect,Scalar( 0, 255, 0),2);
         rectangle(frame,templateRect,Scalar( 255, 0, 0),2);
         Point p(workingCenter.x+vect.x,workingCenter.y+vect.y);
         arrowedLine(frame,workingCenter,p,Scalar(255,255,255),2);
         // Cas d'un mouvement raisonnable
         if(abs(vect.x)<30 && abs(vect.y)<30){
             //int motionDetect_minLimit=10;
             int motionDetect_minLimit=2;
             // Cas d'un mouvement léger
             if(vect.x>motionDetect_minLimit){direction="droite";}
             else if(vect.x<-motionDetect_minLimit){direction="gauche";}
             else if(vect.y>motionDetect_minLimit){direction="bas";}
             else if (vect.y<-motionDetect_minLimit){direction="haut";}
             else {direction="null";}
             std::cout<<direction<<endl;
             /*
             // Redefinition du model ( à revoir)
             Rect newModel(p.x-templateWidth/2,p.y-templateHeight/2,templateWidth,templateHeight);
             this->modelFace=Mat(frame_gray,newModel);
             // Dessin du rectangle
             rectangle(frame,newModel,Scalar(255,255,0),2);
             */
             // Redefinition du model ( à revoir)
             Rect newModel(p.x-templateWidth/2,p.y-templateHeight/2,templateWidth,templateHeight);
             this->modelFace=Mat(frame_gray,newModel);
             templateRect=newModel;
             // Dessin du rectangle
             rectangle(frame,newModel,Scalar(255,255,0),2);
         }
         // Cas contraire
         else{
            // Si gros mouvement, le model se réinitialise avec la zone centrale
            this->modelFace=Mat(frame_gray,templateRect);
        }
    }
    // Swap matrixes
    swap(oldFrame_gray,frame_gray);
    return frame;
}

Mat webcam::captureOrientation(){
    std::string direction;

    Mat frame,frame_gray;
    std::vector<Rect> faces;
    // Get frame
    cap >> frame;
    // Mirror effect
    cv::flip(frame,frame,1);
    // Convert to gray
    cv::cvtColor(frame,frame_gray,COLOR_BGR2GRAY);
    // Equalize graylevels
    equalizeHist( frame_gray, frame_gray );



    // Create the matchTemplate image result
    Mat resultImage;    // to store the matchTemplate result
    int result_cols =  frame.cols-templateWidth  + 1;
    int result_rows = frame.rows-templateHeight + 1;
    resultImage.create( result_cols, result_rows, CV_32FC1 );
    //-- Detect faces
    face_cascade.detectMultiScale( frame_gray, faces, 1.1, 4, 0,Size(60, 60) );

    if (faces.size()==1)
    {
         Rect workingRect=faces[0];
         Rect templateRect(workingRect.x+(workingRect.width-templateWidth)/2,workingRect.y+(workingRect.height-templateHeight)/2,templateWidth,templateHeight);
         Point workingCenter(workingRect.x+workingRect.width/2,workingRect.y+workingRect.height/2);
         if(modelFace.empty()){
             modelFace=cv::Mat(oldFrame_gray,templateRect);
         }
         // Extract template image in oldFrame
         // Do the Matching between the working rect in frame2 and the templateImage in frame1
         matchTemplate( frame_gray, modelFace, resultImage, TM_CCORR_NORMED );
         // Localize the best match with minMaxLoc
         double minVal; double maxVal; Point minLoc; Point maxLoc;
         minMaxLoc( resultImage, &minVal, &maxVal, &minLoc, &maxLoc);
         // Compute the translation vector between the origin and the matching rect
         Point vect(maxLoc.x-templateRect.x,maxLoc.y-templateRect.y);
         // Draw green rectangle and the translation vector
         rectangle(frame,workingRect,Scalar( 0, 255, 0),2);
         rectangle(frame,templateRect,Scalar( 255, 0, 0),2);
         Point p(workingCenter.x+vect.x,workingCenter.y+vect.y);
         arrowedLine(frame,workingCenter,p,Scalar(255,255,255),2);
         // Cas d'un mouvement raisonnable
         if(abs(vect.x)<30 && abs(vect.y)<30){
             //int motionDetect_minLimit=10;
             int motionDetect_minLimit=5;
             // Cas d'un mouvement léger
             if(vect.x>motionDetect_minLimit){direction="droite";}
             else if(vect.x<-motionDetect_minLimit){direction="gauche";}
             else if(vect.y>motionDetect_minLimit){direction="bas";}
             else if (vect.y<-motionDetect_minLimit){direction="haut";}
             else {direction="null";}
             std::cout<<direction<<endl;
             // Redefinition du model ( à revoir)
             Rect newModel(p.x-templateWidth/2,p.y-templateHeight/2,templateWidth,templateHeight);
             this->modelFace=Mat(frame_gray,newModel);
             templateRect=newModel;
             // Dessin du rectangle
             rectangle(frame,newModel,Scalar(255,255,0),2);
         }
         // Cas contraire
         else{
            // Si gros mouvement, le model se réinitialise avec la zone centrale
            this->modelFace=Mat(frame_gray,templateRect);
        }
    }
    // Swap matrixes
    swap(oldFrame_gray,frame_gray);
    return frame;
}

int webcam::mult_capture()
    {
        int frameWidth=640;
        int frameHeight=480;
        int templateWidth=40;
        int templateHeight=40;
        std::string direction;

        VideoCapture cap(0); // open the default camera
        cap.set(CAP_PROP_FRAME_WIDTH,frameWidth);
        cap.set(CAP_PROP_FRAME_HEIGHT,frameHeight);
        if(!cap.isOpened())  // check if we succeeded
        {
            cerr<<"Error openning the default camera"<<endl;
            return -1;
        }
        CascadeClassifier face_cascade;
        if( !face_cascade.load( "../TestDetectMultiScale/haarcascade_frontalface_alt.xml" ) )
        {
            cerr<<"Error loading haarcascade"<<endl;
            return -1;
        }


        Mat oldFrame,oldFrame_gray;
        cap >> oldFrame;
        // Mirror effect
        cv::flip(oldFrame,oldFrame,1);
        // Convert to gray
        cv::cvtColor(oldFrame,oldFrame_gray,COLOR_BGR2GRAY);

        Mat modelFace;

        while (waitKey(5)<0)
        {
            Mat frame,frame_gray;
            std::vector<Rect> faces;
            // Get frame
            cap >> frame;
            // Mirror effect
            cv::flip(frame,frame,1);
            // Convert to gray
            cv::cvtColor(frame,frame_gray,COLOR_BGR2GRAY);
            // Equalize graylevels
            equalizeHist( frame_gray, frame_gray );



            // Create the matchTemplate image result
            Mat resultImage;    // to store the matchTemplate result
            int result_cols =  frame.cols-templateWidth  + 1;
            int result_rows = frame.rows-templateHeight + 1;
            resultImage.create( result_cols, result_rows, CV_32FC1 );
            //-- Detect faces
            face_cascade.detectMultiScale( frame_gray, faces, 1.1, 4, 0,Size(60, 60) );

            if (faces.size()>0)
            {

                // Draw green rectangle
                for (int i=0;i<(int)faces.size();i++) {
                    Rect workingRect=faces[i];
                    Rect templateRect(workingRect.x+(workingRect.width-templateWidth)/2,workingRect.y+(workingRect.height-templateHeight)/2,templateWidth,templateHeight);
                    Point workingCenter(workingRect.x+workingRect.width/2,workingRect.y+workingRect.height/2);
                    if(modelFace.empty()){
                        modelFace=Mat(oldFrame_gray,templateRect);
                    }
                    // Extract template image in oldFrame
                    // Do the Matching between the working rect in frame2 and the templateImage in frame1
                    matchTemplate( frame_gray, modelFace, resultImage, TM_CCORR_NORMED );
                    // Localize the best match with minMaxLoc
                    double minVal; double maxVal; Point minLoc; Point maxLoc;
                    minMaxLoc( resultImage, &minVal, &maxVal, &minLoc, &maxLoc);
                    // Compute the translation vector between the origin and the matching rect
                    Point vect(maxLoc.x-templateRect.x,maxLoc.y-templateRect.y);
                    // Draw green rectangle and the translation vector
                    rectangle(frame,workingRect,Scalar( 0, 255, 0),2);
                    rectangle(frame,templateRect,Scalar( 255, 0, 0),2);
                    Point p(workingCenter.x+vect.x,workingCenter.y+vect.y);
                    arrowedLine(frame,workingCenter,p,Scalar(255,255,255),2);

                    if(abs(vect.x)<30 && abs(vect.y)<30){
                        // Cas d'un mouvement léger
                        if(vect.x>4){direction="droite";}
                        else if(vect.x<-4){direction="gauche";}
                        else if(vect.y>4){direction="bas";}
                        else if (vect.y<-4){direction="haut";}
                        else {direction="null";}
                        std::cout<<direction<<endl;
                        // Redefinition du model ( à revoir)
                        Rect newModel(p.x-templateWidth/2,p.y-templateHeight/2,templateWidth,templateHeight);
                        modelFace=Mat(frame_gray,newModel);
                        // Dessin du rectangle
                        rectangle(frame,newModel,Scalar(255,255,0),2);
                    }
                    // Swap matrixes
                    swap(oldFrame_gray,frame_gray);
                }
            }
            // Display frame
             imshow("WebCam", frame);
            //window->updateVisage(frame);
        }
        // the camera will be deinitialized automatically in VideoCapture destructor
        return 0;
}
