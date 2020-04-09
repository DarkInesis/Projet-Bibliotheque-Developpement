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
 * La classe Webcam hérite de la classe QThread, ce qui permet de faire les acquisitions d'image
 * ainsi que leur traitement
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
    // A partir de la distance calculée, determine la direction de l'utilisateur (emet le signal
    // directionChanged)
    void getDirection();
    // Reset la valeur de la face initialisée, et modifie les differents champs pour qu'à la
    // prochaine capture,
    // On initialise la face.
    void resetInitFace();
    // Modifie le boolean isUpdate, pour indiquer qu'il y a besoin de reinitialiser la face
    void needUpdate() { isUpdated = false; };

    enum Move { AVANT, ARRIERE, DROITE, GAUCHE, NEUTRE };

   protected:
    // Méthode faisant une boucle infinie et dont la fonction est de manager l'acquisition et le
    // traitement des images.
    // (emet le signal webcamFrameCaptured)
    void run() override;

   private:
    // Booléen indiquant si la position neutre est à jour
    bool isUpdated = false;
    // Objet permettant de detecter la face
    cv::CascadeClassifier face_cascade;
    // Variables de la webcam
    int frameWidth = 640;
    int frameHeight = 480;
    cv::VideoCapture cap;
    // Matrice de la position neutre, et de la position actuelle
    cv::Mat init_frame, current_frame;
    // Rectangle indiquant la position du visage neutre
    cv::Rect face;
    // p : centre de la position actuelle, face_center : centre de la position neutre,
    // vect : vecteur entre ces deux points (de face_center vers p)
    cv::Point p, face_center, vect;
    // Variable indiquant la direction de la personne par rapport à sa phase neutre.
    Move direction;
    // Compteurs
    int counterAbsurde = 0, counterChangeDirection = 0;

   signals:
    // Signal emis lorsque la webcam a fini l'acquisition et le traitement de l'image
    // Ce signal contient un pointeur vers l'image traitée
    void webcamFrameCaptured(cv::Mat*);
    // Signal emis lors d'un changement de direction. Permet d'indiquer au GLWidget un changement de
    // direction
    void directionChanged(Webcam::Move);
};

#endif  // WEBCAM_H
