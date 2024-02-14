#pragma once

#include "ofMain.h"

class Interface {
public:
	void setup();
	void draw();


private:
	ofColor backgroundLineColor = ofColor(217, 217, 217);
	ofColor backgroundInteractionColor = ofColor(26, 26, 26);
	ofColor backgroundInteractionBorderColor = ofColor(64, 64, 64);

	void backgroundLine();
	void backgroundInteraction();
};

