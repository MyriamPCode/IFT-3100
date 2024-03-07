#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "Renderer.h"
#include "Forme.h"

using namespace std;

class Application : public ofBaseApp{

	public:
		Renderer renderer;
		Forme forme;
		ofColor backgroundColor = ofColor(178, 184, 194);

		bool isImportable = false; //Indique si le mode d'importation est actif ou non
		int imgDistFromMax = 0; //Indicateur de la position de l'image par rapport � la taille maximale de la liste
		bool isRepositioning = false; //Indique si un image est en cours de repositionnement

		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		void reset(bool& value);

		ofxPanel gui;

		ofParameter <int> uiAmount; // Total de la liste formes 

		ofxGuiGroup primitivesGroupe; // Cr�er un groupe pour les boutons
		ofxToggle drawTriangle; // Bouton pour le triangle 
		ofxToggle drawCircle; // Bouton pour le cercle
		ofxToggle drawRectangle; // Bouton pour le rectangle
		ofxToggle drawLine; // Bouton pour le triangle 
		ofxToggle drawEllipse; // Bouton pour le cercle
		ofxToggle drawBezier; // Bouton pour le rectangle

		ofxGuiGroup reinitialisationGroupe;
		ofxToggle resetButton; // Bouton de r�initialisation

		ofxGuiGroup primitivesMode;
		ofxToggle toggleDraw, toggleTransform; //Boutons toggle pour les modes draw & transform
		ofxIntField indexBox; 

		bool draw_triangle;
		bool draw_circle;
		bool draw_rectangle;
		bool draw_line, draw_ellipse, draw_bezier; 

		void button_triangle(bool & value);
		void button_circle(bool& value);
		void button_rectangle(bool& value);
		void button_line(bool& value);
		void button_ellipse(bool& value);
		void button_bezier(bool& value);
		void button_modeDraw(bool& value);
		void button_modeTransform(bool& value);

		float diffX, diffY, newX2, newY2, newX3, newY3;

	
		void showButtonsList(); 
		ofColor backgroundPanelSceneColor = ofColor(125);
		ofxPanel guiScene; 
		vector<unique_ptr<ofxToggle>> v_buttons;
		vector<unique_ptr<ofxToggle>>* v_buttons_ptr;
		void deleteShapeSelected();
		bool shapeBool;
};
