
#include "Forme.h"
#include "ofMain.h"

using namespace std;

void Forme::setup() {

    containsPointTriangle = false;
    containsPointCercle = false;
    containsPointRectangle = false;
    //v_formes.clear();
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
    //wE = 500; 
    //hE = 300; 
};


// Implémentation du constructeur
Forme::Forme() {
    // Initialisation du vecteur ou autres initialisations si nécessaire
}

// Implémentation du destructeur
Forme::~Forme() {
    // Libération de la mémoire ou nettoyage des ressources si nécessaire
}

Forme::Forme(TypeForme type, float x1, float y1, float x2, float y2, float x3, float y3, vector<ofColor> colors, bool fill, float stroke)
    : type(type), x1(x1), y1(y1), x2(x2), y2(y2), x3(x3), y3(y3), formeColors(colors), formeFill(fill), formeStroke(stroke) {}

Forme::Forme(TypeForme type, float x, float y, float rayon, vector<ofColor> colors, bool fill, float stroke)
    : type(type), xC(x), yC(y), rayon(rayon), formeColors(colors), formeFill(fill), formeStroke(stroke) {}

Forme::Forme(TypeForme type, float x, float y, float width, float height, vector<ofColor> colors, bool fill, float stroke)
    : type(type), xR(x), yR(y), width(width), height(height), formeColors(colors), formeFill(fill), formeStroke(stroke) {}

Forme::Forme(TypeForme type, float x1, float y1, float xB1, float yB1, float xB2, float yB2, float x2, float y2, vector<ofColor> colors, bool fill, float stroke)
    : type(type), x1(x1), y1(y1), xB1(xB1), yB1(yB1), xB2(xB2), yB2(yB2), x2(x2), y2(y2), formeColors(colors), formeFill(fill), formeStroke(stroke) {}

//Forme::Forme(TypeForme type, float x, float y, float width, float height)
//    : type(type), x1(x), y1(y), width(width), height(height){}

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

void Forme::setColorParameters() {

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
