#include "Interface.h"
#include "Constants.h"

using namespace std;

void Interface::setup() {
	buttonFont.load("roboto/Roboto-Regular.ttf", 24);

	ofxToggle button;
	button.setup("Outils de Dessin", false, 175, 39);
	button.loadFont("roboto/Roboto-Regular.ttf", 16);
	button.setBackgroundColor(backgroundInteractionColor);
	button.setPosition(103, 0);
	button.addListener(this, &Interface::showOutils);
	buttonList.push_back(button);

	outilsGui.setup();
	outilsGui.loadFont("roboto/Roboto-Regular.ttf", 10);
	outilsGui.setPosition(103, 40);
	outilsGui.setSize(200, 1080);

	color_picker_background.set("Couleur du canevas", ofColor(31), ofColor(0, 0), ofColor(255, 255));
	color_picker_stroke.set("Couleur de la ligne de contour", ofColor(255), ofColor(0, 0), ofColor(255, 255));
	colorPickerFill.set("Couleur de remplissage", ofColor(255), ofColor(0, 0), ofColor(255, 255));
	slider_stroke_weight.set("Epaisseur de la ligne de contour", 4.0f, 0.0f, 10.0f);

	outilsGui.add(color_picker_background);
	outilsGui.add(color_picker_stroke);
	outilsGui.add(colorPickerFill);
	outilsGui.add(slider_stroke_weight);

	outilsGui.setFillColor(backgroundInteractionColor);
	outilsGui.setTextColor(backgroundInteractionColor);
	outilsGui.maximizeAll();
	outilsGui.disableHeader();
}

void Interface::draw() {
	backgroundLine();
	backgroundInteraction();
	importButton();
	for (list<ofxToggle>::iterator iter = buttonList.begin(); iter != buttonList.end(); ++iter) {
		iter->draw();
	}
	if (outilsPressed) {
		outilsGui.draw();
	}
}

void Interface::showOutils(bool& value) {
	outilsPressed = !outilsPressed;
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
