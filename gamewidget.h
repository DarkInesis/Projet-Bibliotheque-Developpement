/** Author : F. Zdrada & L. Dedier **/

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
    bool canMove = true;
    // Moving directions
    bool moveForward = false, moveBackward = false, moveLeft = false, moveRight = false;

    // Game variable
    bool ball_found = false;
    // Position de départ (centre de la première case)
    float x_position = SZ / 2;
    float y_position = -SZ / 2;
    float z_position = 7;
    float angle_view_x = 0;
    bool head_move = true;
    // Labyrinthe et ses variables de construction.
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
    // Methode qui manage le mouvement du joueur (calcul de la nouvelle position/test si
    // collision/test si la balle est attrapée/...)
    void userMove();
    // On teste si le mouvement est possible ( les deux arguments correspondent aux futures
    // coordonnées du joueur)
    // Si il n'y a pas de collision, on applique le mouvement.
    void detectCollision(float, float);
    // TODO
    void checkBallFound();
    // Verifie si le joueur a gagné la partie à la suite de son mouvement. Emet le signal
    // GameFinished si c'est le cas.
    void checkUserWin();

   private:
    // Timer d'animation
    float m_TimeElapsed{0.0f};
    QTimer m_AnimationTimer;
    // Variables de gestion du temps
    bool game_started = false;
    QElapsedTimer chrono;
    int lastTimeMove = 3000;
    int timeWin = 0;
    // Pointeur vers la Ball (objectif à récuperer)
    Ball *ball;
    // Tableau contenant les tailles prédéfinies que peut prendre le labyrinthe
    int labyrintheSizeOptionsWidth[5] = {5, 10, 10, 15, 15};
    int labyrintheSizeOptionsHeight[5] = {5, 6, 10, 10, 15};
   signals:
    // Signal emis lorsque le jeu a été gagné.
    // Renvoi le QString correspondant au temps mis par le joueur pour finir
    void gameFinished(QString);
   private slots:
    // Lorsque la webcam indique qu'il y a eu un mouvement, on modifie les champs responsables de la
    // direction
    void updateDirection(Webcam::Move);
    // Lorsque l'on clique sur recommencer, cette méthode est appelée.
    // Réinitialise le jeu
    void restartGame();
    // Lorsque le joueur modifie la taille du labyrinthe, on modifie les paramètres de tailles du
    // labyrinthe et on recommence une partie
    void updateSize(int);
};

#endif  // GAMEWIDGET_H
