#ifndef LABYRINTHE_H
#define LABYRINTHE_H

class Labyrinthe {
public:
    Labyrinthe(int largeur, int hauteur);
    void generer();

private:
    int m_largeur;
    int m_hauteur;
};

#endif