#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include "labyrinthe.h"
#include "ball.h"
#include "webcam.h"
#include <QGLWidget>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QTimer>
#include <QElapsedTimer>

// Classe dediee a l'affichage d'une scene OpenGL
class GameWidget : public QGLWidget {
    Q_OBJECT

   public:
    // Booléen qui indique si le joueur peut bouger (est à faux lorsqu'il a gagné)
    bool canMove=true;
    // Booléen qui indique si il faut dessiner ou non le labyrinthe 2D
    bool isNeededToPaint2DLabyrinthe = true;

    // Moving directions
    bool moveForward = false, moveBackward = false, moveLeft = false, moveRight = false;

    // Game variable
    bool ball_found = false;
    // Position de départ (centre de la première case)
    float x_position = SZ / 2;
    float y_position = -SZ / 2;
    float z_position = 7;
    float angle_view_x = -90;
    bool head_move = true;

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

    void userMove();
    void detectCollision(float, float);
    void checkBallFound();
    void checkUserWin();

   protected:
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

   private:
    // Timer d'animation
    float m_TimeElapsed{0.0f};
    QTimer m_AnimationTimer;
    bool game_started = false;
    QElapsedTimer chrono;
    int lastTimeMove = 3000;
    int timeWin = 0;
    Ball *ball;
signals :
    void gameFinished(QString);
   private slots:
    void updateDirection(Webcam::Move);
    void restartGame();
};

#endif  // GAMEWIDGET_H
