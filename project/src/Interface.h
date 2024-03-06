#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxButton.h"
//#include "Renderer.h"
#include <memory>
// Déclaration anticipée de la classe Renderer

using namespace std;

class Interface {
	public:
		void setup();
		void draw();

		ofParameter<ofColor> color_picker_background;
		ofParameter<ofColor> color_picker_stroke;
		ofxToggle fillButton;
		ofParameter<ofColor> colorPickerFill;
		ofParameter<float> slider_stroke_weight;
		bool fillEnabled = false;

	private:
		bool outilsPressed = false;
		ofTrueTypeFont buttonFont;

		ofColor backgroundLineColor = ofColor(217, 217, 217);
		ofColor backgroundInteractionColor = ofColor(4, 3, 77);
		ofColor backgroundInteractionBorderColor = ofColor(215, 215, 224);

		ofColor textColor = backgroundLineColor;

		list<ofxToggle> buttonList;

		ofxPanel outilsGui;

		void backgroundLine();
		void backgroundInteraction();
		void importButton();

		void panelScene(); 
		ofColor backgroundPanelSceneColor = ofColor(125);
		//Renderer& renderer;
		void showOutils(bool& value);
		void enableFill(bool& value);
};

//class Renderer;

