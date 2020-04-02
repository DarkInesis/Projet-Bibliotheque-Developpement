#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include "labyrinthe.h"
#include "ball.h"
#include <QGLWidget>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QTimer>
#include "usercontrol.h"

// Classe dediee a l'affichage d'une scene OpenGL
class GameWidget : public QGLWidget {
      Q_OBJECT

     public:
        UserControl* userControler;
      // Moving directions
      bool moveForward = false, moveBackward = false, moveLeft = false, moveRight = false;

      // Game variable
      bool ball_found = false;
      // Position de départ (centre de la première case)
      float x_position = SZ / 2;
      float y_position = -SZ / 2;
      float z_position = 7;
      float angle_view_x = -90;

      Labyrinthe *labyrinthe;
      int maze_width_, maze_height_;

      // Constructeur
      GameWidget(QWidget *parent = nullptr, int width = 10, int height = 6);

     protected:
      // Fonction d'initialisation
      void initializeGL();

      // Fonction de redimensionnement
      void resizeGL(int width, int height);

      // Fonction d'affichage
      void paintGL();

      // Fonction de gestion d'interactions clavier
      void keyPressEvent(QKeyEvent *event);
      void keyReleaseEvent(QKeyEvent *event);

      void userMove();
      void detectCollision(float, float);
      void checkBallFound();
      void checkUserWin();

     private:
      // Timer d'animation
      float m_TimeElapsed{0.0f};
      QTimer m_AnimationTimer;
      Ball *ball;


};

#endif  // GAMEWIDGET_H