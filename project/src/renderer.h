#pragma once

#include "ofMain.h"
#include "Interface.h"
#include "Forme.h"
#include <memory>
#include "ofxGui.h"

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
        vector<ofColor> triangleColors = {}; // Contient les couleurs du dessin
        bool triangleFill = false; // Indique si l'interieur de la forme doit etre colore
        float triangleStroke; // Taille du outline
        void dessinerCercle(); 
        vector<ofColor> cercleColors = {}; // Contient les couleurs du dessin
        bool cercleFill = false; // Indique si l'interieur de la forme doit etre colore
        float cercleStroke; // Taille du outline
        void dessinerRectangle(); 
        vector<ofColor> rectangleColors = {}; // Contient les couleurs du dessin
        bool rectangleFill = false; // Indique si l'interieur de la forme doit etre colore
        float rectangleStroke; // Taille du outline
        void dessinerLigne(); 
        ofColor ligneColor; // Contient la couleur du dessin
        float ligneStroke; // Taille du outline
        void dessinerEllipse();
        vector<ofColor> ellipseColors = {}; // Contient les couleurs du dessin
        bool ellipseFill = false; // Indique si l'interieur de la forme doit etre colore
        float ellipseStroke; // Taille du outline
        void dessinerBezier();
        vector<ofColor> bezierColors = {}; // Contient les couleurs du dessin
        bool bezierFill = false; // Indique si l'interieur de la forme doit etre colore
        float bezierStroke; // Taille du outline

        vector<unique_ptr<Forme>>* getVecteurFormesPtr() {
            return &v_formes;
        }


        ofPolyline ligne;
        vector<ofPolyline> vecteur_lignes;

        //Capture images
        void update();
        void captureImage();
        int frameCounter, captureInterval, compteur;
        bool isRecording;

        void toggleExportGUI();
        void toggleColorWheelGUI();

    private:
        ofxPanel gui;
        ofParameter<string> nameField;
        ofParameter<bool> visibleButton = true;
        ofParameter<bool> exportButton = false;

        void image_export(const string name, const string extension) const;
};
