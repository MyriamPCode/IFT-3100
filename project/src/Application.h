#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "Renderer.h"

using namespace std;

class Application : public ofBaseApp{

	public:
		Renderer renderer;

		ofColor backgroundColor = ofColor(178, 184, 194);

		bool isImportable = false; //Indique si le mode d'importation est actif ou non
		int imgDistFromMax = 0; //Indicateur de la position de l'image par rapport à la taille maximale de la liste
		bool isRepositioning = false; //Indique si un image est en cours de repositionnement

		ofColor stroke_color = renderer.interface.color_picker_stroke;
		ofColor fillColor = renderer.interface.colorPickerFill;
		float stroke_weight = renderer.interface.slider_stroke_weight;

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
		ofParameter <ofVec2f> uiPosition;
		ofParameter <int> uiAmount;
		ofParameter <ofVec2f> uiStep;
		ofParameter <ofVec3f> uiRotate;
		ofParameter <ofVec2f> uiShift;
		ofParameter <ofVec2f> uiSize;

		ofxGuiGroup primitivesGroupe; // Créer un groupe pour les boutons
		ofxToggle drawTriangle; // Bouton pour le triangle 
		ofxToggle drawCircle; // Bouton pour le cercle
		ofxToggle drawRectangle; // Bouton pour le rectangle

		ofxGuiGroup reinitialisationGroupe;
		ofxToggle resetButton; // Bouton de réinitialisation

		bool draw_triangle;
		bool draw_circle;
		bool draw_rectangle;

		void button_triangle(bool & value);
		void button_circle(bool& value);
		void button_rectangle(bool& value);
};
