
#include "Forme.h"
#include "ofMain.h"

using namespace std;

void Forme::setup() {

    containsPointTriangle = false;
    containsPointCercle = false;
    containsPointRectangle = false;
    v_formes.clear();
    x1 = 0; 
    y1 = 0;
    x2 = -16;
    y2 = 32;
    x3 = 16;
    y3 = 32;
    xC = 100;
    yC = 100;
    xR = 50;
    yR = 50;
    width = 100;
    height = 200;
    rayon = 50;


};


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

void Forme::draw() {
    // Dessiner la forme en fonction de son type
    switch (type) {
    case TRIANGLE:
        // Dessiner un triangle avec les attributs x1, y1, x2, y2, x3, y3
        break;
    case CERCLE:
        // Dessiner un cercle avec les attributs x, y, rayon
        break;
    case RECTANGLE:
        // Dessiner un rectangle avec les attributs x, y, width, height
        break;
    }
}

bool Forme::containsPoint(float x, float y) {
    // Vérifier si le point (x, y) est à l'intérieur de la forme en fonction de son type
    switch (type) {
    case TRIANGLE:
        // Vérifier si le point est à l'intérieur du triangle
        if (TRUE)
        {
            return containsPointTriangle = TRUE;
        }
        else
            return containsPointTriangle = FALSE; 
        break;
    case CERCLE:
        // Vérifier si le point est à l'intérieur du cercle
        if (TRUE)
        {
            return containsPointCercle = TRUE;
        }
        else
            return containsPointCercle = FALSE;
        break;
    case RECTANGLE:
        // Vérifier si le point est à l'intérieur du rectangle
        if (TRUE)
        {
            return containsPointRectangle = TRUE;
        }
        else
            return containsPointRectangle = FALSE;
        break;
    }
}

float Forme::getX1() const { return x1; }
float Forme::getY1() const { return y1; }
float Forme::getX2() const { return x2; }
float Forme::getY2() const { return y2; }
float Forme::getX3() const { return x3; }
float Forme::getY3() const { return y3; }
float Forme::getXC() const { return xC; }
float Forme::getYC() const { return yC; }
float Forme::getXR() const { return xR; }
float Forme::getYR() const { return yR; }
float Forme::getWidth() const { return width; }
float Forme::getHeight() const { return height; }
float Forme::getRayon() const { return rayon; }

void Forme::setX1(float x) { this->x1 = x; }
void Forme::setY1(float y) { this->y1 = y; }
void Forme::setX2(float x) { this->x2 = x; }
void Forme::setY2(float y) { this->y2 = y; }
void Forme::setX3(float x) { this->x3 = x; }
void Forme::setY3(float y) { this->y3 = y; }
void Forme::setXC(float x) { this->xC = x; }
void Forme::setYC(float y) { this->yC = y; }
void Forme::setXR(float x) { this->xR = x; }
void Forme::setYR(float y) { this->yR = y; }
void Forme::setWidth(float w) { this->width = w; }
void Forme::setHeight(float h) { this->height = h; }
void Forme::setRayon(float r) { this->rayon = r; }