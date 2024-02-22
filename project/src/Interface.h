#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxButton.h"

using namespace std;

class Interface {
	public:
		void setup();
		void draw();

	private:
		bool outilsPressed = false;
		ofTrueTypeFont buttonFont;

		ofColor backgroundLineColor = ofColor(217, 217, 217);
		ofColor backgroundInteractionColor = ofColor(4, 3, 77);
		ofColor backgroundInteractionBorderColor = ofColor(215, 215, 224);

		ofColor textColor = backgroundLineColor;

		list<ofxToggle> buttonList;

		ofxPanel outilsGui;

		ofParameter<ofColor> color_picker_background;
		ofParameter<ofColor> color_picker_stroke;
		ofParameter<ofColor> colorPickerFill;
		ofParameter<float> slider_stroke_weight;

		void backgroundLine();
		void backgroundInteraction();
		void importButton();
		void showOutils(bool& value);
};

