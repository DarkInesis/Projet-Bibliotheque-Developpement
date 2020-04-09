#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <iostream>
#include "webcam.h"
#include "gamewidget.h"
namespace Ui {
class Window;
}
/*
 * Classe principale d'affichage
 * Fais le lien entre ses composants graphiques (boutons/gamewidget) et la webcam
*/
class Window : public QWidget {
    Q_OBJECT

   public:
    explicit Window(QWidget* parent = nullptr);
    ~Window();

   private:
    Ui::Window* ui;
    Webcam* myWebCam;

   private slots:
    // slot appele lorsqu'une image a ete capturee : affiche l'image de la webcam
    void update(cv::Mat*);
    // slot appele lors du click du bouton "initialiser webcam" : demande l'initialisation de la webcam
    void on_buttonInitWebCam_clicked();
    //
    void updateStateInitialisationButton(bool);
    // Slot appele lorsqu'on appui sur le bouton "recommencer" : cache le menu
    void on_restart_button_2_clicked();
    // Slot appele a la fin de la partie. Montre le menu (le QString correspond à la duree du jeu)
    void enableMenu(QString);
};

#endif  // WINDOW_H
