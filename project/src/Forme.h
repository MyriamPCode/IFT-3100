#pragma once

#include "ofMain.h"
#include <vector>
#include <string>

// Déclaration de la classe Forme
class Forme {
public:
    enum TypeForme
    {
        TRIANGLE, CERCLE, RECTANGLE, LIGNE, ELLIPSE, BEZIER, SPHERE
    };

    Forme(TypeForme type, float x1, float y1, float x2, float y2, float x3, float y3); // Triangle
    Forme(TypeForme type, float x, float y, float rayon); // Cercle
    Forme(TypeForme type, float x, float y, float width, float height); // Rectangle et Ellipse
    Forme(TypeForme type, float x1, float y1, float xB1, float yB1, float xB2, float yB2, float x2, float y2); // Bezier 
    Forme(TypeForme type, ofVec3f p); // Lignes
    Forme(TypeForme type, ofVec3f point1, ofVec3f point2, ofVec3f point3); // Triangle avec pts 3d 
   // Forme(TypeForme type, float x, float y, float z, float radius); 
    Forme(TypeForme type, ofVec3f v, float rad);


    ofVec3f getPoint1() const { return point1; }
    ofVec3f getPoint2() const { return point2; }
    ofVec3f getPoint3() const { return point3; }
    void setPoint1(ofVec3f pt) { this->point1 = pt; }
    void setPoint2(ofVec3f pt) { this->point2 = pt; }
    void setPoint3(ofVec3f pt) { this->point3 = pt; }


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
    //float getXE() const { return xE; }
    //float getYE() const { return yE; }
    //float getWidthEllipse() const { return wE; }
    //float getHeightEllipse() const { return hE; }
    float getXS() const { return xS; }
    float getYS() const { return yS; }
    float getZS() const { return zS; }
    float getSphereRad() const{ return sphereRad; }
    ofVec3f getVSphere() const { return v; }
 
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
    //void setXE(float x)  { this->xE = x; }
    //void setYE(float y)  { this->yE = y; }
    //void setWidthEllipse(float w)  { this->wE = w; }
    //void setHeightEllipse(float h) { this->hE = h; }
    void setXS(float x) { this->xS = x; }
    void setYS(float x) { this->yS = x; }
    void setZS(float x) { this->zS = x; }
    void setSphereRad(float x) { this->sphereRad = x; }
    void setVSphere(ofVec3f v) { this->v = v; }

    TypeForme getType() const { return type; }


private:
    TypeForme type;
    
    float x1, y1, x2, y2, x3, y3; // Pour le triangle
    float xC, yC, xR, yR, width, height, rayon; // Pour le cercle et le rectangle
    float xL, yL; // Pour les lignes
    float xE, yE, wE, hE; // Pour les ellipses
    float xB1, xB2, yB1, yB2; // Pour les beziers 
    float xS, yS, zS, sphereRad; // sphere 3d 
    ofVec3f point1, point2, point3, p, v;
};

