#include <Labyrinthe.h>
#include <iostream>
#include <cstdlib>
#include <ctime>

Labyrinthe::Labyrinthe(int largeur, int hauteur) {
    m_largeur = largeur;
    m_hauteur = hauteur;
}

void Labyrinthe::generer() {
    // Implementation de l'algo : https://weblog.jamisbuck.org/2011/2/3/maze-generation-sidewinder-algorithm
    std::cout << "Génération d'un labyrinthe de " << m_largeur << "x" << m_hauteur << std::endl;
    std::srand(std::time(nullptr));

    int grid[m_largeur][m_hauteur];
    int cell;

    // initialisation du tableau
    for (int x = 0; x < m_largeur; x++) {
        for (int y = 0; y < m_hauteur; y++) {
          grid[x][y] = 0;  
        }
    }
    int run_start = 0;
    for(int y = 0; y < m_hauteur; y++){
        run_start = 0;
        for(int x = 0; x < m_largeur; x++){
            // creuse le nord
            if (y > 0 && (x+1 == m_largeur || (rand() % 2) == 0)){
                cell = run_start + std::rand() % (x - run_start + 1);
                grid[y][cell] |= 0b1000; // North
                grid[y-1][cell] |= 0b0010; // South
                run_start = x + 1;
            } // creuse l'est
            else if (x+1 < m_largeur){
                grid[y][x] |= 0b0100; // Est
                grid[y][x+1] |= 0b0001; // West 
            }
        }
    }

    // affichage du tableau 
    // W, S, E, N = 1, 2, 4, 8
    for (int y = 0; y < m_hauteur; y++) {
        for (int x = 0; x < m_largeur; x++) {
            std::cout << grid[y][x] << " ";
        }
        std::cout << std::endl;
    }
}