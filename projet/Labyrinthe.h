#ifndef LABYRINTHE_H
#define LABYRINTHE_H

class Labyrinthe
{
public:
    Labyrinthe(int largeur, int hauteur);
    int **generer();
    static bool hasWallBetweenCells(int x1, int y1, int x2, int y2, int grid1, int grid2);

private:
    int m_largeur;
    int m_hauteur;
};

#endif