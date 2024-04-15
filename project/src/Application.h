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


		ofxPanel gui;

		ofxPanel drawingGUI;
		ofParameter <ofVec2f> uiPosition;
		ofParameter <int> uiAmount;
		ofParameter <ofVec2f> uiStep;
		ofParameter <ofVec3f> uiRotate;
		ofParameter <ofVec2f> uiShift;
		ofParameter <ofVec2f> uiSize;

		ofxGuiGroup primitivesGroupe; // Cr�er un groupe pour les boutons
		ofxToggle toggleDrawTriangle; // Bouton pour le triangle 
		ofxToggle toggleDrawCircle; // Bouton pour le cercle
		ofxToggle toggleDrawRectangle; // Bouton pour le rectangle
		ofxToggle toggleDrawLine; // Bouton pour le triangle 
		ofxToggle toggleDrawEllipse; // Bouton pour le cercle
		ofxToggle toggleDrawBezier; // Bouton pour le rectangle
		ofxToggle drawSphere;
		ofxToggle drawCube;


		ofxGuiGroup reinitialisationGroupe;
		ofxToggle resetButton; // Bouton de r�initialisation


		ofxGuiGroup primitivesMode;
		ofxToggle toggleDraw, toggleTransform; //Boutons toggle pour les modes draw & transform
		ofxIntField indexBox; 

		void button_triangle(bool & value);
		void button_circle(bool& value);
		void button_rectangle(bool& value);
		void button_line(bool& value);
		void button_ellipse(bool& value);
		void button_bezier(bool& value);
		void button_modeDraw(bool& value);
		void button_modeTransform(bool& value);
		void button_sphere(bool& value); 
		void button_cube(bool& value);


		ofxGuiGroup animationGroupe;
		ofxToggle rotationButton; // Bouton pour l'animation de rotation

		ofxGuiGroup meshGroupe;
		ofxToggle meshButton;
		ofxToggle meshAnimationButton; // Bouton pour l'animation sur la maille géométrique 

		ofxPanel filterGUI;
		ofxGuiGroup textureGroupe;
		ofxToggle sphereTextureButton;

		ofxPanel curveGui;
		ofxGuiGroup catmullRomGroupe;
		ofxGuiGroup hermiteGroupe;
		ofParameter<bool> catmullRomButton = false;
		ofParameter<bool> catmullRomButton6 = false;
		ofParameter<bool> hermiteButton = false;
		void button_catmullRom(bool& value);
		void button_catmullRom6(bool& value);
		void button_hermite(bool& value);
		void hermite(float t, const ofVec2f& p0, const ofVec2f& p1, const ofVec2f& p2, const ofVec2f& p3, const ofVec2f& p4, float& x, float& y);

		vector<ofVec2f> controlPoints;
		ofParameter<int> segments;
		int selectedPointIndex;
		ofVec2f Application::catmullRom(float t, const ofVec2f& p0, const ofVec2f& p1, const ofVec2f& p2, const ofVec2f& p3, const ofVec2f& p4);
		ofVec2f catmullRom6(float t, const ofVec2f& p0, const ofVec2f& p1, const ofVec2f& p2, const ofVec2f& p3, const ofVec2f& p4, const ofVec2f& p5);

		bool drawingGUIPressed;
		bool draw_triangle;
		bool draw_circle;
		bool draw_rectangle;
		bool draw_line, draw_ellipse, draw_bezier;
		bool rotation_activate;
		bool mesh_activate;
		bool noise_activate;
		bool draw_sphere, draw_cube;
		bool draw_sphereTexture;
		bool catmullRom_activate;
		bool catmullRom6_activate;
		bool hermite_activate;

		float rotate;

		float diffX, diffY, newX2, newY2, newX3, newY3;

		void showButtonsList(); 
		ofColor backgroundPanelSceneColor = ofColor(125);

		vector<unique_ptr<ofxToggle>> v_buttons;
		vector<unique_ptr<ofxToggle>>* v_buttons_ptr;
		void deleteShapeSelected();

		void addAction(function<void()> undoAction, function<void()> redoAction) {
			undoStack.push(move(undoAction));
			// Effacer la pile Redo car les actions précédentes ne sont plus valides
			while (!redoStack.empty()) {
				redoStack.pop();
			}
		}

		void undo() {
			if (!undoStack.empty()) {
				function<void()> undoAction = move(undoStack.top());
				undoStack.pop();
				undoAction();
				redoStack.push(move(undoAction));
			}
		}

		void redo() {
			if (!redoStack.empty()) {
				function<void()> redoAction = move(redoStack.top());
				redoStack.pop();
				redoAction();
				undoStack.push(move(redoAction));
			}
		}

private:
	stack<function<void()>> undoStack;
	stack<function<void()>> redoStack;

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
  		
  Forme::TypeForme lastShape;

		ofxPanel guiScene;
		ofParameter<string> circle = "CIRCLE";
		ofParameter<string> triangle = "TRIANGLE";
		ofParameter<string> ellipse = "ELLIPSE";
		ofParameter<string> line = "LINE";
		ofParameter<string> bezier = "BEZIER";
		ofParameter<string> rectangle = "RECTANGLE";
		ofParameter<string> sphere = "SPHERE";
		ofParameter<string> cube = "CUBE";

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
		void button_sphereTexture(bool& value);



};
