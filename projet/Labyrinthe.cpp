#include <Labyrinthe.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

Labyrinthe::Labyrinthe(int largeur, int hauteur)
{
    m_largeur = largeur;
    m_hauteur = hauteur;
}

int **Labyrinthe::generer()
{
    // Implementation de l'algo : https://weblog.jamisbuck.org/2011/2/3/maze-generation-sidewinder-algorithm
    cout << "Génération d'un labyrinthe de " << m_largeur << "x" << m_hauteur << endl;
    srand(time(nullptr));

    int **grid = new int *[m_largeur];
    int cell;

    // initialisation du tableau
    for (int y = 0; y < m_hauteur; y++)
    {
        grid[y] = new int[m_largeur];
        for (int x = 0; x < m_largeur; x++)
        {
            grid[y][x] = 0;
        }
    }

    // Sidewinder Algorithm
    int run_start = 0;
    for (int y = 0; y < m_hauteur; y++)
    {
        run_start = 0;
        for (int x = 0; x < m_largeur; x++)
        {
            // creuse le nord
            if (y > 0 && (x + 1 == m_largeur || (rand() % 2) == 0))
            {
                cell = run_start + std::rand() % (x - run_start + 1);
                grid[y][cell] |= 0b1000;     // North
                grid[y - 1][cell] |= 0b0010; // South
                run_start = x + 1;
            } // creuse l'est
            else if (x + 1 < m_largeur)
            {
                grid[y][x] |= 0b0100;     // Est
                grid[y][x + 1] |= 0b0001; // West
            }
        }
    }

    // affichage du tableau
    // W, S, E, N = 1, 2, 4, 8
    for (int y = 0; y < m_hauteur; y++)
    {
        for (int x = 0; x < m_largeur; x++)
        {
            cout.width(2);
            cout << grid[y][x] << " ";
        }
        cout << endl;
    }

    return grid;
}

bool Labyrinthe::hasWallBetweenCells(int x1, int y1, int x2, int y2, int grid1, int grid2) {
    // std::cout << "hasWallBetweenCells(" << x1 << ", " << y1 << ", " << x2 << ", " << y2 << ", " << grid1 << ", " << grid2 << ")" << std::endl;
    if (x1 == x2) {
        if (y1 > y2) {
            return (grid1 & 0b1000) == 0;
        } else {
            return (grid2 & 0b1000) == 0;
        }
    } else if (y1 == y2) {
        if (x1 > x2) {
            return (grid1 & 0b0001) == 0;
        } else {
            return (grid2 & 0b0001) == 0;
        }
    } else if (abs(x1 - x2) == abs(y1 - y2)) { // diagonal
        int dx = x2 - x1;
        int dy = y2 - y1;
        if (dx > 0 && dy > 0) { // bottom right diagonal
            return (grid1 & 0b0010) == 0;
        } else if (dx > 0 && dy < 0) { // top right diagonal
            return (grid1 & 0b1000) == 0;
        } else if (dx < 0 && dy > 0) { // bottom left diagonal
            return (grid1 & 0b0100) == 0;
        } else if (dx < 0 && dy < 0) { // top left diagonal
            return (grid1 & 0b0001) == 0;
        }
    }
    return false;
}