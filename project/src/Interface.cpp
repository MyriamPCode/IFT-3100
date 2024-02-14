#include "Interface.h"
#include "Constants.h"

void Interface::setup() {}

void Interface::draw() {
	backgroundLine();
}

void Interface::backgroundLine() {
	ofSetColor(backgroundLineColor);
	for (int i = 0; i < WIDTH; i += WIDTH / 40) {
		ofDrawLine(i, 0, i, HEIGHT);
		ofDrawLine(0, i, WIDTH, i);
	};

	for (int i = 0; i < HEIGHT; i += WIDTH / 40) {
		ofDrawLine(0, i, WIDTH, i);
	};
}
