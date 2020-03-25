#include "wall.h"
#include "draw_functions.h"
#include <iostream>
#include <QGLWidget>

Wall::Wall(int width, int height) {

  // Initialisation des paramètres
  // Le labyrinthe à dessiner est généré ici
  maze = Maze(width, height);
  maze.generate();

  // Temporaire : on affiche dans la console pour vérifier que
  // le labyrinthe est bien dessiné
  std::cout << "Affichage" << endl;
  maze.display();
}

Wall::~Wall() {

}

void Wall::Display() const {

  glPushMatrix(); // On stocke le repère d'origine

  GLfloat color_floor[]={1,1,1,1};
  GLfloat color_cap[]={0,0,0,1};
  GLfloat color_Wall[]={0.5,0.5,0.5,1};
  int i, j;
  // Dessin du sol et plafond du labyrinthe
  glBegin(GL_QUADS);

  // Couleur matériaux sol
  glMaterialfv(GL_FRONT,GL_AMBIENT,color_floor);
  glColor3ub(150, 150, 150);
  // Sol
  glVertex3f(0, 0, 0);
  glVertex3f(coeff_move * maze.width_, 0, 0);
  glVertex3f(coeff_move * maze.width_, -coeff_move * maze.height_, 0);
  glVertex3f(0, -coeff_move * maze.height_, 0);

  // Couleur matériaux plafond
  glMaterialfv(GL_FRONT,GL_AMBIENT,color_cap);
  glColor3ub(150, 150, 150);
  // Plafond
  glVertex3f(0, 0, 0.6 * SZ);
  glVertex3f(coeff_move * maze.width_, 0, 0.6 * SZ);
  glVertex3f(coeff_move * maze.width_, -coeff_move * maze.height_, 0.6 * SZ);
  glVertex3f(0, -coeff_move * maze.height_, 0.6 * SZ);
  glEnd();

  // Couleur matériaux murs
  glMaterialfv(GL_FRONT,GL_AMBIENT,color_Wall);
  glColor3ub(150, 150, 150);
  // On se déplace au centre de la première case
  glTranslatef(SZ / 2, -SZ / 2, 0);

  // Dessin du mur du haut du labyrinthe
  // On monte sur la case du haut pour dessiner les murs côté Nord avec la
  // fonction DrawSouthWall()
  glTranslatef(0, coeff_move, 0);

  for (j = 0; j < maze.width_; j++) {
    DrawSouthWall(SZ);
    // On se déplace sur la case à droite
    glTranslatef(coeff_move, 0, 0);
  }
  // On revient à la ligne
  glTranslatef(-coeff_move * maze.width_, -coeff_move, 0);

  // Print other lines
  for (i = 0; i < maze.height_; i++) {
    // Beginning of line
    // On se déplace sur la case de gauche pour dessiner le mur de gauche avec
    // la fonction DrawEastWall
    glTranslatef(-coeff_move, 0, 0);
    DrawEastWall(SZ);
    glTranslatef(coeff_move, 0, 0);

    // Print cells
    for (j = 0; j < maze.width_; j++) {
      if (maze.grid_[i][j].isFrontier(Cell::E))
        DrawEastWall(SZ);
      if (maze.grid_[i][j].isFrontier(Cell::S))
        DrawSouthWall(SZ);
      // On passe à la case d'après
      glTranslatef(coeff_move, 0, 0);
    }
    // On revient à la ligne
    glTranslatef(-coeff_move * maze.width_, -coeff_move, 0);
  }

  glPopMatrix();
}
