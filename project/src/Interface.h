#pragma once

#include "ofMain.h"
#include <vector>
#include "ofxGui.h"
#include "ofxButton.h"
//#include "Renderer.h"
#include <memory>
// D�claration anticip�e de la classe Renderer

using namespace std;

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

		void setup();
		void draw();
		void drawBackground();
		void toggleColorWheel();
		void Interface::toggleCamOptions();

	private:
		bool outilsPressed = false;
		bool camPressed = false;
		
		ofTrueTypeFont buttonFont;

		ofImage imgImport;
		ofImage imgExport;
		ofImage imgColorWheel;
		ofImage imgPen;
		ofImage imgCircle;
		ofImage imgRectangle;
		ofImage imgTriangle;
		ofImage imgAnimation;
		ofImage imgCamera;

		ofColor backgroundLineColor = ofColor(217, 217, 217);
		ofColor backgroundInteractionColor = ofColor(4, 3, 77);
		ofColor backgroundInteractionBorderColor = ofColor(215, 215, 224);

		ofColor textColor = backgroundLineColor;

		ofxPanel outilsGui;
		ofxPanel cameraGui;

		void backgroundLine();
		void backgroundInteraction();
		void topButtons();
		void sideButtons();
		void draw_cursor(float x, float y) const;

		void panelScene(); 
		ofColor backgroundPanelSceneColor = ofColor(125);
		void setupCameraOptions();
		void setupOutilsGuiOptions();
		//Renderer& renderer;
		void showOutils(bool& value);
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
};


