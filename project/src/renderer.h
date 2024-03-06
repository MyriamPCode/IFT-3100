#pragma once

#include "ofMain.h"
#include "Interface.h"
#include "Forme.h"
#include <memory>

using namespace std;

class Renderer {
	public:
		Interface interface;
		ofImage image;

		list<ofImage> imageList; //Liste d'images import�es
		list<vector<int>> imgPosList; //Positions x et y des images import�es

    int mouse_press_x;
    int mouse_press_y;
    
    int mouse_current_x;
    int mouse_current_y;

    int mouse_drag_x;
    int mouse_drag_y;

    bool is_mouse_button_pressed;
    bool is_mouse_button_dragged;
    bool import_activate;

    void setup();
    void setup(vector<Forme*>& v_formes);
    void draw();
    void newImage(string filePath, int posX, int posY);
    void draw_cursor(float x, float y) const;

    Forme forme; 
    vector<unique_ptr<Forme>>* v_formes_ptr;
    vector<unique_ptr<Forme>> v_formes;
    void setup(vector<unique_ptr<Forme>>& v_formes);
    bool okDessiner; 
    void dessinerTriangle();
    void dessinerCercle(); 
    void dessinerRectangle(); 
    void dessinerLigne(); 
    void dessinerEllipse(); 
    void dessinerBezier();
    vector<unique_ptr<Forme>>* getVecteurFormesPtr() {
        return &v_formes;
    }


    ofPolyline ligne;
    vector<ofPolyline> vecteur_lignes;

    //Capture images
    void update();
    void image_export(const string name, const string extension) const;
    void captureImage();
    int frameCounter, captureInterval, compteur;
    bool isRecording;
};
