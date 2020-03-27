#define _USE_MATH_DEFINES

#include "myglwidget.h"
#include "wall.h"
#include <iostream>
#include <QApplication>
#include <QDesktopWidget>
#include <QRandomGenerator>
#include <cmath>
#include <random>

// Declarations des constantes
const unsigned int WIN_WIDTH = 300;
const unsigned int WIN_HEIGHT =400;
const float MAX_DIMENSION = 50.0f;
const float PI = 3.14159265359;

// Position de départ (centre de la première case)
float x_position = SZ / 2;
float y_position = -SZ / 2;
float z_position = 7;
float angle_view_x = -90;

// Moving directions
bool moveForward = false, moveBackward = false, moveLeft = true,
     moveRight = false;

// Constructeur
MyGLWidget::MyGLWidget(QWidget *parent) : QOpenGLWidget(parent) {
  // Reglage de la taille/position
  setFixedSize(WIN_WIDTH, WIN_HEIGHT);
  move(QApplication::desktop()->screen()->rect().center() - rect().center());

  // Connexion du timer d'animation
  connect(&m_AnimationTimer, &QTimer::timeout, [&] {
    m_TimeElapsed += 1.0f / 5.0f;
    //updateGL();
    update();
  });

  m_AnimationTimer.setInterval(10);
  m_AnimationTimer.start();
}

// Fonction d'initialisation
void MyGLWidget::initializeGL() {
  // Reglage de la couleur de fond
  glClearColor(0.1f, 0.1f, 0.1f, 0.0f);

  // Activation du zbuffer
  glEnable(GL_DEPTH_TEST);

  // Activation des lumieres
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  // Initialisation de la lumiere du soleil
  // ---- Positionnement en mode spot----
  GLfloat light_tab[]={0,0,0,1};
  glLightfv(GL_LIGHT0, GL_POSITION,light_tab);
  // ----- Changement de la couleur ----
  GLfloat color_ambiant[]={1,1,1,1};
  glLightfv(GL_LIGHT0,GL_AMBIENT,color_ambiant);
}

// Fonction de redimensionnement
void MyGLWidget::resizeGL(int width, int height) {
  // Definition du viewport (zone d'affichage)
  glViewport(0, 0, width, height);

  if (width != 0)
    glOrtho(-MAX_DIMENSION, MAX_DIMENSION,
            -MAX_DIMENSION * height / static_cast<float>(width),
            MAX_DIMENSION * height / static_cast<float>(width),
            -MAX_DIMENSION * 2.0f, MAX_DIMENSION * 2.0f);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

// Fonction d'affichage
void MyGLWidget::paintGL() {

  // Reinitialisation des tampons
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
  glLoadIdentity();

  userMove();

  // Definition de la position de la camera
  gluLookAt(x_position, y_position, z_position,
            x_position + cos(angle_view_x * PI / 180),
            y_position + sin(angle_view_x * PI / 180), z_position, 0, 0, 1);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(
      80, (double)1000 / 900, 1,
      1000); // Définir les paramètres pour notre projection perspective

  glMatrixMode(GL_MODELVIEW);

  wall.Display();
}

// Fonction de gestion d'interactions clavier
void MyGLWidget::keyPressEvent(QKeyEvent *event) {

  switch (event->key()) {

  case Qt::Key_Right: {
    moveRight = true;
    break;
  }
  case Qt::Key_Left: {
    moveLeft = true;
    break;
  }
  case Qt::Key_Up: {
    moveForward = true;
    break;
  }
  case Qt::Key_Down: {
    moveBackward = true;
    break;
  }

  // Sortie de l'application
  case Qt::Key_Escape: {
    exit(0);
  }

  // Cas par defaut
  default: {
    // Ignorer l'evenement
    event->ignore();
    return;
  }
  }

  // Acceptation de l'evenement et mise a jour de la scene
  event->accept();
  //updateGL();
  update();
}

void MyGLWidget::keyReleaseEvent(QKeyEvent *event) {
  switch (event->key()) {

  case Qt::Key_Right: {
    moveRight = false;
    break;
  }
  case Qt::Key_Left: {
    moveLeft = false;
    break;
  }
  case Qt::Key_Up: {
    moveForward = false;
    break;
  }
  case Qt::Key_Down: {
    moveBackward = false;
    break;
  }
  // Cas par defaut
  default: {
    // Ignorer l'evenement
    event->ignore();
    return;
  }
  }
}

void MyGLWidget::userMove() {

  // Calcul de la position de la caméra
  if (moveRight) {
    angle_view_x -= 1;
  } else if (moveLeft) {
    angle_view_x += 1;
  }

  // Position du joueur
  float new_x_position = x_position;
  float new_y_position = y_position;

  if (moveForward) {
    new_x_position += 0.2 * cos(angle_view_x * PI / 180);
    new_y_position += 0.2 * sin(angle_view_x * PI / 180);
  } else if (moveBackward) {
    new_x_position -= 0.2 * cos(angle_view_x * PI / 180);
    new_y_position -= 0.2 * sin(angle_view_x * PI / 180);
  }

  // Detecte les collisions avec les murs
  // On cherche la case dans laquelle le joueur se trouve s'il avance
  int case_x = (new_x_position - SZ / 8) / coeff_move;
  int case_y = -(new_y_position + SZ / 8) / coeff_move;

  // On récupère la cellule du labyrinthe où se trouve le joueur
  Cell here = wall.maze.grid_[case_y][case_x];
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

  if (x_position < new_x_position) { // On va vers la droite
    // Si on est dans la zone 4
    if (new_x_position > center_x + SZ / 4 - SZ * 0.11) {
      if (here.isFrontier(Cell::E)) // Si il y a un mur
        x_ok = false;
      else { // Sinon, si on est dans la zone 3 ou 5
        if (new_y_position > center_y + SZ / 4 - SZ * 0.1) { // Zone 3
          if (wall.maze.grid_[case_y][case_x + 1].isFrontier(Cell::N) ||
              wall.maze.grid_[case_y - 1][case_x].isFrontier(Cell::E))
            x_ok = false;
        } else if (new_y_position < center_y - SZ / 4 + SZ * 0.1) { // Zone 5
          if (wall.maze.grid_[case_y][case_x + 1].isFrontier(Cell::S) ||
              wall.maze.grid_[case_y + 1][case_x].isFrontier(Cell::E))
            x_ok = false;
        }
      }
    }
  } else if (x_position > new_x_position) { // Vers la gauche
    // Si on est dans la zone 8
    if (new_x_position < center_x - SZ / 4 + SZ * 0.11) {
      if (here.isFrontier(Cell::W))
        x_ok = false;
      else { // Sinon, si on est dans la zone 1 ou 7
        if (new_y_position > center_y + SZ / 4 - SZ * 0.1) { // Zone 1
          if (wall.maze.grid_[case_y][case_x - 1].isFrontier(Cell::N) ||
              wall.maze.grid_[case_y - 1][case_x].isFrontier(Cell::W))
            x_ok = false;
        } else if (new_y_position < center_y - SZ / 4 + SZ * 0.1) { // Zone 7
          if (wall.maze.grid_[case_y][case_x - 1].isFrontier(Cell::S) ||
              wall.maze.grid_[case_y + 1][case_x].isFrontier(Cell::W))
            x_ok = false;
        }
      }
    }
  }

  if (y_position < new_y_position) { // On va vers le haut
    // Si on est dans la zone 2
    if (new_y_position > center_y + SZ / 4 - SZ * 0.11) {
      if (here.isFrontier(Cell::N)) // Si il y a un mur
        y_ok = false;
      else { // Sinon, si on est dans la zone 1 ou 3
        if (new_x_position < center_x - SZ / 4 + SZ * 0.1) { // Zone 1
          if (wall.maze.grid_[case_y - 1][case_x].isFrontier(Cell::W) ||
              wall.maze.grid_[case_y][case_x - 1].isFrontier(Cell::N))
            y_ok = false;
        } else if (new_x_position > center_x + SZ / 4 - SZ * 0.1) { // Zone 3
          if (wall.maze.grid_[case_y - 1][case_x].isFrontier(Cell::E) ||
              wall.maze.grid_[case_y][case_x + 1].isFrontier(Cell::N))
            y_ok = false;
        }
      }
    }
  } else if (y_position > new_y_position) { // Vers le bas
    // Si on est dans la zone 6
    if (new_y_position < center_y - SZ / 4 + SZ * 0.11) {
      if (here.isFrontier(Cell::S))
        y_ok = false;
      else { // Sinon, si on est dans la zone 5 ou 7
        if (new_x_position < center_x - SZ / 4 + SZ * 0.1) { // Zone 7
          if (wall.maze.grid_[case_y + 1][case_x].isFrontier(Cell::W) ||
              wall.maze.grid_[case_y][case_x - 1].isFrontier(Cell::S))
            y_ok = false;
        } else if (new_x_position > center_x + SZ / 4 - SZ * 0.1) { // Zone 5
          if (wall.maze.grid_[case_y + 1][case_x].isFrontier(Cell::E) ||
              wall.maze.grid_[case_y][case_x + 1].isFrontier(Cell::S))
            y_ok = false;
        }
      }
    }
  }

  if (x_ok)
    x_position = new_x_position;
  if (y_ok)
    y_position = new_y_position;
}
