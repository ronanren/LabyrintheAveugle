// Définition de la classe Cube

#include <iostream>

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <math.h>

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>

#include <utils.h>

#include <Cube.h>
#include <Material.h>
#include "Constants.h"

using namespace mesh;

/** matériau pour le cube **/
class MaterialCube : public Material
{
public:
    MaterialCube() : Material("MatCube")
    {
        // vertex shader
        std::string srcVertexShader =
            "#version 300 es\n"
            "uniform mat4 matP;\n"
            "uniform mat4 matVM;\n"
            "in vec3 glVertex;\n"
            "in vec3 glColor;\n"
            "out vec3 frgColor;\n"
            "void main()\n"
            "{\n"
            "    gl_Position = matP * matVM * vec4(glVertex, 1.0);\n"
            "    frgColor = glColor;\n"
            "}";

        // fragment shader
        std::string srcFragmentShader =
            "#version 300 es\n"
            "precision mediump float;\n"
            "in vec3 frgColor;\n"
            "out vec4 glFragColor;\n"
            "void main()\n"
            "{\n"
            "    glFragColor = vec4(frgColor, 1.0);\n"
            "}";

        setShaders(srcVertexShader, srcFragmentShader);
    }
};

/** constructeur */
Cube::Cube(std::string soundpathname, int walls) : Mesh("cube")
{
    // matériau
    m_Material = new MaterialCube();
    setMaterials(m_Material);

    // maillage
    const float b = largeur_cube;

    // sommets
    /*
          p3  ____    p0
        p2  /|___/| p1
         p7 |/___|/   p4
        p6          p5
    */
    Vertex *P0 = new Vertex(this, +b, +b, +b);
    Vertex *P1 = new Vertex(this, +b, +b, -b);
    Vertex *P2 = new Vertex(this, -b, +b, -b);
    Vertex *P3 = new Vertex(this, -b, +b, +b);
    Vertex *P4 = new Vertex(this, +b, -b, +b);
    Vertex *P5 = new Vertex(this, +b, -b, -b);
    Vertex *P6 = new Vertex(this, -b, -b, -b);
    Vertex *P7 = new Vertex(this, -b, -b, +b);

    // couleurs
    // couleurs
    P0->setColor(0.498, 0.160, 0.101); // P0 couleur brique la plus foncée
    P1->setColor(0.537, 0.176, 0.113); // P1 couleur brique légèrement plus claire
    P2->setColor(0.576, 0.192, 0.125); // P2 couleur brique encore plus claire
    P3->setColor(0.615, 0.208, 0.137); // P3 couleur brique encore plus claire
    P4->setColor(0.654, 0.224, 0.149); // P4 couleur brique encore plus claire
    P5->setColor(0.693, 0.240, 0.161); // P5 couleur brique encore plus claire
    P6->setColor(0.732, 0.256, 0.173); // P6 couleur brique encore plus claire
    P7->setColor(0.771, 0.272, 0.185); // P7 couleur brique la plus claire

    // ouverture du flux audio à placer dans le buffer
    buffer = alutCreateBufferFromFile(soundpathname.c_str());
    if (buffer == AL_NONE) {
         std::cerr << "unable to open file " << soundpathname << std::endl;
         alGetError();
         throw std::runtime_error("file not found or not readable");
    }

    // quads
    // addQuad(P1,P0,P3,P2); // face du dessus
    addQuad(P4, P5, P6, P7); // face du dessous

    float refDistance = 1.0f; // Distance de référence (où le son est le plus fort)
    float maxDistance = 5.0f; // Distance maximale (où le son est inaudible)
    float rolloffFactor = 2.0f; // Facteur de décroissance sonore

    if ((walls >=1) && (walls <= 3) || (walls >= 8) && (walls <= 11)){
        addQuad(P5, P4, P0, P1); // est
    }
    if ((walls >= 1) && (walls <= 7)){
        addQuad(P6, P5, P1, P2); // north
    }
    if (walls == 1 || walls == 4 || walls == 5 || walls == 8 || walls == 9 || walls == 12 || walls == 13){
        addQuad(P4, P7, P3, P0); // south
    }
    if (walls % 2 == 0){
        addQuad(P7, P6, P2, P3); // west
        // alGenSources(1, &source);
        // alSourcei(source, AL_BUFFER, buffer);
        // alSource3f(source, AL_POSITION, -b, 0.0f, 0.0f);
        // alSourcei(source, AL_LOOPING, AL_TRUE); // Définition de la boucle
        
        // // Ajout du système d'atténuation sonore en fonction de la distance
        // alSourcef(source, AL_REFERENCE_DISTANCE, refDistance);
        // alSourcef(source, AL_MAX_DISTANCE, maxDistance);
        // alSourcef(source, AL_ROLLOFF_FACTOR, rolloffFactor);

        // alSourcePlay(source);
    }
}

/**
 * dessiner le cube
 * @param matP : matrice de projection
 * @param matMV : matrice view*model (caméra * position objet)
 */
void Cube::onRender(const mat4 &matP, const mat4 &matVM)
{
    /** dessin OpenGL **/

    onDraw(matP, matVM);

    /** sonorisation OpenAL **/

    // gestion de l'atténuation sonore
    // alSourcef(source, AL_GAIN, 1.0f / (1.0f + 10));
    
    // obtenir la position relative à la caméra
    // vec4 pos = vec4::fromValues(0, 0, 0, 1); // point en (0,0,0)
    // vec4::transformMat4(pos, pos, matVM);
    // // std::cout << "Position = " << vec4::str(pos);
    // alSource3f(source, AL_POSITION, pos[0], pos[1], pos[2]);

    // // obtenir la direction relative à la caméra
    // vec4 dir = vec4::fromValues(0, 0, 1, 0); // vecteur +z
    // vec4::transformMat4(dir, dir, matVM);
    // // std::cout << "    Direction = " << vec4::str(dir) << std::endl;
    // alSource3f(source, AL_DIRECTION, dir[0], dir[1], dir[2]);
}

vec3 &Cube::getPosition()
{
    return m_Position;
}

void Cube::setPosition(vec3 pos)
{
    vec3::copy(m_Position, pos);
}

/** destructeur */
Cube::~Cube()
{
    // libération du matériau
    delete m_Material;

    // libération des ressources openal
    alDeleteSources(1, &source);
    alDeleteBuffers(1, &buffer);
}
