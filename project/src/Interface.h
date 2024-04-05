#pragma once

#include "ofMain.h"
#include <vector>
#include "ofxGui.h"
#include "ofxButton.h"
//#include "Renderer.h"
#include <memory>
// D�claration anticip�e de la classe Renderer

using namespace std;

enum class MeshRenderMode { fill, wireframe, vertex };

class Interface {
	public:
		vector<ofImage> iconTopBar;
		vector<ofImage> iconSideBar;
		
		ofParameter<ofColor> color_picker_background;
		ofParameter<ofColor> color_picker_stroke;
		ofxToggle fillButton;
		ofParameter<ofColor> colorPickerFill;
		ofParameter<float> slider_stroke_weight;
		bool fillEnabled = false;

		string camera_name;

		ofxToggle orthoActivate;
		ofxToggle angleActivate;
		bool orthoIsActive = true;
		bool angleIsActive = false;
		ofxGuiGroup orthoOptions;
		ofxToggle renderAsOrtho;
		ofxToggle renderAsPersp;
		bool orthoRendering = true;
		bool perspRendering = false;
		ofxGuiGroup angleOptions;
		ofxToggle frontCam;
		ofxToggle backCam;
		ofxToggle leftCam;
		ofxToggle rightCam;
		ofxToggle topCam;
		ofxToggle bottomCam;
		bool frontCamRendering = true;
		bool backCamRendering = false;
		bool leftCamRendering = false;
		bool rightCamRendering = false;
		bool topCamRendering = false;
		bool bottomCamRendering = false;

		int mouse_press_x;
		int mouse_press_y;

		int mouse_current_x;
		int mouse_current_y;

		int mouse_drag_x;
		int mouse_drag_y;

		bool is_mouse_button_pressed;
		bool is_mouse_button_dragged;
		bool import_activate;
		bool export_activate;

		ofxToggle modelToggle;
		ofxToggle fillRender;
		ofxToggle wireframeRender;
		ofxToggle pointRender;

		bool mesh_activate;
		bool noise_activate;
		bool hermite_activate;

		void setup();
		void draw();
		void drawBackground();
		void toggleColorWheel();
		void toggleModelOptions();
		void setRenderType(MeshRenderMode renderMode) { typeRender = renderMode; }
		MeshRenderMode getRenderType() { return typeRender; }
		bool getShowModel() { return showModels; }
		void backgroundLine();
		void toggleCamOptions();
		void toggleMailleGUI();
		void toggleCurveOptions();

		/*
		ofVec3f ctrl_point1;
		ofVec3f ctrl_point2;
		ofVec3f ctrl_point3;
		ofVec3f ctrl_point4;

		ofVec3f initial_position1;
		ofVec3f initial_position2;
		ofVec3f initial_position3;
		ofVec3f initial_position4;
		ofVec3f initial_position5;

		ofVec3f position;

		ofVec3f tangent1;
		ofVec3f tangent2;

		int framebuffer_width;
		int framebuffer_height;

		int line_resolution;
		float line_width_outline;
		float line_width_curve;
		float radius;
		ofPolyline line_renderer;
		int index;*/

		//void hermite(float t, float p1x, float p1y, float p1z, float p2x, float p2y, float p2z, float p3x, float p3y, float p3z, float p4x, float p4y, float p4z, float& x, float& y, float& z);
		//void button_hermite(bool& value);

	private:
		bool outilsPressed = false;
		bool camPressed = false;
		bool curvePressed = false;
		
		bool modelsPressed = false;
		bool showModels = false;
		MeshRenderMode typeRender;

		ofTrueTypeFont buttonFont;

		ofImage imgImport;
		ofImage imgExport;
		ofImage imgColorWheel;
		ofImage imgPen;
		ofImage imgCircle;
		ofImage imgRectangle;
		ofImage imgTriangle;
		ofImage imgAnimation;
		ofImage imgModele;
		ofImage imgEllipse;
		ofImage imgMesh;
		ofImage imgCamera;
		ofImage imgCurve;

		ofColor backgroundLineColor = ofColor(217, 217, 217);
		ofColor backgroundInteractionColor = ofColor(4, 3, 77);
		ofColor backgroundInteractionBorderColor = ofColor(215, 215, 224);

		ofColor textColor = backgroundLineColor;

		ofxPanel outilsGui;
		ofxPanel modelsGui;
		ofxPanel cameraGui;
		ofxPanel mailleGui;
		//ofxPanel curveGui;

		bool mailleVisible = false;
		ofParameter<bool> meshButton = false;
		ofParameter<bool> meshAnimationButton = false;

		//ofxGuiGroup hermiteGroupe;
		//ofParameter<bool> hermiteButton = false;

		void backgroundInteraction();
		void topButtons();
		void sideButtons();
		void draw_cursor(float x, float y) const;

		void panelScene(); 
		ofColor backgroundPanelSceneColor = ofColor(125);
		void setupCameraOptions();
		void setupOutilsGuiOptions();
		void setupModelOptions();
		void setupMeshOptions();
		//void setupCurveOptions();
		//Renderer& renderer;
		void showOutils(bool& value);
		void showMaille();
		void enableFill(bool& value);
		void enableOrtho(bool& value);
		void enableAngle(bool& value);
		void orthoSelect(bool& value);
		void perspSelect(bool& value);
		void frontCamSelect(bool& value);
		void backCamSelect(bool& value);
		void leftCamSelect(bool& value);
		void rightCamSelect(bool& value);
		void topCamSelect(bool& value);
		void bottomCamSelect(bool& value);
		void enableModels(bool& value);
		void modelFill(bool& value);
		void modelWireframe(bool& value);
		void modelPoints(bool& value);
};


