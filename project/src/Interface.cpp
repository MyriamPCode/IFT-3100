#include "Interface.h"
#include "Constants.h"

using namespace std;

void Interface::setup() {
	buttonFont.load("roboto/Roboto-Regular.ttf", 24);
}

void Interface::draw() {
	backgroundLine();
	backgroundInteraction();
	importButton();
}

void Interface::importButton() {
	string buttonText = "Import";
	ofRectangle textbox = buttonFont.getStringBoundingBox(buttonText, 0, 0);
	if (IMPORT_BUTTON_WIDTH != textbox.getWidth() + 9) {
		IMPORT_BUTTON_WIDTH = textbox.getWidth() + 9;
	}

	ofSetColor(backgroundInteractionBorderColor);
	ofDrawRectangle(0, 0, IMPORT_BUTTON_WIDTH, 40);
	ofSetColor(textColor);

	buttonFont.drawString(buttonText, 3, 30);
}

void Interface::backgroundLine() {
	ofSetLineWidth(1);
	ofSetColor(backgroundLineColor);
	for (int i = 0; i < WIDTH; i += WIDTH / 40) {
		ofDrawLine(i, 0, i, HEIGHT);
		ofDrawLine(0, i, WIDTH, i);
	};

	for (int i = 0; i < HEIGHT; i += WIDTH / 40) {
		ofDrawLine(0, i, WIDTH, i);
	};
}

void Interface::backgroundInteraction() {
	// Right rectangle
	ofSetColor(backgroundInteractionColor);
	ofFill();
	ofDrawRectangle(WIDTH - 40, 0, 40, HEIGHT);

	ofSetColor(backgroundInteractionBorderColor);
	ofNoFill();
	ofDrawRectangle(WIDTH - 40, 0, 40, HEIGHT);

	//Top rectangle
	ofSetColor(backgroundInteractionColor);
	ofFill();
	ofDrawRectangle(0, 0, WIDTH, 40);

	ofSetColor(backgroundInteractionBorderColor);
	ofNoFill();
	ofDrawRectangle(0, 0, WIDTH, 40);
}
