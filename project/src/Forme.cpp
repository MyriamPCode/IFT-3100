
#include "Forme.h"
#include "ofMain.h"

using namespace std;

void Forme::setup() {
    x1 = 0; 
    y1 = 0;
    x2 = -32;
    y2 = 64;
    x3 = 32;
    y3 = 64;
    xC = 100;
    yC = 100;
    xR = 50;
    yR = 50;
    width = 100;
    height = 200;
    rayon = 50;
    xS = 2560.00 / 2.00;
    yS = 1440.00 / 2.00;
    zS = 0.00; 
    sphereRad = 150.00; 
};


Forme::Forme(TypeForme type, ofVec3f point1, ofVec3f point2, ofVec3f point3)
    : type(type), point1(point2), point2(point2), point3(point3) {}

// Implémentation du constructeur
Forme::Forme() {
    // Initialisation du vecteur ou autres initialisations si nécessaire
}

// Implémentation du destructeur
Forme::~Forme() {
    // Libération de la mémoire ou nettoyage des ressources si nécessaire
}

Forme::Forme(TypeForme type, float x1, float y1, float x2, float y2, float x3, float y3)
    : type(type), x1(x1), y1(y1), x2(x2), y2(y2), x3(x3), y3(y3) {}

Forme::Forme(TypeForme type, float x, float y, float rayon)
    : type(type), xC(x), yC(y), rayon(rayon) {}

Forme::Forme(TypeForme type, float x, float y, float width, float height)
    : type(type), xR(x), yR(y), width(width), height(height) {}

Forme::Forme(TypeForme type, float x1, float y1, float xB1, float yB1, float xB2, float yB2, float x2, float y2)
    : type(type), x1(x1), y1(y1), xB1(xB1), yB1(yB1), xB2(xB2), yB2(yB2), x2(x2), y2(y2) {}

Forme::Forme(TypeForme type, ofVec3f p)
    : type(type), p(p) {}

Forme::Forme(TypeForme type, ofVec3f v, float rad)
    : type(type), v(v), sphereRad(rad) {}

//Forme::Forme(TypeForme type, float x, float y, float z, float radius)
//    : type(type), xS(x), yS(y), zS(z), sphereRad(radius) {}