#ifndef WALL_H
#define WALL_H

#include "cell.h"
#include "maze.h"

#include <qopengl.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <QColor>


// SZ est la taille du côté du carré d'une case
// du labyrinthe (voir draw_functions.h)
const int SZ = 20;
// coeff_move = Coefficient pour aller d'une case à l'autre
// On considère que les cases se "chevauchent" de 1/4 * SZ, ainsi on ne se
// déplace que de 3/4 de SZ (voir schéma ligne 13 dans draw_functions.h)
const float coeff_move = 3 * SZ / 4;


// Classe dediee pour la gestion d'une planete
class Wall {

private:
  GLUquadric *quadric;
  GLUquadric *orbite;
  GLuint texture;

public:
  Maze maze = Maze(0, 0);
  // Constructeur avec parametres
  Wall(int, int);

  // Destructeur
  virtual ~Wall();

  // Methode d'affichage
  void Display() const;
};

#endif // WALL_H
