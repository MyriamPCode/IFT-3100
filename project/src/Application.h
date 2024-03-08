#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "Renderer.h"
#include "Forme.h"

using namespace std;

enum class Camera { front, back, left, right, top, down };

class Application : public ofBaseApp{
	public:
		Renderer renderer;
		Forme forme;
		ofColor backgroundColor = ofColor(178, 184, 194);
		ofMesh mesh;
		int size = 96; // Taille de la maille

		Camera camera_active;

		ofCamera cam;

		ofCamera camFront;
		ofCamera camBack;
		ofCamera camLeft;
		ofCamera camRight;
		ofCamera camTop;
		ofCamera camBottom;
		ofCamera* camera;

		ofQuaternion camera_orientation;
		ofVec3f camera_position;

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
		void reset_cam();

		ofxPanel drawingGUI;
		ofParameter <ofVec2f> uiPosition;
		ofParameter <int> uiAmount;
		ofParameter <ofVec2f> uiStep;
		ofParameter <ofVec3f> uiRotate;
		ofParameter <ofVec2f> uiShift;
		ofParameter <ofVec2f> uiSize;

		//ofxGuiGroup primitivesGroupe; // Cr�er un groupe pour les boutons
		//ofxToggle drawTriangle; // Bouton pour le triangle 
		//ofxToggle drawCircle; // Bouton pour le cercle
		//ofxToggle drawRectangle; // Bouton pour le rectangle
		//ofxToggle drawLine; // Bouton pour le triangle 
		//ofxToggle drawEllipse; // Bouton pour le cercle
		//ofxToggle drawBezier; // Bouton pour le rectangle

		ofxGuiGroup reinitialisationGroupe;
		ofxToggle resetButton; // Bouton de r�initialisation

		ofxGuiGroup animationGroupe;
		ofxToggle rotationButton; // Bouton pour l'animation de rotation

		ofxGuiGroup meshGroupe;
		ofxToggle meshButton;
		ofxToggle meshAnimationButton; // Bouton pour l'animation sur la maille géométrique 

		bool drawingGUIPressed;
		bool draw_triangle;
		bool draw_circle;
		bool draw_rectangle;
		bool draw_line, draw_ellipse, draw_bezier;
		bool rotation_activate;
		bool mesh_activate;
		bool noise_activate;

		float rotate;

		float diffX, diffY, newX2, newY2, newX3, newY3;

		void buttons_list(bool& value);
		ofColor backgroundPanelSceneColor = ofColor(125);

		vector<unique_ptr<ofxToggle>> v_buttons;
		vector<unique_ptr<ofxToggle>>* v_buttons_ptr;
		void deleteShapeSelected();
		bool shapeBool; 

		void camera_setup_perspective(float width, float height, float fov, float n, float f);
		float compute_zoom_from_fov(float fov);
		void setupCamera();
		bool orthoEnabled = false;
		ofVec3f camera_target = { 0.0f, 0.0f, 0.0f };
		float offset_camera;
		bool is_visible_camera;

		bool moveCameraRight;
		bool moveCameraLeft;
		bool moveCameraUp;
		bool moveCameraDown;
		bool moveCameraNear;
		bool moveCameraFar;

	private:
		Forme::TypeForme lastShape;

		ofxPanel guiScene;
		ofParameter<string> circle = "CIRCLE";
		ofParameter<string> triangle = "TRIANGLE";
		ofParameter<string> ellipse = "ELLIPSE";
		ofParameter<string> line = "LINE";
		ofParameter<string> bezier = "BEZIER";
		ofParameter<string> rectangle = "RECTANGLE";

		void toggleDrawingGUI(Forme::TypeForme drawingShape);
		void drawTriangle();
		void drawCircle();
		void drawRectangle();
		void drawLine();
		void drawEllipse();
		void drawBezier();
		void button_rotation(bool& value);
		void button_mesh(bool& value);
		void button_noise(bool& value);
};
