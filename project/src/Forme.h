#pragma once

#include "ofMain.h"
#include <vector>

// Déclaration de la classe Forme
class Forme {
public:
    enum TypeForme
    {
        TRIANGLE, CERCLE, RECTANGLE
    };

    Forme(TypeForme type, float x1, float y1, float x2, float y2, float x3, float y3);
    Forme(TypeForme type, float x, float y, float rayon);
    Forme(TypeForme type, float x, float y, float width, float height);

    bool containsPoint(float x, float y);

    void draw();
    void setup();

    // Constructeur
    Forme();

    // Destructeur virtuel
    virtual ~Forme();

    float getX1() const;
    float getY1() const;
    float getX2() const;
    float getY2() const;
    float getX3() const;
    float getY3() const;
    float getXC() const;
    float getYC() const;
    float getXR() const;
    float getYR() const;
    float getWidth() const; 
    float getHeight() const; 
    float getRayon() const; 

    void setX1(float x);
    void setY1(float y);
    void setX2(float x);
    void setY2(float y);
    void setX3(float x);
    void setY3(float y);
    void setXC(float x);
    void setYC(float y);
    void setXR(float x);
    void setYR(float y);
    void setWidth(float w);
    void setHeight(float h);
    void setRayon(float r);

    TypeForme getType() const { return type; }

    vector<Forme*> v_formes; // Vecteur de pointeurs vers des formes

private:
    TypeForme type;
    float x1, y1, x2, y2, x3, y3; // Pour le triangle
    float xC, yC, xR, yR, width, height, rayon; // Pour le cercle et le rectangle
   
    //std::string forme; // Attribut spécifique à chaque forme
    bool containsPointTriangle, containsPointCercle, containsPointRectangle;
    
};

