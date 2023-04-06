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
        grid[y] = new int[m_hauteur];
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