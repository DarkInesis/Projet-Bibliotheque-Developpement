#ifndef DRAW_FUNCTIONS_H
#define DRAW_FUNCTIONS_H

#include <QGLWidget>

// rouge = {227, 14, 53};
// orange = {227, 110, 14};
// bleu = {14, 67, 227};
// vert = {13, 181, 38};
// blanc = {240, 240, 240};
// jaune = {230, 226, 14};

// Ici, nous avons le carré ABCD (de côté de longueur SZ) qui représente une
// case/cellule.
// Lors de la fonction Wall::Display(), on se déplace uniquement sur les centres
// des cases (0), ainsi on ne dessine les murs que par rapport à cette origine
// 0.
//
// On voit bien que l'épaisseur d'un mur est de SZ/4.
// La taille des murs est défini dans les fonctions ci-dessous.
//
//      A---+---+---+---B---+---+---+---+---+---+
//      |                                       |
//      +   +---+---+   +---+---+   +---+---+   +
//      |   |       |   |       |   |       |   |
//      +   +   0   +   +   0   +   +   0   +   +
//      |   |       |   |       |   |       |   |
//      +   +---+---+   +---+---+   +---+---+   +
//      |                                       |
//      D   +---+---+   C---+---+   +---+---+   +
//      |   |       |   |       |   |       |   |
//      +   +   0   +   +   0   +   +   0   +   +
//      |   |       |   |       |   |       |   |
//      +   +---+---+   +---+---+   +---+---+   +
//      |                                       |
//      +   +---+---+   +---+---+   +---+---+   +
//      |   |       |   |       |   |       |   |
//      +   +   0   +   +   0   +   +   0   +   +
//      |   |       |   |       |   |       |   |
//      +   +---+---+   +---+---+   +---+---+   +
//      |                                       |
//      +---+---+---+---+---+---+---+---+---+---+

void DrawEastWall(int SZ) {

  // Mur côté Est (rectange abdc), on défini les coordonnées des coins en
  // fonction du centre 0
  // de la case du labyrinthe. SZ correspond à la distance entre b et d
  //
  //            vert
  //      +---+---+---a---b
  //      |               |
  //  r   +   +---+---+   +   o
  //  o   |   |       |   |   r
  //  u   +   +   0   +   +   a
  //  g   |   |       |   |   n
  //  e   +   +---+---+   +   g
  //      |               |   e
  //      +---+---+---c---d
  //
  //            bleu

  float x_a = SZ / 4, y_a = SZ / 2;
  float x_b = SZ / 2, y_b = SZ / 2;
  float x_c = SZ / 4, y_c = -SZ / 2;
  float x_d = SZ / 2, y_d = -SZ / 2;

  float hauteur = 0.6 * SZ; // Hauteur du mur

  glBegin(GL_QUADS);

  // Couleur de l'objet blanc
  glColor3ub(240, 240, 240);
  // Face côté ciel
  glVertex3f(x_c, y_c, hauteur);
  glVertex3f(x_d, y_d, hauteur);
  glVertex3f(x_b, y_b, hauteur);
  glVertex3f(x_a, y_a, hauteur);

  // Couleur de l'objet bleu
  glColor3ub(14, 67, 227);
  // Face côté
  glVertex3f(x_c, y_c, 0);
  glVertex3f(x_c, y_c, hauteur);
  glVertex3f(x_d, y_d, hauteur);
  glVertex3f(x_d, y_d, 0);

  // Couleur de l'objet vert
  glColor3ub(13, 181, 38);
  // Face côté
  glVertex3f(x_a, y_a, 0);
  glVertex3f(x_a, y_a, hauteur);
  glVertex3f(x_b, y_b, hauteur);
  glVertex3f(x_b, y_b, 0);

  // Couleur de l'objet rouge
  glColor3ub(227, 14, 53);
  // Face côté
  glVertex3f(x_a, y_a, 0);
  glVertex3f(x_a, y_a, hauteur);
  glVertex3f(x_c, y_c, hauteur);
  glVertex3f(x_c, y_c, 0);

  // Couleur de l'objet orange
  glColor3ub(227, 110, 14);
  // Face côté
  glVertex3f(x_b, y_b, 0);
  glVertex3f(x_b, y_b, hauteur);
  glVertex3f(x_d, y_d, hauteur);
  glVertex3f(x_d, y_d, 0);

  glEnd();
}

void DrawSouthWall(int SZ) {

  // Mur côté Sud (abdc), on défini les coordonnées des coins en fonction du
  // centre 0
  // de la case du labyrinthe. Ici, SZ est la distance entre a et b ou c et d
  //
  //            vert
  //      +---+---+---+---+
  //      |               |
  //  r   +   +---+---+   +   o
  //  o   |   |       |   |   r
  //  u   +   +   0   +   +   a
  //  g   |   |       |   |   n
  //  e   a   +---+---+   b   g
  //      |               |   e
  //      c---+---+---+---d
  //
  //            bleu

  float x_a = -SZ / 2, y_a = -SZ / 4;
  float x_b = SZ / 2, y_b = -SZ / 4;
  float x_c = -SZ / 2, y_c = -SZ / 2;
  float x_d = SZ / 2, y_d = -SZ / 2;

  float hauteur = 0.6 * SZ; // Hauteur du mur

  glBegin(GL_QUADS);
  // Couleur de l'objet blanc
  glColor3ub(240, 240, 240);
  // Face coté ciel
  glVertex3f(x_c, y_c, hauteur);
  glVertex3f(x_d, y_d, hauteur);
  glVertex3f(x_b, y_b, hauteur);
  glVertex3f(x_a, y_a, hauteur);

  // Couleur de l'objet bleu
  glColor3ub(14, 67, 227);
  // Face côté
  glVertex3f(x_c, y_c, 0);
  glVertex3f(x_c, y_c, hauteur);
  glVertex3f(x_d, y_d, hauteur);
  glVertex3f(x_d, y_d, 0);

  // Couleur de l'objet vert
  glColor3ub(13, 181, 38);
  // Face côté
  glVertex3f(x_a, y_a, 0);
  glVertex3f(x_a, y_a, hauteur);
  glVertex3f(x_b, y_b, hauteur);
  glVertex3f(x_b, y_b, 0);

  // Couleur de l'objet rouge
  glColor3ub(227, 14, 53);
  // Face côté
  glVertex3f(x_a, y_a, 0);
  glVertex3f(x_a, y_a, hauteur);
  glVertex3f(x_c, y_c, hauteur);
  glVertex3f(x_c, y_c, 0);

  // Couleur de l'objet orange
  glColor3ub(227, 110, 14);
  // Face côté
  glVertex3f(x_b, y_b, 0);
  glVertex3f(x_b, y_b, hauteur);
  glVertex3f(x_d, y_d, hauteur);
  glVertex3f(x_d, y_d, 0);

  glEnd();
}

#endif // DRAW_FUNCTIONS_H
