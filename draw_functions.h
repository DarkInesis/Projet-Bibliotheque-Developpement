#ifndef DRAW_FUNCTIONS_H
#define DRAW_FUNCTIONS_H

#include <QGLWidget>

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

void DrawWall(float x_a, float y_a, float x_b, float y_b, float x_c, float y_c, float x_d,
              float y_d, float hauteur) {
    glBegin(GL_QUADS);

    // Couleur de l'objet blanc
    glColor3ub(240, 240, 240);
    // Face côté ciel
    glNormal3f(0.0, 0.0, 1.0);
    glVertex3f(x_c, y_c, hauteur);
    glVertex3f(x_d, y_d, hauteur);
    glVertex3f(x_b, y_b, hauteur);
    glVertex3f(x_a, y_a, hauteur);

    // Couleur de l'objet bleu
    glColor3ub(14, 67, 227);
    // Face côté sud
    glNormal3f(0.0, -1.0, 0.0);
    glVertex3f(x_c, y_c, 0);
    glVertex3f(x_c, y_c, hauteur);
    glVertex3f(x_d, y_d, hauteur);
    glVertex3f(x_d, y_d, 0);

    // Couleur de l'objet vert
    glColor3ub(13, 181, 38);
    // Face côté nord
    glNormal3f(0.0, 1.0, 0.0);
    glVertex3f(x_a, y_a, 0);
    glVertex3f(x_a, y_a, hauteur);
    glVertex3f(x_b, y_b, hauteur);
    glVertex3f(x_b, y_b, 0);

    // Couleur de l'objet rouge
    glColor3ub(227, 14, 53);
    // Face côté ouest
    glNormal3f(-1.0, 0.0, 0.0);
    glVertex3f(x_a, y_a, 0);
    glVertex3f(x_a, y_a, hauteur);
    glVertex3f(x_c, y_c, hauteur);
    glVertex3f(x_c, y_c, 0);

    // Couleur de l'objet orange
    glColor3ub(227, 110, 14);
    // Face côté est
    glNormal3f(1.0, 0.0, 0.0);
    glVertex3f(x_b, y_b, 0);
    glVertex3f(x_b, y_b, hauteur);
    glVertex3f(x_d, y_d, hauteur);
    glVertex3f(x_d, y_d, 0);

    glEnd();
}

void DrawPostSW(int SZ) {
    // Ici on dessine le poteau en bas à gauche
    //
    //     y
    //     ʌ
    //     |
    //     +--> x
    //
    //            vert
    //      +---+---+---+---+
    //      |               |
    //  r   +   +---+---+   +   o
    //  o   |   |       |   |   r
    //  u   +   +   0   +   +   a
    //  g   |   |       |   |   n
    //  e   a   b---+---+   +   g
    //      |               |   e
    //      c---d---+---+---+
    //
    //            bleu

    float hauteur = 0.6 * SZ;  // Hauteur du poteau
    float x_a = -SZ / 2, y_a = -SZ / 4;
    float x_b = -SZ / 4, y_b = -SZ / 4;
    float x_c = -SZ / 2, y_c = -SZ / 2;
    float x_d = -SZ / 4, y_d = -SZ / 2;

    DrawWall(x_a, y_a, x_b, y_b, x_c, y_c, x_d, y_d, hauteur);
}

void DrawPostSE(int SZ) {
    // Ici on dessine le poteau en bas à droite
    //
    //            vert
    //      +---+---+---+---+
    //      |               |
    //  r   +   +---+---+   +   o
    //  o   |   |       |   |   r
    //  u   +   +   0   +   +   a
    //  g   |   |       |   |   n
    //  e   +   +---+---a   b   g
    //      |               |   e
    //      +---+---+---c---d
    //
    //            bleu

    float hauteur = 0.6 * SZ;  // Hauteur du poteau
    float x_a = SZ / 4, y_a = -SZ / 4;
    float x_b = SZ / 2, y_b = -SZ / 4;
    float x_c = SZ / 4, y_c = -SZ / 2;
    float x_d = SZ / 2, y_d = -SZ / 2;

    DrawWall(x_a, y_a, x_b, y_b, x_c, y_c, x_d, y_d, hauteur);
}

void DrawEastWall(int SZ) {
    // Mur côté Est (rectange abdc), on défini les coordonnées des coins en
    // fonction du centre 0
    // de la case du labyrinthe. SZ correspond à la longueur d'un côté extérieur
    //
    //            vert
    //      +---+---+---+---+
    //      |               |
    //  r   +   +---+---a   b   o
    //  o   |   |       |   |   r
    //  u   +   +   0   +   +   a
    //  g   |   |       |   |   n
    //  e   +   +---+---c   d   g
    //      |               |   e
    //      +---+---+---+---+
    //
    //            bleu

    float x_a = SZ / 4, y_a = SZ / 4;
    float x_b = SZ / 2, y_b = SZ / 4;
    float x_c = SZ / 4, y_c = -SZ / 4;
    float x_d = SZ / 2, y_d = -SZ / 4;

    float hauteur = 0.6 * SZ;  // Hauteur du mur

    DrawWall(x_a, y_a, x_b, y_b, x_c, y_c, x_d, y_d, hauteur);
}

void DrawSouthWall(int SZ) {
    // Mur côté Sud (abdc), on défini les coordonnées des coins en fonction du
    // centre 0
    // de la case du labyrinthe. Ici, SZ est la longueur d'un côté extérieur
    //
    //            vert
    //      +---+---+---+---+
    //      |               |
    //  r   +   +---+---+   +   o
    //  o   |   |       |   |   r
    //  u   +   +   0   +   +   a
    //  g   |   |       |   |   n
    //  e   +   a---+---b   +   g
    //      |               |   e
    //      +---c---+---d---+
    //
    //            bleu

    float x_a = -SZ / 4, y_a = -SZ / 4;
    float x_b = SZ / 4, y_b = -SZ / 4;
    float x_c = -SZ / 4, y_c = -SZ / 2;
    float x_d = SZ / 4, y_d = -SZ / 2;

    float hauteur = 0.6 * SZ;  // Hauteur du mur

    DrawWall(x_a, y_a, x_b, y_b, x_c, y_c, x_d, y_d, hauteur);
}


#endif  // DRAW_FUNCTIONS_H
