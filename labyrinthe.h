/** Author : F. Zdrada **/

#ifndef LABYRINTHE_H
#define LABYRINTHE_H

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

// Classe pour afficvher le labyrinthe en 3D
class Labyrinthe {
   private:
    GLUquadric *quadric;
    GLuint texture;

   public:
    Maze maze = Maze(0, 0);

    // Constructeurs
    Labyrinthe();
    Labyrinthe(int, int);

    float ball_x, ball_y;
    int i_exit, j_exit;
    Cell::Direction d_exit;

    // Destructeur
    virtual ~Labyrinthe();

    // Methode d'affichage
    void Display() const;
    void ShowBall() const;
    void Show2DMap(float, float, float) const;
    void OpenExit() { maze.grid_[i_exit][j_exit].setFrontier(d_exit, false); };

    // Getters
    std::tuple<float, float> getBallPosition() { return std::make_tuple(ball_x, ball_y); }
};

#endif  // LABYRINTHE_H
