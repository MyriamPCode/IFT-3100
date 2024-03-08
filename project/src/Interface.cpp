#include "Interface.h"
#include "Constants.h"
#include <vector>

using namespace std;

void Interface::setup() {
	//buttonFont.load("roboto/Roboto-Regular.ttf", 24);

	//we only need to add the images of the buttons here to add it to the top bar
	imgImport.load("img/import.png");
	iconTopBar.push_back(imgImport);
	imgExport.load("img/export.png");
	iconTopBar.push_back(imgExport);
	imgAnimation.load("img/animation.png");
	iconTopBar.push_back(imgAnimation);

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

	outilsGui.setup();
	outilsGui.loadFont("roboto/Roboto-Regular.ttf", 10);
	outilsGui.setPosition(WIDTH - outilsGui.getWidth() - INTERACTION_BAR_HEIGHT, INTERACTION_BAR_HEIGHT);
	outilsGui.setSize(200, 1080);

	color_picker_background.set("Couleur du canevas", ofColor(31), ofColor(0, 0), ofColor(255, 255));
	color_picker_stroke.set("Couleur de la ligne de contour", ofColor(255), ofColor(0, 0), ofColor(255, 255));
	colorPickerFill.set("Couleur de remplissage", ofColor(255), ofColor(0, 0), ofColor(255, 255));
	slider_stroke_weight.set("Epaisseur de la ligne de contour", 4.0f, 0.0f, 10.0f);

	outilsGui.add(color_picker_background);
	outilsGui.add(color_picker_stroke);
	outilsGui.add(fillButton.setup("Remplissage de forme", false));
	outilsGui.add(colorPickerFill);
	outilsGui.add(slider_stroke_weight);

	fillButton.addListener(this, &Interface::enableFill);

	outilsGui.setFillColor(backgroundInteractionColor);
	outilsGui.setTextColor(backgroundInteractionColor);
	outilsGui.maximizeAll();
	outilsGui.disableHeader();

	is_mouse_button_pressed = false;
	is_mouse_button_dragged = false;
	import_activate = false;

	mouse_current_x = mouse_current_y = mouse_press_x = mouse_press_y = mouse_drag_x = mouse_drag_y = 0;
}

void Interface::draw() {
	backgroundLine();
	backgroundInteraction();
	topButtons();
	sideButtons();
	draw_cursor(mouse_current_x, mouse_current_y);

	if (outilsPressed) {
		outilsGui.draw();
	}
	int gridSize = 50; // Espacement de la grille
	// Obtenir les coordonnées de la souris
	int mouseX = ofGetMouseX();
	int mouseY = ofGetMouseY();

	// Convertir les coordonnées de la souris dans l'espace de la grille
	int gridX = mouseX / gridSize;
	int gridY = mouseY / gridSize;

	// Dessiner les coordonnées de la souris sur la grille
	ofSetColor(255); // Couleur blanc
	ofDrawBitmapString("Mouse X: " + ofToString(mouseX) + ", Mouse Y: " + ofToString(mouseY), 1630, 65);
	ofDrawBitmapString("Grid X: " + ofToString(gridX) + ", Grid Y: " + ofToString(gridY), 1630, 85);
	ofDrawBitmapString("Current camera: " + camera_name, 1630, 105);

}

void Interface::draw_cursor(float x, float y) const {
	float length = 10.0f;
	float offset = 5.0f;

	ofSetLineWidth(2);

	if (is_mouse_button_dragged) {
		ofSetColor(254, 142, 118); // Couleur rouge
		ofDrawCircle(x, y, 20);
	}

	else if (is_mouse_button_pressed) {
		ofSetColor(249, 220, 40); // Couleur jaune
		ofDrawRectangle(x - 20, y - 20, 40, 40);
	}
	else if (import_activate) {
		ofSetColor(135, 210, 88); // Couleur verte
		float tailleTriangle = 20;
		ofDrawTriangle(x, y - tailleTriangle, x - tailleTriangle, y + tailleTriangle, x + tailleTriangle, y + tailleTriangle);
	}

	else
		ofSetColor(255); // Couleur blanche


	ofDrawLine(x + offset, y, x + offset + length, y);
	ofDrawLine(x - offset, y, x - offset - length, y);
	ofDrawLine(x, y + offset, x, y + offset + length);
	ofDrawLine(x, y - offset, x, y - offset - length);
}

void Interface::topButtons() {
	for (int i = 0; i < iconTopBar.size(); i++) {
		if (i == 0) {
			iconTopBar[i].draw(0, 0, iconWidth, iconWidth);
		}
		else {
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

void Interface::showOutils(bool& value) {
	outilsPressed = !outilsPressed;
}

void Interface::toggleColorWheel() {
	outilsPressed = !outilsPressed;
}

void Interface::enableFill(bool& value) {
	fillEnabled = !fillEnabled;
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
	//int numSegments = 8; // Nombre de segments � dessiner
	//float segmentHeight = (HEIGHT - 40) / numSegments; // Hauteur de chaque segment

	//for (int i = 1; i < numSegments; ++i) {
	//	// Dessiner une ligne horizontale pour segmenter la zone
	//	ofDrawLine(0, 41 + i * segmentHeight, 255, 41 + i * segmentHeight);
	//}

}