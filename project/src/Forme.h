#pragma once

#include "ofMain.h"
#include <vector>
#include <string>

// Déclaration de la classe Forme
class Forme {
public:
    enum TypeForme
    {
        TRIANGLE, CERCLE, RECTANGLE, LIGNE, ELLIPSE, BEZIER
    };

    Forme(TypeForme type, float x1, float y1, float x2, float y2, float x3, float y3, vector<ofColor> colors, bool fill, float stroke);
    Forme(TypeForme type, float x, float y, float rayon, vector<ofColor> colors, bool fill, float stroke);
    Forme(TypeForme type, float x, float y, float width, float height, vector<ofColor> colors, bool fill, float stroke);
    Forme(TypeForme type, float x1, float y1, float xB1, float yB1, float xB2, float yB2, float x2, float y2, vector<ofColor> colors, bool fill, float stroke);
    //Forme(TypeForme type, float x, float y, float width, float height); 

    bool containsPoint(float x, float y);
    void setColorParameters();

    void draw();
    void setup();

    // Constructeur
    Forme();

    // Destructeur virtuel
    virtual ~Forme();

    float getX1() const { return x1; }
    float getY1() const { return y1; }
    float getX2() const { return x2; }
    float getY2() const { return y2; }
    float getX3() const { return x3; }
    float getY3() const { return y3; }
    float getXC() const { return xC; }
    float getYC() const { return yC; }
    float getXR() const { return xR; }
    float getYR() const { return yR; }
    float getWidth() const { return width; }
    float getHeight() const { return height; }
    float getRayon() const { return rayon; }
    float getXB1() const { return xB1; }
    float getYB1() const { return yB1; }
    float getXB2() const { return xB2; }
    float getYB2() const { return yB2; }
    vector<ofColor> getColors() const { return formeColors; }
    bool getIsFilled() const { return formeFill; }
    float getOutlineWeight() const { return formeStroke; }
    //float getXE() const { return xE; }
    //float getYE() const { return yE; }
    //float getWidthEllipse() const { return wE; }
    //float getHeightEllipse() const { return hE; }

    void setX1(float x) { this->x1 = x; }
    void setY1(float y) { this->y1 = y; }
    void setX2(float x) { this->x2 = x; }
    void setY2(float y) { this->y2 = y; }
    void setX3(float x) { this->x3 = x; }
    void setY3(float y) { this->y3 = y; }
    void setXC(float x) { this->xC = x; }
    void setYC(float y) { this->yC = y; }
    void setXR(float x) { this->xR = x; }
    void setYR(float y) { this->yR = y; }
    void setWidth(float w) { this->width = w; }
    void setHeight(float h) { this->height = h; }
    void setRayon(float r) { this->rayon = r; }
    void setXB1(float x) { this->xB1 = x; }
    void setYB1(float y) { this->yB1 = y; }
    void setXB2(float x) { this->xB2 = x; }
    void setYB2(float y) { this->yB2 = y; }
    void setColors(ofColor outline, ofColor fill) { this->formeColors = {outline, fill}; }
    void setIsFilled(bool fill) { this->formeFill = fill; }
    void setOutlineWeight(float stroke) { this->formeStroke = stroke; }
    //void setXE(float x)  { this->xE = x; }
    //void setYE(float y)  { this->yE = y; }
    //void setWidthEllipse(float w)  { this->wE = w; }
    //void setHeightEllipse(float h) { this->hE = h; }

    //using TypeForme = string;
    TypeForme getType() const { return type; }


    ofPolyline ligne; // ligne

private:
    TypeForme type;
    
    float x1, y1, x2, y2, x3, y3; // Pour le triangle
    float xC, yC, xR, yR, width, height, rayon; // Pour le cercle et le rectangle
    float xL, yL; // Pour les lignes
    float xE, yE, wE, hE; // Pour les ellipses
    float xB1, xB2, yB1, yB2; // Pour les beziers 
    bool containsPointTriangle, containsPointCercle, containsPointRectangle;
    vector<ofColor> formeColors = {}; // Contient les couleurs du dessin
    bool formeFill = false; // Indique si l'interieur de la forme doit etre colore
    float formeStroke; // Taille du outline
    
};

