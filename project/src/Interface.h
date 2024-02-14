#pragma once

#include "ofMain.h"

class Interface {
public:
	void setup();
	void draw();


private:
	ofColor backgroundLineColor = ofColor(217, 217, 217);
	void backgroundLine();
};

