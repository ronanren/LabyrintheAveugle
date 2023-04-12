#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include <cstdio>
#include <iostream>

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>

#include <libs/utils.h>

#include "Scene.h"
#include "Labyrinthe.h"

/** constructeur */
Scene::Scene(int **grid)
{

    m_grid = grid;
    // Création du sol
    m_Ground = new Ground();

    // création des cubes (murs)
    for (int x = 0; x < hauteur; x++)
    {
        for (int y = 0; y < largeur; y++)
        {
            m_Cube[x][y] = new Cube("data/white_noise.wav", grid[x][y]);
        }
    }

    // caractéristiques de la lampe
    m_Light = new Light();
    m_Light->setColor(20000.0, 20000.0, 20000.0);
    m_Light->setPosition(0.0, 200.0, 0.0, 1.0);
    m_Light->setDirection(0.0, -1.0, 0.0, 0.0);
    // m_Light->setAngles(0.0, 0.0);

    // activer le depth buffer
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // initialiser les matrices
    m_MatP = mat4::create();   // projection matrix, used to define the projection transformation for rendering 3D graphics onto a 2D screen
    m_MatV = mat4::create();   // view matrix, used to define the view transformation for positioning and orienting the virtual camera in the 3D world
    m_MatVM = mat4::create();  // view-model matrix, which is the combination of the view matrix and the model matrix.
    m_MatTMP = mat4::create(); // temporary matrix used for temporary calculations or transformation

    // gestion vue et souris
    m_Azimut = 90.0;
    m_Elevation = 0.0;
    m_Distance = largeur_cube;
    m_Center = vec3::create();
    last_Center = vec3::create();
    m_Clicked = false;
    m_debug = false;

    // mettre le centre de la scene dans la premiere case du labyrinthe
    vec3::set(m_Center, -largeur_cube, -largeur_cube, -largeur_cube);
    vec3::set(last_Center, -largeur_cube, -largeur_cube, -largeur_cube);
    last_maze_x = 0;
    last_maze_y = 0;
    // sauvegarder la position de la camera pour le debug
    lastPosition = vec3::create();
    vec3::copy(lastPosition, m_Center);
    lastAzimut = m_Azimut;
}

/**
 * appelée quand la taille de la vue OpenGL change
 * @param width : largeur en nombre de pixels de la fenêtre
 * @param height : hauteur en nombre de pixels de la fenêtre
 */
void Scene::onSurfaceChanged(int width, int height)
{
    // met en place le viewport
    glViewport(0, 0, width, height);

    // matrice de projection (champ de vision)
    mat4::perspective(m_MatP, Utils::radians(35.0), (float)width / height, 1.0, 10.0);
}

/**
 * appelée quand on enfonce un bouton de la souris
 * @param btn : GLFW_MOUSE_BUTTON_LEFT pour le bouton gauche
 * @param x coordonnée horizontale relative à la fenêtre
 * @param y coordonnée verticale relative à la fenêtre
 */
void Scene::onMouseDown(int btn, double x, double y)
{
    if (btn != GLFW_MOUSE_BUTTON_LEFT)
        return;
    m_Clicked = true;
    m_MousePrecX = x;
    m_MousePrecY = y;
}

/**
 * appelée quand on relache un bouton de la souris
 * @param btn : GLFW_MOUSE_BUTTON_LEFT pour le bouton gauche
 * @param x coordonnée horizontale relative à la fenêtre
 * @param y coordonnée verticale relative à la fenêtre
 */
void Scene::onMouseUp(int btn, double x, double y)
{
    m_Clicked = false;
}

/**
 * appelée quand on bouge la souris
 * @param x coordonnée horizontale relative à la fenêtre
 * @param y coordonnée verticale relative à la fenêtre
 */
void Scene::onMouseMove(double x, double y)
{
    if (!m_Clicked || !m_debug)
        return;
    m_Azimut += (x - m_MousePrecX) * 0.1;
    m_Elevation += (y - m_MousePrecY) * 0.1;
    if (m_Elevation > 90.0)
        m_Elevation = 90.0;
    if (m_Elevation < -90.0)
        m_Elevation = -90.0;
    m_MousePrecX = x;
    m_MousePrecY = y;
}

/**
 * appelée quand on appuie sur une touche du clavier
 * @param code : touche enfoncée
 */
void Scene::onKeyDown(unsigned char code)
{
    // construire la matrice inverse de l'orientation de la vue à la souris
    mat4::identity(m_MatTMP);
    mat4::rotateY(m_MatTMP, m_MatTMP, Utils::radians(-m_Azimut));
    mat4::rotateX(m_MatTMP, m_MatTMP, Utils::radians(-m_Elevation));

    // vecteur indiquant le décalage à appliquer au pivot de la rotation
    vec3 offset = vec3::create();
    int maze_x; 
    int maze_y;
    switch (code)
    {
    case GLFW_KEY_W: // touche avant Z
        vec3::transformMat4(offset, vec3::fromValues(0, 0, +vitesse_marche), m_MatTMP);
        vec3::add(m_Center, m_Center, offset);
        if (!m_debug){
            if (last_Center[0] < m_Center[0])
                maze_x = abs(floor( (m_Center[0] + rayon_collision) / (largeur_cube * 2) )) - 1;
            if (last_Center[0] >= m_Center[0])
                maze_x = abs(floor( (m_Center[0] - rayon_collision) / (largeur_cube * 2) )) - 1;
            if (last_Center[2] < m_Center[2])
                maze_y = abs(floor( (m_Center[2] + rayon_collision) / (largeur_cube * 2) )) - 1;
            if (last_Center[2] >= m_Center[2])
                maze_y = abs(floor( (m_Center[2] - rayon_collision) / (largeur_cube * 2) )) - 1;
            
            // std::cout << m_Center[0] << ", " << m_Center[2] << std::endl << std::flush;
            if (abs(m_Center[0]) <= rayon_collision || abs(m_Center[2]) <= rayon_collision || abs(m_Center[0]) >= largeur_cube * 2 * largeur - rayon_collision || abs(m_Center[2]) >= largeur_cube * 2 * hauteur - rayon_collision){
                vec3::subtract(m_Center, m_Center, offset);
                std::cout << "Vous avez atteint les limites du labyrinthe" << m_Center[0] << ", " << m_Center[2] << std::endl;
            } else if (last_maze_y != maze_y || last_maze_x != maze_x){
                bool res = Labyrinthe::hasWallBetweenCells(last_maze_x, last_maze_y, maze_x, maze_y, m_grid[last_maze_y][last_maze_x], m_grid[maze_y][maze_x]);
                // std::cout << "res = " << res << std::endl << std::flush;
                if (res){
                    vec3::subtract(m_Center, m_Center, offset);
                    std::cout << "Vous avez atteint un mur" << m_Center[0] << ", " << m_Center[2] << std::endl;
                } else {
                    last_maze_x = maze_x;
                    last_maze_y = maze_y;
                    vec3::copy(last_Center, m_Center);
                }
            }
            if((maze_x == largeur - 1) && (maze_y == hauteur - 1))std::cout << "Win !" << std::endl << std::flush;
        }
        break;
    case GLFW_KEY_S: // touche arrière S
        vec3::transformMat4(offset, vec3::fromValues(0, 0, -vitesse_marche), m_MatTMP);
        vec3::add(m_Center, m_Center, offset);
        if (!m_debug){
            if (last_Center[0] < m_Center[0])
                maze_x = abs(floor( (m_Center[0] + rayon_collision) / (largeur_cube * 2) )) - 1;
            if (last_Center[0] >= m_Center[0])
                maze_x = abs(floor( (m_Center[0] - rayon_collision) / (largeur_cube * 2) )) - 1;
            if (last_Center[2] < m_Center[2])
                maze_y = abs(floor( (m_Center[2] + rayon_collision) / (largeur_cube * 2) )) - 1;
            if (last_Center[2] >= m_Center[2])
                maze_y = abs(floor( (m_Center[2] - rayon_collision) / (largeur_cube * 2) )) - 1;
            
            // std::cout << m_Center[0] << ", " << m_Center[2] << std::endl << std::flush;
            if (abs(m_Center[0]) <= rayon_collision || abs(m_Center[2]) <= rayon_collision || abs(m_Center[0]) >= largeur_cube * 2 * largeur - rayon_collision || abs(m_Center[2]) >= largeur_cube * 2 * hauteur - rayon_collision){
                vec3::subtract(m_Center, m_Center, offset);
                std::cout << "Vous avez atteint les limites du labyrinthe" << m_Center[0] << ", " << m_Center[2] << std::endl;
            } else if (last_maze_y != maze_y || last_maze_x != maze_x){
                bool res = Labyrinthe::hasWallBetweenCells(last_maze_x, last_maze_y, maze_x, maze_y, m_grid[last_maze_y][last_maze_x], m_grid[maze_y][maze_x]);
                // std::cout << "res = " << res << std::endl << std::flush;
                if (res){
                    vec3::subtract(m_Center, m_Center, offset);
                    std::cout << "Vous avez atteint un mur" << m_Center[0] << ", " << m_Center[2] << std::endl;
                } else {
                    last_maze_x = maze_x;
                    last_maze_y = maze_y;
                    vec3::copy(last_Center, m_Center);
                }
            }
            if((maze_x == largeur - 1) && (maze_y == hauteur - 1))std::cout << "Win !" << std::endl << std::flush;
        }
        break;
    case GLFW_KEY_D: // touche droite D
        if(m_debug){
            vec3::transformMat4(offset, vec3::fromValues(-vitesse_marche, 0, 0), m_MatTMP);
            vec3::add(m_Center, m_Center, offset);
        }
        else m_Azimut += degre_rotation_tete;
        break;
    case GLFW_KEY_A: // touche gauche S
        if(m_debug){
            vec3::transformMat4(offset, vec3::fromValues(+vitesse_marche, 0, 0), m_MatTMP);
            vec3::add(m_Center, m_Center, offset);
        }
        else m_Azimut -= degre_rotation_tete;
        break;
    case GLFW_KEY_Y:
        m_debug = !m_debug;
        if (m_debug){
            std::cout << "Debug mode" << std::endl << std::flush;
            // sauvegarder la position de la camera pour le debug
            vec3::copy(lastPosition, m_Center);
            lastAzimut = m_Azimut;
            mat4::perspective(m_MatP, Utils::radians(35.0), (float)640 / 480, 1.0, 1000.0);
        } else {
            std::cout << "Normal mode" << std::endl << std::flush;
            // restaurer la position de la camera pour le debug
            vec3::copy(m_Center, lastPosition);
            m_Azimut = lastAzimut;
            m_Elevation = 0.0;
            mat4::perspective(m_MatP, Utils::radians(35.0), (float)640 / 480, 1.0, 10.0);
        }
        break;
    default:
        return;
    }
}

/**
 * Dessine l'image courante
 */
void Scene::onDrawFrame()
{
    /** préparation des matrices **/

    // positionner la caméra
    mat4::identity(m_MatV);

    // éloignement de la scène
    mat4::translate(m_MatV, m_MatV, vec3::fromValues(0.0, 0.0, -m_Distance));

    // rotation demandée par la souris
    mat4::rotateX(m_MatV, m_MatV, Utils::radians(m_Elevation));
    mat4::rotateY(m_MatV, m_MatV, Utils::radians(m_Azimut));

    // centre des rotations
    mat4::translate(m_MatV, m_MatV, m_Center);

    /** gestion des lampes **/

    // calculer la position et la direction de la lampe par rapport à la scène
    m_Light->transform(m_MatV);

    // fournir position et direction en coordonnées caméra aux objets éclairés
    m_Ground->setLight(m_Light);

    /** dessin de l'image **/

    // effacer l'écran
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // translater la scène pour que le sol soit centré sur l'origine
    mat4::translate(m_MatV, m_MatV, vec3::fromValues(largeur * largeur_cube, 0.0, hauteur * largeur_cube));

    // dessiner le sol
    m_Ground->onDraw(m_MatP, m_MatV);

    // translater la scène pour que le point (0,0) soit au centre du sol et au milieu du premier cube
    mat4::translate(m_MatV, m_MatV, vec3::fromValues(-largeur * largeur_cube + largeur_cube, largeur_cube - 0.01, -hauteur * largeur_cube + largeur_cube));

    // Dessiner et modifier la position des cubes (murs)
    for (int x = 0; x < hauteur; x++)
    {
        for (int y = 0; y < largeur; y++)
        {
            m_Cube[x][y]->onRender(m_MatP, m_MatV);
            mat4::translate(m_MatV, m_MatV, vec3::fromValues(largeur_cube * 2.0, 0.0, 0.0));
        }
        mat4::translate(m_MatV, m_MatV, vec3::fromValues(largeur_cube * -2.0 * largeur, 0.0, largeur_cube * 2.0));
    }

    // affichage du point en (0,0)
    glPointSize(5.0);
    glBegin(GL_POINTS);
    glVertex2f(0.0, 0.0);
    glEnd();
}

/** supprime tous les objets de cette scène */
Scene::~Scene()
{
    for (int x = 0; x < hauteur; x++)
    {
        for (int y = 0; y < largeur; y++)
        {
            delete m_Cube[x][y];
        }
    }
    delete m_Ground;
    delete m_Light;
}
