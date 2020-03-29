#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include "labyrinthe.h"
#include "ball.h"
#include <QGLWidget>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QTimer>

// Classe dediee a l'affichage d'une scene OpenGL
class GameWidget : public QGLWidget {
      Q_OBJECT

     public:
      // Constructeur
      GameWidget(QWidget *parent = nullptr);

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
      Labyrinthe *labyrinthe;
      Ball *ball;
};

#endif  // GAMEWIDGET_H
