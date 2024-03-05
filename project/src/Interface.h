#pragma once

#include "ofMain.h"
#include <vector>

class Interface {
	public:
		vector<ofImage> iconTopBar;
		vector<ofImage> iconSideBar;

		void setup();
		void draw();

	private:
		ofTrueTypeFont buttonFont;

		ofImage imgImport;
		ofImage imgExport;
		ofImage imgColorWheel;
		ofImage imgPen;
		ofImage imgCircle;
		ofImage imgRectangle;
		ofImage imgTriangle;

		ofColor backgroundLineColor = ofColor(217, 217, 217);
		ofColor backgroundInteractionColor = ofColor(4, 3, 77);
		ofColor backgroundInteractionBorderColor = ofColor(215, 215, 224);

		ofColor textColor = backgroundLineColor;

		void backgroundLine();
		void backgroundInteraction();
		void topButtons();
		void sideButtons();
};

