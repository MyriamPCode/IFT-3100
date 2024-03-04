using namespace std;

#include "Application.h"
#include "Constants.h"

void Application::setup(){
	ofSetWindowTitle("Team 7");
	ofBackground(backgroundColor);
	renderer.setup();

	cam.setDistance(100);

	gui.setup();
	gui.add(uiPosition.set("Position", ofVec2f(0), ofVec2f(0), ofVec2f(ofGetWidth(), ofGetHeight()))); // La position des primitives
	gui.add(uiAmount.set("Amount", 1, 0, 64)); // La quantit� de primitives. Nombre maximal est 64 et nombre minimum est 1
	gui.add(uiStep.set("Step", ofVec2f(0), ofVec2f(0), ofVec2f(300)));
	gui.add(uiRotate.set("Rotate", ofVec3f(0), ofVec3f(-180), ofVec3f(180))); // La rotation des primitives
	gui.add(uiShift.set("Shift", ofVec2f(0), ofVec2f(0), ofVec2f(300)));
	gui.add(uiSize.set("Size", ofVec2f(6), ofVec2f(0), ofVec2f(30)));

	draw_triangle = false;
	draw_circle = false;
	draw_rectangle = false;
	rotation_activate = false;
	mesh_activate = false;
	noise_activate = false;

	primitivesGroupe.setup("Primitives");

	// Ajout des boutons pour les primitives
	primitivesGroupe.add(drawTriangle.setup("Draw Triangle",false));
	primitivesGroupe.add(drawCircle.setup("Draw Circle",false));
	primitivesGroupe.add(drawRectangle.setup("Draw Rectangle",false));

	
	// Associer des fonctions de rappel aux boutons
	drawTriangle.addListener(this, &Application::button_triangle);
	drawCircle.addListener(this, &Application::button_circle);
	drawRectangle.addListener(this, &Application::button_rectangle);
	gui.add(&primitivesGroupe);

	reinitialisationGroupe.setup("Reinitialisation");
	reinitialisationGroupe.add(resetButton.setup("Reset", false));
	resetButton.addListener(this, &Application::reset);
	gui.add(&reinitialisationGroupe);

	animationGroupe.setup("Animations");
	animationGroupe.add(rotationButton.setup("Rotation", false));
	rotationButton.addListener(this, &Application::button_rotation);
	gui.add(&animationGroupe);

	meshGroupe.setup("Maille g�om�trique");
	meshGroupe.add(meshButton.setup("Maille", false));
	meshButton.addListener(this, &Application::button_mesh);
	meshGroupe.add(bruitButton.setup("Bruit", false));
	bruitButton.addListener(this, &Application::button_noise);
	gui.add(&meshGroupe);
	
	
	for (int x = 0; x < size; x++) {
		for (int y = 0; y < size; y++) {
			mesh.addVertex(ofPoint(x - size / 2, y - size / 2));
			}
		}

	for (int y = 0; y < size - 1; y++) {
		for (int x = 0; x < size - 1; x++) {
			mesh.addIndex(x + y * size);
			mesh.addIndex((x+1) + y * size);
			mesh.addIndex(x + (y+1) * size);
			mesh.addIndex((x + 1) + y * size);
			mesh.addIndex((x + 1) + (y + 1) * size);
			mesh.addIndex(x + (y + 1) * size);
		}
	}
}


void Application::update() {
	rotate++;
	if (noise_activate) {
		int count = 0;
		for (int x = 0; x < size; x++) {
			for (int y = 0; y < size; y++) {
				ofVec3f vertex = mesh.getVertex(count);
				vertex.z = ofMap(ofNoise(count, ofGetElapsedTimef()), 0, 1, 0, uiAmount);
				mesh.setVertex(count, vertex);
				count++;
			}
		}
	}
}


void Application::draw(){
	if (isImportable) {
		renderer.import_activate = true;
		ofDrawBitmapString("Please drag an image to import it.", 30, 30);
	}
	renderer.draw();

	ofPushMatrix();
	cam.begin();
	ofTranslate(uiPosition->x, uiPosition->y);
	for (int i = 0; i < uiAmount; i++) {
		ofPushMatrix();
		ofTranslate(i * uiStep->x, i * uiStep->y);
		ofRotateXDeg(i * uiRotate->x);
		ofRotateYDeg(i * uiRotate->y);
		ofRotateZDeg(i * uiRotate->z);
		ofTranslate(i * uiShift->x, i * uiShift->y);
		ofScale(uiSize->x, uiSize->y);
		ofBeginShape();
		if (draw_triangle) {
			if (rotation_activate == true) {
				ofRotateXDeg(rotate);
				ofRotateYDeg(rotate);
				ofRotateZDeg(rotate);
			}
			ofDrawTriangle(0, 0, -16, 32, 16, 32);
		} 
		if (draw_circle) {
			if (rotation_activate == true) {
				ofRotateXDeg(rotate);
				ofRotateYDeg(rotate);
				ofRotateZDeg(rotate);
			}
			ofDrawCircle(100, 100, 50);
			ofSetCircleResolution(55);
		}
		if (draw_rectangle) {
			if (rotation_activate == true) {
				ofRotateXDeg(rotate);
				ofRotateYDeg(rotate);
				ofRotateZDeg(rotate);
			}
			ofDrawRectangle(50, 50, 100, 200);
		}
		if (mesh_activate) {
			mesh.drawWireframe();
			if (noise_activate) {
				button_noise(noise_activate);
			}

		}
		ofEndShape();
		ofPopMatrix();

	}
	cam.end();
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
	ofEasyCam cam;
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

void Application::button_triangle(bool& value) { 
	if (value) {
		draw_triangle = true;
		draw_circle = false;
		draw_rectangle = false;
		drawCircle = false;
		drawRectangle = false;
	}
}

void Application::button_circle(bool& value) {
	if (value) {
		draw_circle = true;
		draw_triangle = false;
		draw_rectangle = false;
		drawTriangle = false;
		drawRectangle = false;
	}
}

void Application::button_rectangle(bool& value) {
	if (value) {
		draw_rectangle = true;
		draw_circle = false;
		draw_triangle = false;
		drawTriangle = false;
		drawCircle = false;
	}
}

void Application::reset(bool & value) {
	if (value) {
		uiPosition.set(ofVec2f(0));
		uiAmount.set(1);
		uiStep.set(ofVec2f(0));
		uiRotate.set(ofVec3f(0));
		uiShift.set(ofVec2f(0));
		uiSize.set(ofVec2f(6));

		draw_triangle = false;
		drawTriangle = false;
		draw_circle = false;
		drawCircle = false;
		draw_rectangle = false;
		drawRectangle = false;
		resetButton = false;
		rotationButton = false;
		rotation_activate = false;
		mesh_activate = false;
		meshButton = false;
		noise_activate = false;
		bruitButton = false;
	}
}

void Application::button_rotation(bool& value) {
	rotation_activate = value;
	if (value) {
		rotation_activate = true;
	}
}

void Application::button_mesh(bool& value) {
	mesh_activate = value;
	if (value) {
		mesh_activate = true;
	}
}

void Application::button_noise(bool& value) {
	noise_activate = value;
	if (value) {
		noise_activate = true;
	}
}