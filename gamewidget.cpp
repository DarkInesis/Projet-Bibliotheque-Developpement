#define _USE_MATH_DEFINES

#include "gamewidget.h"
#include "labyrinthe.h"
#include <iostream>
#include <QApplication>
#include <QDesktopWidget>
#include <QRandomGenerator>
#include <cmath>
#include <random>

// Declarations des constantes
const unsigned int WIN_WIDTH = 1150;
const unsigned int WIN_HEIGHT = 850;
const float MAX_DIMENSION = 50.0f;
const float PI = 3.14159265359;
const int nbMSecAvantMap2D = 3000;
const int intervalTimer = 10;

// Convert time in millisecond to mm::ss::mm
QString timeToString(int time) {
    int m = time / 60000;
    QString min = QString("%1").arg(m, 2, 10, QChar('0'));
    time = time - 60000 * m;

    int s = time / 1000;
    QString sec = QString("%1").arg(s, 2, 10, QChar('0'));
    time = time - 1000 * s;

    QString msec = QString("%1").arg(time / 10, 2, 10, QChar('0'));

    return min + ":" + sec + ":" + msec;
}

// Constructeur
GameWidget::GameWidget(QWidget *parent, int width, int height) : QGLWidget(parent) {
    // Reglage de la taille/position
    setFixedSize(WIN_WIDTH, WIN_HEIGHT);
    move(QApplication::desktop()->screen()->rect().center() - rect().center());

    // Connexion du timer d'animation
    connect(&m_AnimationTimer, &QTimer::timeout, [&] {
        m_TimeElapsed += 1.0f / 5.0f;
        updateGL();
    });

    maze_width_ = width;
    maze_height_ = height;

    m_AnimationTimer.setInterval(10);
    m_AnimationTimer.start();
}

// Fonction d'initialisation
void GameWidget::initializeGL() {
    // Reglage de la couleur de fond
    glClearColor(0.9, 0.9, 0.9, 1.0);

    glEnable(GL_DEPTH_TEST);
    // Activation du zbuffer et lumières
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    labyrinthe = new Labyrinthe(maze_width_, maze_height_);

    // Ball position
    ball = new Ball(2, 2);
}

// Fonction de redimensionnement
void GameWidget::resizeGL(int width, int height) {
    // Definition du viewport (zone d'affichage)
    glViewport(0, 0, width, height);

    if (width != 0)
        glOrtho(-MAX_DIMENSION, MAX_DIMENSION, -MAX_DIMENSION * height / static_cast<float>(width),
                MAX_DIMENSION * height / static_cast<float>(width), -MAX_DIMENSION * 2.0f,
                MAX_DIMENSION * 2.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// Fonction d'affichage
void GameWidget::paintGL() {
    // Reinitialisation des tampons
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    if (canMove) {
        userMove();
    }
    // Definition de la position de la camera
    gluLookAt(x_position, y_position, z_position, x_position + cos(angle_view_x * PI / 180),
              y_position + sin(angle_view_x * PI / 180), z_position, 0, 0, 1);

    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();
    gluPerspective(85, (double)WIN_WIDTH / WIN_HEIGHT, 1,
                   1000);  // Définir les paramètres pour notre projection perspective

    glMatrixMode(GL_MODELVIEW);

    labyrinthe->Display();
    if (!ball_found) ball->Display();

    //** Ajout du chronomètre en haut à droite **//
    long int currentTime = chrono.elapsed();
    glColor3f(1.0, 1.0, 1.0);
    QFont serifFont("Consolas", 20, QFont::Light);
    if (game_started)
        QGLWidget::renderText(WIN_WIDTH - 200, 100, timeToString(currentTime), serifFont);
    else
        QGLWidget::renderText(WIN_WIDTH - 200, 100, timeToString(timeWin), serifFont);

    // Si on s'est déplacé la dernière fois il y a plus de 3 secondes, on affiche la carte
    if (currentTime - lastTimeMove >= 3000) {
        //*** 2D Part ***//
        // Set the 2D environment
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();

        //** Ajout de la carte du labyrinthe **//
        // Calcul pour avoir les bonnes proportions du repère ortho2D vu que le widget n'est pas
        // carré
        float proportions = float(WIN_WIDTH) / WIN_HEIGHT;
        // On multiplie par 4 pour que la largueur du labyrinthe fasse 1/4 de la largueur du widget
        // On fait -1 pour avoir une marge de 1 entre le plan et le bord du widget
        gluOrtho2D(-1, (4 * labyrinthe->maze.width_ - 1),
                   (4 * labyrinthe->maze.height_ - 1) * proportions, -1);

        // On calcule x et y en "pourcentage de position" dans le labyrinthe
        float x = x_position / (labyrinthe->maze.width_ * coeff_move + SZ / 4);
        float y = -y_position / (labyrinthe->maze.height_ * coeff_move + SZ / 4);

        labyrinthe->Show2DMap(x, y, angle_view_x);

        // Fin du dessin, réinitialisation des matrices
        glEnable(GL_LIGHTING);
        glEnable(GL_DEPTH_TEST);
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();
        //** End 2D **//
    }
}

void GameWidget::userMove() {
    // Calcul de la position de la caméra
    if (moveRight) {
        angle_view_x -= 0.6;
    } else if (moveLeft) {
        angle_view_x += 0.6;
    }

    // Position du joueur
    float new_x_position = x_position;
    float new_y_position = y_position;

    if (moveForward) {
        new_x_position += 0.1 * cos(angle_view_x * PI / 180);
        new_y_position += 0.1 * sin(angle_view_x * PI / 180);
    } else if (moveBackward) {
        new_x_position -= 0.1 * cos(angle_view_x * PI / 180);
        new_y_position -= 0.1 * sin(angle_view_x * PI / 180);
    }

    if (moveForward || moveBackward) {
        if (head_move) {
            z_position += 0.015;
            if (z_position >= 7.2) head_move = false;
        } else {
            z_position -= 0.02;
            if (z_position <= 6.8) head_move = true;
        }
        if (!game_started) {
            chrono.start();
            game_started = true;
        }
        lastTimeMove = chrono.elapsed();
    }

    // Detecte les collisions avec les murs
    detectCollision(new_x_position, new_y_position);

    if (!ball_found)
        checkBallFound();
    else
        checkUserWin();
}

void GameWidget::detectCollision(float new_x_position, float new_y_position) {
    // On cherche la case dans laquelle le joueur se trouve s'il avance
    int case_x = (new_x_position - SZ / 8) / coeff_move;
    int case_y = -(new_y_position + SZ / 8) / coeff_move;

    // On récupère la cellule du labyrinthe où se trouve le joueur
    Cell here = labyrinthe->maze.grid_[case_y][case_x];
    float center_x = coeff_move * case_x + SZ / 2;
    float center_y = -coeff_move * case_y - SZ / 2;

    bool x_ok = true, y_ok = true;

    // On regarde si on avance dans une zone de collision (de 1 à 8)
    // puis on vérifie s'il y a un mur ou non
    //
    //      +---+---+---+---+
    //      | 1 |   2   | 3 |
    //      +---+---+---+---+
    //      |   |       |   |
    //      + 8 +   0   + 4 +
    //      |   |       |   |
    //      +---+---+---+---+
    //      | 7 |   6   | 5 |
    //      +---+---+---+---+
    //

    if (x_position < new_x_position) {  // On va vers la droite
        // Si on est dans la zone 4
        if (new_x_position > center_x + SZ / 4 - SZ * 0.11) {
            if (here.isFrontier(Cell::E))  // Si il y a un mur
                x_ok = false;
            else {  // Sinon, si on est dans la zone 3 ou 5
                if (new_y_position > center_y + SZ / 4 - SZ * 0.1) {  // Zone 3
                    if (labyrinthe->maze.grid_[case_y][case_x + 1].isFrontier(Cell::N) ||
                        labyrinthe->maze.grid_[case_y - 1][case_x].isFrontier(Cell::E))
                        x_ok = false;
                } else if (new_y_position < center_y - SZ / 4 + SZ * 0.1) {  // Zone 5
                    if (labyrinthe->maze.grid_[case_y][case_x + 1].isFrontier(Cell::S) ||
                        labyrinthe->maze.grid_[case_y + 1][case_x].isFrontier(Cell::E))
                        x_ok = false;
                }
            }
        }
    } else if (x_position > new_x_position) {  // Vers la gauche
        // Si on est dans la zone 8
        if (new_x_position < center_x - SZ / 4 + SZ * 0.11) {
            if (here.isFrontier(Cell::W))
                x_ok = false;
            else {  // Sinon, si on est dans la zone 1 ou 7
                if (new_y_position > center_y + SZ / 4 - SZ * 0.1) {  // Zone 1
                    if (labyrinthe->maze.grid_[case_y][case_x - 1].isFrontier(Cell::N) ||
                        labyrinthe->maze.grid_[case_y - 1][case_x].isFrontier(Cell::W))
                        x_ok = false;
                } else if (new_y_position < center_y - SZ / 4 + SZ * 0.1) {  // Zone 7
                    if (labyrinthe->maze.grid_[case_y][case_x - 1].isFrontier(Cell::S) ||
                        labyrinthe->maze.grid_[case_y + 1][case_x].isFrontier(Cell::W))
                        x_ok = false;
                }
            }
        }
    }

    if (y_position < new_y_position) {  // On va vers le haut
        // Si on est dans la zone 2
        if (new_y_position > center_y + SZ / 4 - SZ * 0.11) {
            if (here.isFrontier(Cell::N))  // Si il y a un mur
                y_ok = false;
            else {  // Sinon, si on est dans la zone 1 ou 3
                if (new_x_position < center_x - SZ / 4 + SZ * 0.1) {  // Zone 1
                    if (labyrinthe->maze.grid_[case_y - 1][case_x].isFrontier(Cell::W) ||
                        labyrinthe->maze.grid_[case_y][case_x - 1].isFrontier(Cell::N))
                        y_ok = false;
                } else if (new_x_position > center_x + SZ / 4 - SZ * 0.1) {  // Zone 3
                    if (labyrinthe->maze.grid_[case_y - 1][case_x].isFrontier(Cell::E) ||
                        labyrinthe->maze.grid_[case_y][case_x + 1].isFrontier(Cell::N))
                        y_ok = false;
                }
            }
        }
    } else if (y_position > new_y_position) {  // Vers le bas
        // Si on est dans la zone 6
        if (new_y_position < center_y - SZ / 4 + SZ * 0.11) {
            if (here.isFrontier(Cell::S))
                y_ok = false;
            else {  // Sinon, si on est dans la zone 5 ou 7
                if (new_x_position < center_x - SZ / 4 + SZ * 0.1) {  // Zone 7
                    if (labyrinthe->maze.grid_[case_y + 1][case_x].isFrontier(Cell::W) ||
                        labyrinthe->maze.grid_[case_y][case_x - 1].isFrontier(Cell::S))
                        y_ok = false;
                } else if (new_x_position > center_x + SZ / 4 - SZ * 0.1) {  // Zone 5
                    if (labyrinthe->maze.grid_[case_y + 1][case_x].isFrontier(Cell::E) ||
                        labyrinthe->maze.grid_[case_y][case_x + 1].isFrontier(Cell::S))
                        y_ok = false;
                }
            }
        }
    }

    if (x_ok) x_position = new_x_position;
    if (y_ok) y_position = new_y_position;
}

void GameWidget::checkBallFound() {
    float ball_x, ball_y;
    tie(ball_x, ball_y) = ball->getBallPosition();

    if (x_position < ball_x + 2 && x_position > ball_x - 2)
        if (y_position < ball_y + 2 && y_position > ball_y - 2) {
            ball_found = true;
            labyrinthe->OpenExit();
        }
}

void GameWidget::checkUserWin() {
    if (x_position < SZ / 4 || x_position > coeff_move * labyrinthe->maze.width_ ||
        y_position > -SZ / 4 || y_position < -coeff_move * labyrinthe->maze.height_) {
        canMove = false;
        game_started = false;
        ball_found = false;
        timeWin = chrono.elapsed();
        emit gameFinished(timeToString(timeWin));
    }
}

void GameWidget::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Up) moveForward = true;
    if (event->key() == Qt::Key_Down) moveBackward = true;
    if (event->key() == Qt::Key_Left) moveLeft = true;
    if (event->key() == Qt::Key_Right) moveRight = true;
}

void GameWidget::keyReleaseEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Up) moveForward = false;
    if (event->key() == Qt::Key_Down) moveBackward = false;
    if (event->key() == Qt::Key_Left) moveLeft = false;
    if (event->key() == Qt::Key_Right) moveRight = false;
}

// Slots
void GameWidget::updateDirection(Webcam::Move d) {
    moveRight = false;
    moveLeft = false;
    moveForward = false;
    moveBackward = false;
    if (d == Webcam::DROITE) {
        moveRight = true;
    } else if (d == Webcam::GAUCHE) {
        moveLeft = true;
    } else if (d == Webcam::AVANT) {
        moveForward = true;
    } else if (d == Webcam::ARRIERE) {
        moveBackward = true;
    }
}

void GameWidget::restartGame() {
    canMove = true;
    timeWin = 0;
    // Réinitialisation du point de départ
    x_position = SZ / 2;
    y_position = -SZ / 2;
    angle_view_x = -90;

    // Load new labyrinthe
    initializeGL();
    m_AnimationTimer.start();
}
