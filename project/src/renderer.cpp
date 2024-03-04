#include "Renderer.h"
#include "Interface.h"

#include "Constants.h"

using namespace std;

void Renderer::setup() {
	ofSetFrameRate(60);
	interface.setup();

	is_mouse_button_pressed = false;
	is_mouse_button_dragged = false;
	import_activate = false;

	mouse_current_x = mouse_current_y = mouse_press_x = mouse_press_y = mouse_drag_x = mouse_drag_y = 0;
}

void Renderer::draw() {
	interface.draw();

	auto currImg = imgPosList.begin();
	for (list<ofImage>::iterator iter = imageList.begin(); iter != imageList.end(); ++iter) {
		vector temp = *currImg;
		iter->draw(temp[0], temp[1]);
		++currImg;
	}

	draw_cursor(mouse_current_x, mouse_current_y);
}

void Renderer::newImage(string filePath, int posX, int posY) {
	ofImage newImage;
	imageList.push_back(newImage);
	imageList.back().load(filePath);
	imgPosList.push_back({posX, posY});
}

void Renderer::draw_cursor(float x, float y) const {
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
