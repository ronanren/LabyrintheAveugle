// Définition de la classe Ground

#include <iostream>

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <math.h>

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>

#include <utils.h>

#include <Ground.h>
#include "Constants.h"

using namespace mesh;

/** constructeur */
Ground::Ground(): Mesh("sol")
{
    // matériaux
    m_Material = new MaterialTexture("data/ground.jpg", GL_LINEAR_MIPMAP_LINEAR, GL_REPEAT);
    setMaterials(m_Material);

    double larg = static_cast<double>(largeur);
    double haut = static_cast<double>(hauteur);

    // sommets
    /*
      p1 ____ p3
        |____|
      p4      p2
    */

    // ajout des sommets
    Vertex* P1 = new Vertex(this, -larg, 0.0, haut); P1->setTexCoords(  0.0,   0.0); P1->setNormal(0.0,1.0,0.0);
    Vertex* P2 = new Vertex(this, larg, 0.0, haut); P2->setTexCoords(larg*2.0,   0.0); P2->setNormal(0.0,1.0,0.0);
    Vertex* P3 = new Vertex(this, larg, 0.0, -haut); P3->setTexCoords(larg*2.0, haut*2.0); P3->setNormal(0.0,1.0,0.0);
    Vertex* P4 = new Vertex(this, -larg, 0.0, -haut); P4->setTexCoords(  0.0, haut*2.0); P4->setNormal(0.0,1.0,0.0);

    // ajout des triangles
    addQuad(P1, P2, P3, P4);
}


/**
 * définit la lampe
 * @param light : instance de Light spécifiant les caractéristiques de la lampe
 */
void Ground::setLight(Light* light)
{
    m_Material->setLight(light);
}


/** destructeur */
Ground::~Ground()
{
    // libération du matériau
    delete m_Material;
}
