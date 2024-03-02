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

	// chargement du modèle 3D en mémoire
	teapot.loadModel("teapot.obj");
	bunny.loadModel("bun_zipper.ply");

	typeRender = MeshRenderMode::wireframe;

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

	teapot.setPosition(1350,1000,0);
	bunny.setPosition(400, 1300, 0);

	if (typeRender == MeshRenderMode::wireframe) {
		teapot.draw(OF_MESH_WIREFRAME);
		bunny.draw(OF_MESH_WIREFRAME);
	}
	else if (typeRender == MeshRenderMode::fill) {
		teapot.draw(OF_MESH_FILL);
		bunny.draw(OF_MESH_FILL);
	}
	else if (typeRender == MeshRenderMode::vertex) {
		teapot.draw(OF_MESH_POINTS);
		bunny.draw(OF_MESH_POINTS);
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
	}
	else if (is_mouse_button_pressed) {
		ofSetColor(249, 220, 40); // Couleur jaune
	}
	else if (import_activate) {
		ofSetColor(135, 210, 88); // Couleur verte
	}
	else
		ofSetColor(255); // Couleur blanche

	ofDrawLine(x + offset, y, x + offset + length, y);
	ofDrawLine(x - offset, y, x - offset - length, y);
	ofDrawLine(x, y + offset, x, y + offset + length);
	ofDrawLine(x, y - offset, x, y - offset - length);

}
