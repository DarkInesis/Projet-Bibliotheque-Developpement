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
/*
 * La classe Webcam hérite de la classe QThread, ce qui permet de faire les acquisitions d'image ainsi que leur traitement
 * dans un autre thread (ce qui permet de garantir la fluidité du jeu)
*/
class Webcam : public QThread {
    Q_OBJECT
   public:
    // Constructeur
    Webcam();
    // Calcul la distance entre la tête et le point de référence
    void detectMotion();
    // Reinitialise la face
    void detectFace();
    // A partir de la distance calculee, determine la direction de l'utilisateur (emet le signal directionChanged)
    void getDirection();
    // Reset la valeur de la face initialisee, et modifie les differents champs pour qu'à la prochaine capture,
    // on initialise la face.
    void resetInitFace();
    // Modifie le boolean isUpdate, pour indiquer qu'il y a besoin de reinitialiser la face
    void needUpdate() { isUpdated = false; };

    enum Move { AVANT, ARRIERE, DROITE, GAUCHE, NEUTRE };

   protected:
    // methode faisant une boucle infinie et dont la fonction est de manager l'aquisition et le traitement des images.
    // (emet le signal webcamFrameCaptured)
    void run() override;

   private:
    // TODO
    bool isUpdated = false;
    // Objet permettant de detecter la face
    cv::CascadeClassifier face_cascade;
    // Variables de la webcam
    int frameWidth = 640;
    int frameHeight = 480;
    cv::VideoCapture cap;
    // TODO
    cv::Mat init_frame, current_frame;
    cv::Rect face;
    cv::Point p, vect, face_center;
    // Variable indiquant la direction de la personne par rapport à sa phase neutre.
    Move direction;
    // Compteurs
    int counterAbsurde = 0, counterChangeDirection = 0;

   signals:
    // Signal emis lorsque la webcam a fini l'acquisition et le traitement de l'image
    // Ce signal contient un pointeur vers l'image traitée
    void webcamFrameCaptured(cv::Mat*);
    // Signal emis lors d'un changement de direction. Permet d'indiquer au GLWidget un changement de direction
    void directionChanged(Webcam::Move);
};

#endif  // WEBCAM_H
