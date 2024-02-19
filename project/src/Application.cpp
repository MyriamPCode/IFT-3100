using namespace std;

#include "Application.h"
#include "Constants.h"

void Application::setup(){
	ofSetWindowTitle("Team 7");
	ofBackground(backgroundColor);
	renderer.setup();

	gui.setup();
	gui.add(uiPosition.set("position", ofVec2f(0), ofVec2f(0), ofVec2f(ofGetWidth(), ofGetHeight()))); // La position des primitives
	gui.add(uiAmount.set("amount", 1, 0, 64)); // La quantité de primitives. Nombre maximal est 64 et nombre minimum est 1
	gui.add(uiStep.set("step", ofVec2f(0), ofVec2f(0), ofVec2f(300)));
	gui.add(uiRotate.set("rotate", ofVec3f(0), ofVec3f(-180), ofVec3f(180))); // La rotation des primitives
	gui.add(uiShift.set("shift", ofVec2f(0), ofVec2f(0), ofVec2f(300)));
	gui.add(uiSize.set("size", ofVec2f(6), ofVec2f(0), ofVec2f(30)));

	draw_triangle = false;
	draw_circle = false;
	draw_rectangle = false;

	primitivesGroupe.setup("Primitives");
	gui.add(&primitivesGroupe);

	// Ajout des boutons pour les primitives
	primitivesGroupe.add(drawTriangle.setup("Draw Triangle",false));
	primitivesGroupe.add(drawCircle.setup("Draw Circle",false));
	primitivesGroupe.add(drawRectangle.setup("Draw Rectangle",false));

	/*
	// Associer des fonctions de rappel aux boutons
	drawTriangle.addListener(this, &Application::button_triangle);
	drawCircle.addListener(this, &Application::button_circle);
	drawRectangle.addListener(this, &Application::button_rectangle);
	gui.add(&primitivesGroupe);
	*/
}


void Application::update() {}


void Application::draw(){
	if (isImportable) {
		renderer.import_activate = true;
		ofDrawBitmapString("Please drag an image to import it.", 30, 30);
	}
	renderer.draw();

	ofPushMatrix();
	ofTranslate(uiPosition->x, uiPosition->y);
	for (int i = 0; i < uiAmount; i++) {
		ofPushMatrix();
		ofTranslate(i * uiStep->x, i * uiStep->y);
		ofRotateXDeg(i * uiRotate->x);
		ofRotateYDeg(i * uiRotate->y);
		ofRotateZDeg(i * uiRotate->z);
		ofTranslate(i * uiShift->x, i * uiShift->y);
		ofScale(uiSize->x, uiSize->y);
		//ofDrawTriangle(0, 0, -16, 32, 16, 32);
		ofBeginShape();
		ofVertex(0, 0);
		ofVertex(-16, 32);
		ofVertex(16, 32); // Autre façon de dessiner un triangle
		ofEndShape();
		ofPopMatrix();

	}
	ofPopMatrix();
	gui.draw();
}


void Application::keyPressed(int key) {}


void Application::keyReleased(int key){
	if (key == 105) { // 105 = key "i"
		isImportable = !isImportable;
		renderer.import_activate = !renderer.import_activate;
	}
}


void Application::mouseMoved(int x, int y ){
	renderer.mouse_current_x = x;
	renderer.mouse_current_y = y;
}

void Application::mouseDragged(int x, int y, int button){
	renderer.mouse_current_x = x;
	renderer.mouse_current_y = y;

	renderer.mouse_drag_x = x;
	renderer.mouse_drag_y = y;

	renderer.is_mouse_button_dragged = true;
	renderer.is_mouse_button_pressed = false;
}

void Application::mousePressed(int x, int y, int button){
	if (isImportable && !isRepositioning) { //Si l'importation d'image est active
		list<vector<int>>::reverse_iterator imgPos = renderer.imgPosList.rbegin();
		for (list<ofImage>::reverse_iterator iter = renderer.imageList.rbegin(); iter != renderer.imageList.rend(); ++iter) {
			vector temp = *imgPos;
			if (ofGetMouseX() > temp[0] && ofGetMouseX() < temp[0] + iter->getWidth() && ofGetMouseY() > temp[1] && ofGetMouseY() < temp[1] + iter->getHeight()) {
				isRepositioning = true;
				break;
			}
			++imgPos;
			imgDistFromMax++;
		}
	}

	renderer.is_mouse_button_pressed = true;
	renderer.is_mouse_button_dragged = false;

	renderer.mouse_current_x = x;
	renderer.mouse_current_y = y;

	renderer.mouse_press_x = x;
	renderer.mouse_press_y = y;
}


void Application::mouseReleased(int x, int y, int button){
	if (isRepositioning) { //Si une image est en repositionnement
		isRepositioning = false;
		list<vector<int>>::reverse_iterator imgPos = renderer.imgPosList.rbegin();
		for (int i = imgDistFromMax; i > 0; i--) {
			++imgPos;
		}
		*imgPos = {ofGetMouseX(), ofGetMouseY()};
		imgDistFromMax = 0;
	}

	if (button == 0 && y < 40) {
		if (x < IMPORT_BUTTON_WIDTH) {
			//call to import image method
			isImportable = !isImportable;
		}
	}

	renderer.is_mouse_button_pressed = false;
	renderer.is_mouse_button_dragged = false;

	renderer.mouse_current_x = x;
	renderer.mouse_current_y = y;
}


void Application::mouseEntered(int x, int y){

	renderer.mouse_current_x = x;
	renderer.mouse_current_y = y;
}


void Application::mouseExited(int x, int y){

	renderer.mouse_current_x = x;
	renderer.mouse_current_y = y;
}


void Application::windowResized(int w, int h){
	WIDTH = w;
	HEIGHT = h;
}


void Application::gotMessage(ofMessage msg){

}


void Application::dragEvent(ofDragInfo dragInfo) {
	if (isImportable) {
		vector<string> imgTypes = { ".png", ".jpg", ".gif" }; //Verifie si le fichier dragged dans la fenetre de l'application est une image
		for (int i = 0; i < dragInfo.files.size(); i++) {
			if (find(imgTypes.begin(), imgTypes.end(), dragInfo.files.at(i).substr(dragInfo.files.at(i).size() - 4, dragInfo.files.at(i).size() - 1)) != imgTypes.end()) {
				renderer.newImage(dragInfo.files.at(i), ofGetMouseX(), ofGetMouseY());
			}
		}
	}
}

void Application::button_triangle() {
	draw_triangle = !draw_triangle; 
	if (draw_triangle) {
		draw_circle = false;
		draw_rectangle = false;
	}
}

void Application::button_circle() {
	draw_circle = !draw_circle;
	if (draw_circle) {
		draw_triangle = false;
		draw_rectangle = false;
	}
}

void Application::button_rectangle() {
	draw_rectangle = !draw_rectangle;
	if (draw_rectangle) {
		draw_circle = false;
		draw_triangle = false;
	}
}