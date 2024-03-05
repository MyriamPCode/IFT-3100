#pragma once

#include "ofMain.h"
#include <vector>

class Interface {
	public:
		vector<ofImage> iconTopBar;

		void setup();
		void draw();

	private:
		ofTrueTypeFont buttonFont;

		ofImage imgImport;
		ofImage imgExport;

		ofColor backgroundLineColor = ofColor(217, 217, 217);
		ofColor backgroundInteractionColor = ofColor(4, 3, 77);
		ofColor backgroundInteractionBorderColor = ofColor(215, 215, 224);

		ofColor textColor = backgroundLineColor;

		void backgroundLine();
		void backgroundInteraction();
		void topButtons();
};

