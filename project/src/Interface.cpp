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
	//panelScene();
}

//Interface::Interface(Renderer renderer) : renderer(renderer) {}


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

//	// Dessiner les lignes verticales
//	for (int i = 255; i < WIDTH; i += WIDTH / 40) {
//		ofDrawLine(i, 0, i, HEIGHT);
//	}
//
//	// Dessiner les lignes horizontales
//	for (int i = 40; i < HEIGHT; i += WIDTH / 40) {
//		ofDrawLine(0, i, WIDTH, i);
//	}
}

void Interface::backgroundInteraction() {

	//Right rectangle
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

void Interface::panelScene()
{
	ofSetColor(backgroundPanelSceneColor);
	ofFill();
	ofDrawRectangle(0, 41, 255, HEIGHT - 40);

	ofSetColor(backgroundInteractionBorderColor);
	ofNoFill();

	// Dessiner les lignes de segmentation dans le panneau
	//int numSegments = 8; // Nombre de segments à dessiner
	//float segmentHeight = (HEIGHT - 40) / numSegments; // Hauteur de chaque segment

	//for (int i = 1; i < numSegments; ++i) {
	//	// Dessiner une ligne horizontale pour segmenter la zone
	//	ofDrawLine(0, 41 + i * segmentHeight, 255, 41 + i * segmentHeight);
	//}

}