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

/** constructeur */
Scene::Scene(int** grid)
{   
    // Création du sol
    m_Ground = new Ground();

    // création des cubes (murs)
    for (int x = 0; x < hauteur; x++) {
        for (int y = 0; y < largeur; y++) {
            m_Cube[x][y] = new Cube("data/white_noise.wav", grid[x][y]);
        }
    }

    // caractéristiques de la lampe
    m_Light = new Light();
    m_Light->setColor(20000.0, 20000.0, 20000.0);
    m_Light->setPosition(0.0,  200.0,  0.0, 1.0);
    m_Light->setDirection(0.0, -1.0, 0.0, 0.0);
    // m_Light->setAngles(0.0, 0.0);

    // couleur du fond : gris foncé
    glClearColor(0.4, 0.4, 0.4, 0.0);

    // activer le depth buffer
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // initialiser les matrices
    m_MatP = mat4::create();
    m_MatV = mat4::create();
    m_MatVM = mat4::create();
    m_MatTMP = mat4::create();

    // gestion vue et souris
    m_Azimut    = 0.0;
    m_Elevation = 80.0;
    m_Distance  = 50.0;
    m_Center    = vec3::create();
    m_Clicked   = false;
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
    mat4::perspective(m_MatP, Utils::radians(30.0), (float)width / height, 0.1, 1000.0);
}


/**
 * appelée quand on enfonce un bouton de la souris
 * @param btn : GLFW_MOUSE_BUTTON_LEFT pour le bouton gauche
 * @param x coordonnée horizontale relative à la fenêtre
 * @param y coordonnée verticale relative à la fenêtre
 */
void Scene::onMouseDown(int btn, double x, double y)
{
    if (btn != GLFW_MOUSE_BUTTON_LEFT) return;
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
    if (! m_Clicked) return;
    m_Azimut  += (x - m_MousePrecX) * 0.1;
    m_Elevation += (y - m_MousePrecY) * 0.1;
    if (m_Elevation >  90.0) m_Elevation =  90.0;
    if (m_Elevation < -90.0) m_Elevation = -90.0;
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
    switch (code) {
    case GLFW_KEY_W: // avant
        m_Distance *= exp(-0.01);
        break;
    case GLFW_KEY_S: // arrière
        m_Distance *= exp(+0.01);
        break;
    case GLFW_KEY_A: // droite
        vec3::transformMat4(offset, vec3::fromValues(+0.1, 0, 0), m_MatTMP);
        break;
    case GLFW_KEY_D: // gauche
        vec3::transformMat4(offset, vec3::fromValues(-0.1, 0, 0), m_MatTMP);
        break;
    case GLFW_KEY_Q: // haut
        vec3::transformMat4(offset, vec3::fromValues(0, -0.1, 0), m_MatTMP);
        break;
    case GLFW_KEY_Z: // bas
        vec3::transformMat4(offset, vec3::fromValues(0, +0.1, 0), m_MatTMP);
        break;
    default:
        return;
    }

    // appliquer le décalage au centre de la rotation
    vec3::add(m_Center, m_Center, offset);
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

    // dessiner le sol
    m_Ground->onDraw(m_MatP, m_MatV);

    mat4::translate(m_MatV, m_MatV, vec3::fromValues(0.0, 0.99, 0.0));

    // affichage du point en (0,0)
    glPointSize(5.0);
    glBegin(GL_POINTS);
    glVertex2f(0.0, 0.0);
    glEnd();

    // Dessiner et modifier la position des cubes (murs)
    for (int x = 0; x < hauteur; x++) {
        for (int y = 0; y < largeur; y++) {
            m_Cube[x][y]->onRender(m_MatP, m_MatV);
            mat4::translate(m_MatV, m_MatV, vec3::fromValues(largeur_cube * 2.0, 0.0, 0.0));
        }
        mat4::translate(m_MatV, m_MatV, vec3::fromValues(largeur_cube * -2.0 * largeur, 0.0, largeur_cube * 2.0));
    }
}

/** supprime tous les objets de cette scène */
Scene::~Scene()
{   
    for (int x = 0; x < hauteur; x++) {
        for (int y = 0; y < largeur; y++) {
            delete m_Cube[x][y];
        }
    }
    delete m_Ground;
}
