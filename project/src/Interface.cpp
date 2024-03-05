#include "Interface.h"
#include "Constants.h"
#include <vector>

using namespace std;

void Interface::setup() {
	buttonFont.load("roboto/Roboto-Regular.ttf", 24);

	//we only need to add the images of the buttons here to add it to the top bar
	imgImport.load("img/import.png");
	iconTopBar.push_back(imgImport);
	imgExport.load("img/export.png");
	iconTopBar.push_back(imgExport);

	//same here but for the side bar
	imgColorWheel.load("img/color-wheel.png");
	iconSideBar.push_back(imgColorWheel);
	imgPen.load("img/pen.png");
	iconSideBar.push_back(imgPen);
	imgCircle.load("img/circle.png");
	iconSideBar.push_back(imgCircle);
	imgRectangle.load("img/rectangle.png");
	iconSideBar.push_back(imgRectangle);
	imgTriangle.load("img/triangle.png");
	iconSideBar.push_back(imgTriangle);
}

void Interface::draw() {
	backgroundLine();
	backgroundInteraction();
	topButtons();
	sideButtons();
}

void Interface::topButtons() {
	for (int i = 0; i < iconTopBar.size(); i++) {
		if (i == 0) {
			iconTopBar[i].draw(0, 0, iconWidth, iconWidth);
		} else {
			iconTopBar[i].draw(10 + iconWidth * i, 0, iconWidth, iconWidth);
		}
	}
}

void Interface::sideButtons() {
	for (int i = 0; i < iconSideBar.size(); i++) {
		if (i == 0) {
			iconSideBar[i].draw(WIDTH - iconWidth, INTERACTION_BAR_HEIGHT, iconWidth, iconWidth);
		}
		else {
			iconSideBar[i].draw(WIDTH - iconWidth, INTERACTION_BAR_HEIGHT + iconWidth * i, iconWidth, iconWidth);
		}
	}
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
