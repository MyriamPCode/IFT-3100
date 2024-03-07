#include "Application.h"
#include "Constants.h"
#include <cmath>

using namespace std;

void Application::setup(){
	ofSetWindowTitle("Team 7");
	ofBackground(backgroundColor);
	renderer.setup();

	cam.setDistance(100);
	
	gui.setup();
	gui.setPosition(300, 40);
	gui.add(uiPosition.set("position", ofVec2f(0), ofVec2f(0), ofVec2f(ofGetWidth(), ofGetHeight()))); // La position des primitives
	gui.add(uiAmount.set("amount", 1, 0, 64)); // La quantit� de primitives. Nombre maximal est 64 et nombre minimum est 1
	gui.add(uiStep.set("step", ofVec2f(0), ofVec2f(0), ofVec2f(300)));
	gui.add(uiRotate.set("rotate", ofVec3f(0), ofVec3f(-180), ofVec3f(180))); // La rotation des primitives
	gui.add(uiShift.set("shift", ofVec2f(0), ofVec2f(0), ofVec2f(300)));
	gui.add(uiSize.set("size", ofVec2f(6), ofVec2f(0), ofVec2f(30)));

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
	primitivesGroupe.add(drawLine.setup("Draw Line", false));
	primitivesGroupe.add(drawEllipse.setup("Draw Ellipse", false));
	primitivesGroupe.add(drawBezier.setup("Draw Bezier", false));
	
	// Associer des fonctions de rappel aux boutons
	drawTriangle.addListener(this, &Application::button_triangle);
	drawCircle.addListener(this, &Application::button_circle);
	drawRectangle.addListener(this, &Application::button_rectangle);
	drawLine.addListener(this, &Application::button_line);
	drawEllipse.addListener(this, &Application::button_ellipse);
	drawBezier.addListener(this, &Application::button_bezier);

	gui.add(&primitivesGroupe);

	reinitialisationGroupe.setup("Reinitialisation");
	reinitialisationGroupe.add(resetButton.setup("Reset", false));
	resetButton.addListener(this, &Application::reset);
	gui.add(&reinitialisationGroupe);

	animationGroupe.setup("Animations");
	animationGroupe.add(rotationButton.setup("Rotation", false));
	rotationButton.addListener(this, &Application::button_rotation);
	gui.add(&animationGroupe);

	meshGroupe.setup("Maille geométrique");
	meshGroupe.add(meshButton.setup("Maille", false));
	meshButton.addListener(this, &Application::button_mesh);
	meshGroupe.add(meshAnimationButton.setup("Bruit", false));
	meshAnimationButton.addListener(this, &Application::button_noise);
	gui.add(&meshGroupe);

	// Création de la maille
	for (int x = 0; x < size; x++) {
		for (int y = 0; y < size; y++) {
			mesh.addVertex(ofPoint(x - size / 2, y - size / 2));
		}
	}

	for (int y = 0; y < size - 1; y++) {
		for (int x = 0; x < size - 1; x++) {
			mesh.addIndex(x + y * size);
			mesh.addIndex((x + 1) + y * size);
			mesh.addIndex(x + (y + 1) * size);
			mesh.addIndex((x + 1) + y * size);
			mesh.addIndex((x + 1) + (y + 1) * size);
			mesh.addIndex(x + (y + 1) * size);
		}
	}

	//renderer.setup(forme.v_formes);
	renderer.setup(renderer.v_formes);
	forme.setup();
	diffX = (abs(forme.getX2() - forme.getX3())) / 2;
	diffY = abs(forme.getY1() - forme.getY2());
	newX2 = 0;
	newY2 = 0;
	newX3 = 0;
	newY3 = 0;

	draw_line = draw_ellipse = draw_bezier = false; 

	shapeBool = false; 
	v_buttons_ptr = &v_buttons;
	guiScene.setup();
	guiScene.setPosition(0, 40);
}

void Application::update()
{
	// Rotations des primitives vectorielles
	rotate++;

	// Animation sur la maille 
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
	if (renderer.isRecording) {
		// Mettez � jour et capturez l'image � intervalles r�guliers
		renderer.update();
		renderer.captureImage();
	}
}

void Application::draw(){
	if (isImportable) {
		renderer.import_activate = true;
		ofDrawBitmapString("Please drag an image to import it.", 30, 30);
	}
	
	// Partie Myriam 
	//************** 
	//ofPushMatrix();
	// cam.begin();
	//ofTranslate(uiPosition->x, uiPosition->y);
	//for (int i = 0; i < uiAmount; i++) {
	//	ofPushMatrix();
	//	ofTranslate(i * uiStep->x, i * uiStep->y);
	//	ofRotateXDeg(i * uiRotate->x);
	//	ofRotateYDeg(i * uiRotate->y);
	//	ofRotateZDeg(i * uiRotate->z);
	//	ofTranslate(i * uiShift->x, i * uiShift->y);
	//	ofScale(uiSize->x, uiSize->y);
	//	ofBeginShape();
	//	if (draw_triangle) {
	//     if (rotation_activate == true) {
	            //ofRotateXDeg(rotate);
	            //ofRotateYDeg(rotate);
	            //ofRotateZDeg(rotate);
            //}
	//		//ofDrawTriangle(0, 0, -16, 32, 16, 32);
	//	} 
	//	if (draw_circle) {
	//      if (rotation_activate == true) {
	            //ofRotateXDeg(rotate);
	            //ofRotateYDeg(rotate);
	            //ofRotateZDeg(rotate);
            //}
	//		//ofDrawCircle(100, 100, 50);
	//		//ofSetCircleResolution(55);
	//	}
	//	if (draw_rectangle) {
	/*
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

    }*/
	//	ofEndShape();
	//	ofPopMatrix();
	//}
	
	renderer.draw();
	//cam.end();
	ofPopMatrix();
	gui.draw();
	guiScene.draw();
}

void Application::deleteShapeSelected()
{
	// Vecteur temporaire pour stocker les indices des boutons � supprimer
	vector<int> buttonsToDelete;
	// Vecteur pour stocker l'�tat de chaque bouton
	vector<bool> buttonStates;

	for (const auto& b : *v_buttons_ptr)
	{
		for (int i = 0; i < v_buttons.size(); ++i)
		{
			// V�rifier si le bouton est en �tat TRUE
			if (i == shapeBool) // v_buttons[i] Acc�der � l'�tat bool�en du bouton
			{
				cout << "Il est cense avoir " << i << " forme a effacer" << endl;
				// Ajouter l'index du bouton � supprimer dans le vecteur temporaire
				buttonsToDelete.push_back(i);
			}
		}

	}
	//for (int i = 0; i < v_buttons.size(); ++i)
	//{
	//	// V�rifier si le bouton est en �tat TRUE
	//	if (i == shapeBool) // v_buttons[i] Acc�der � l'�tat bool�en du bouton
	//	{
	//		cout << "Il est cense avoir " << i << " forme a effacer" << endl;
	//		// Ajouter l'index du bouton � supprimer dans le vecteur temporaire
	//		buttonsToDelete.push_back(i);
	//	}
	//}

	if (!buttonsToDelete.empty())
	{
		cout << "Deletion is possible! " << endl;
		cout << "Size of the deletion list is " << buttonsToDelete.size() << endl;
	}

	// Parcourir tous les boutons dans la liste
	//for (int i = 0; i < buttonStates.size(); ++i)
	//{
	//	// V�rifier si le bouton est en �tat TRUE
	//	if (buttonStates[i] == TRUE)
	//	{
	//		// Ajouter l'index du bouton � supprimer dans le vecteur temporaire
	//		buttonsToDelete.push_back(i);
	//	}
	//}


	// Supprimer les boutons en parcourant le vecteur temporaire en sens inverse
	//for (int i = buttonsToDelete.size() - 1; i >= 0; --i)
	//{
	//	v_buttons.erase(v_buttons.begin() + buttonsToDelete[i]);
	//	buttonStates.erase(buttonStates.begin() + buttonsToDelete[i]); // Supprimer �galement l'�tat correspondant
	//}
}

void Application::keyPressed(int key) 
{
	if (key == 100) { // 105 = key "d"
		deleteShapeSelected();
	}

	// D�marrer/arr�ter l'enregistrement lors de l'appui sur la touche 'r'
	if (key == 'r') {
		renderer.isRecording = !renderer.isRecording;
		if (renderer.isRecording) {
			renderer.frameCounter = 0; // R�initialiser le compteur de frames lors du d�marrage de l'enregistrement
			cout << "Enregistrement d�marr�." << endl;
			//renderer.update();
			//renderer.captureImage();
		}
		else {
			cout << "Enregistrement arr�t�." << endl;
		}
	}
}

void Application::keyReleased(int key){
	if (key == 105) { // 105 = key "i"
		isImportable = !isImportable;
		renderer.import_activate = !renderer.import_activate;
	}
	if (key == 'o') { //Change le type de dessin des mod�les 3D
		renderer.interface.toggleModelOptions();
		/*if (renderer.typeRender == MeshRenderMode::wireframe) {
			renderer.typeRender = MeshRenderMode::fill;
		}
		else if (renderer.typeRender == MeshRenderMode::fill) {
			renderer.typeRender = MeshRenderMode::vertex;
		}
		else if (renderer.typeRender == MeshRenderMode::vertex) {
			renderer.typeRender = MeshRenderMode::wireframe;
		}*/
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

	if (draw_line && drawLine)
	{
		renderer.ligne.addVertex(renderer.mouse_drag_x, renderer.mouse_drag_y);
	}
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

	if(draw_triangle && drawTriangle)
	{
		// A partir du mouse click, calcul des 2 autres sommets 
		newX2 = renderer.mouse_press_x - diffX;
		newY2 = renderer.mouse_press_y + diffY;
		newX3 = renderer.mouse_press_x + diffX;
		newY3 = renderer.mouse_press_y + diffY;
		forme.setX1(x);
		forme.setY1(y);
		forme.setX2(newX2);
		forme.setY2(newY2);
		forme.setX3(newX3);
		forme.setY3(newY3);

		renderer.v_formes.push_back(make_unique<Forme>(Forme::TRIANGLE, forme.getX1(), forme.getY1(),
			forme.getX2(), forme.getY2(),
			forme.getX3(), forme.getY3()));

		renderer.okDessiner = true;
		renderer.triangleColors = { renderer.interface.color_picker_stroke, renderer.interface.colorPickerFill }; // Ajuste les parametres
		renderer.triangleFill = renderer.interface.fillEnabled;
		renderer.triangleStroke = renderer.interface.slider_stroke_weight;
		//ofxToggle button;
		auto button = make_unique<ofxToggle>();
		button->addListener(this, &Application::buttons_list);
		guiScene.add(button->setup("TRIANGLE", shapeBool)); // Nom du bouton
		v_buttons.push_back(move(button)); // Ajoutez le bouton � la liste des boutons
	}

	if(draw_circle && drawCircle)
	{ 
		forme.setXC(renderer.mouse_press_x); 
		forme.setYC(renderer.mouse_press_y); 
		renderer.v_formes.push_back(make_unique<Forme>(Forme::CERCLE, forme.getXC(), forme.getYC(), forme.getRayon()));
		//ofSetCircleResolution(55);
		renderer.okDessiner = true; 
		renderer.cercleColors = { renderer.interface.color_picker_stroke, renderer.interface.colorPickerFill }; // Ajuste les parametres
		renderer.cercleFill = renderer.interface.fillEnabled;
		renderer.cercleStroke = renderer.interface.slider_stroke_weight;
		//ofxToggle button;
		auto button = make_unique<ofxToggle>();
		guiScene.add(button->setup("CERCLE", false)); // Nom du bouton
		v_buttons.push_back(move(button)); // Ajoutez le bouton � la liste des boutons
	}

	if(draw_rectangle && drawRectangle)
	{ 
		forme.setXR(renderer.mouse_current_x);
		forme.setYR(renderer.mouse_current_y); 
		renderer.v_formes.push_back(make_unique<Forme>(Forme::RECTANGLE, forme.getXR(), forme.getYR(), forme.getWidth(), forme.getHeight()));
		renderer.okDessiner = true;
		renderer.rectangleColors = { renderer.interface.color_picker_stroke, renderer.interface.colorPickerFill }; // Ajuste les parametres
		renderer.rectangleFill = renderer.interface.fillEnabled;
		renderer.rectangleStroke = renderer.interface.slider_stroke_weight;
		//ofxToggle button;
		auto button = make_unique<ofxToggle>();
		guiScene.add(button->setup("RECTANGLE", false)); // Nom du bouton
		v_buttons.push_back(move(button)); // Ajoutez le bouton � la liste des boutons
	}

	// DRAW LINE
	///////////////////////////////
	if (draw_line && drawLine)
	{
		//renderer.vecteur_lignes_ptr->emplace_back(make_unique<ofPolyline>());
		//auto& polyline = renderer.vecteur_lignes_ptr->back();
		//polyline->addVertex(renderer.mouse_press_x, renderer.mouse_press_y);

		renderer.ligne.addVertex(renderer.mouse_press_x, renderer.mouse_press_y);
		renderer.okDessiner = true;
		renderer.ligneColor = renderer.interface.color_picker_stroke; // Ajuste les parametres
		renderer.ligneStroke = renderer.interface.slider_stroke_weight;
		auto button = make_unique<ofxToggle>();
		guiScene.add(button->setup("LIGNE", false)); // Nom du bouton
		v_buttons.push_back(move(button)); // Ajoutez le bouton � la liste des boutons
	}
	///////////////////////////////

	if (draw_ellipse && drawEllipse)
	{
		forme.setXR(renderer.mouse_press_x);
		forme.setYR(renderer.mouse_press_y);
		renderer.v_formes.push_back(make_unique<Forme>(Forme::ELLIPSE, forme.getXR(), forme.getYR(), forme.getWidth(), forme.getHeight()));
		renderer.okDessiner = true;
		renderer.ellipseColors = { renderer.interface.color_picker_stroke, renderer.interface.colorPickerFill }; // Ajuste les parametres
		renderer.ellipseFill = renderer.interface.fillEnabled;
		renderer.ellipseStroke = renderer.interface.slider_stroke_weight;
		auto button = make_unique<ofxToggle>();
		guiScene.add(button->setup("ELLIPSE", false)); // Nom du bouton
		v_buttons.push_back(move(button)); // Ajoutez le bouton � la liste des boutons
	}

	if(draw_bezier && drawBezier)
	{
		float x1 = renderer.mouse_press_x; 
		float y1 = renderer.mouse_press_y; 
		float x4 = x1; 
		float y4 = y1 + 50; 
		float x2, y2, x3, y3;
		x2 = x1 * 0.75; 
		y2 = y1 * 0.75; 
		x3 = x4 * 0.75;
		y3 = y4 * 0.75;
		forme.setX1(x1); 
		forme.setY1(y1); 
		forme.setX2(x4);
		forme.setY2(y4); 
		forme.setXB1(x2);
		forme.setYB1(y2);
		forme.setXB2(x3);
		forme.setYB2(y3);

		renderer.v_formes.push_back(make_unique<Forme>(Forme::BEZIER, forme.getX1(), forme.getY1(), forme.getXB1(), forme.getYB1(),
			forme.getXB2(), forme.getYB2(), forme.getX2(), forme.getY2()));
		renderer.okDessiner = true;
		renderer.bezierColors = { renderer.interface.color_picker_stroke, renderer.interface.colorPickerFill }; // Ajuste les parametres
		renderer.bezierFill = renderer.interface.fillEnabled;
		renderer.bezierStroke = renderer.interface.slider_stroke_weight;
		auto button = make_unique<ofxToggle>();
		guiScene.add(button->setup("BEZIER", false)); // Nom du bouton
		v_buttons.push_back(move(button)); // Ajoutez le bouton � la liste des boutons
	}

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

	if (button == 0 && y < INTERACTION_BAR_HEIGHT) {
		int index = static_cast<int>(floor(x / (iconWidth)));
		switch (index) {
			case 0:
				//call to import image method
				cout << "import \n";
				isImportable = !isImportable;
				break;
			case 1:
				//call to export method
				renderer.toggleExportGUI();
				break;
			case 2:
				//call to animation method
				cout << "animation \n";
				break;
		}
	}

	if (button == 0 && x > WIDTH - INTERACTION_BAR_HEIGHT) {
		switch (static_cast<int>(floor((y / iconWidth)) - 1)) {
		case 0:
			//call to color wheel
			renderer.toggleColorWheelGUI();
			break;
		case 1:
			//call to pen method
			cout << "pen \n";
			break;
		case 2:
			//call to ellipse method
			cout << "ellipse \n";
			break;
		case 3:
			//call to rectangle method
			cout << "rectangle \n";
			break;
		case 4:
			//call to triangle method
			cout << "triangle \n";
			break;
		}
	}



	renderer.mouse_current_x = x;
	renderer.mouse_current_y = y;

	if (draw_line && drawLine)
	{
		renderer.is_mouse_button_dragged = true; 
		renderer.ligne.addVertex(x, y);
		renderer.vecteur_lignes.push_back(renderer.ligne);
		renderer.okDessiner = true;
		renderer.ligne.clear();
	}

	renderer.is_mouse_button_pressed = false;
	renderer.is_mouse_button_dragged = false;
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
		draw_triangle = !draw_triangle;
		draw_circle = draw_rectangle = draw_line = draw_ellipse = draw_bezier = false;
		drawCircle = drawRectangle = drawLine = drawEllipse = drawBezier = false;
		if (!renderer.triangleColors.empty()) { // Conserve les parametres de la forme pour la reselection
			renderer.interface.colorPickerFill = renderer.triangleColors[1];
			renderer.interface.color_picker_stroke = renderer.triangleColors[0];
			if (renderer.interface.fillButton != renderer.triangleFill) {
				renderer.interface.fillButton = renderer.triangleFill;
			}
			renderer.interface.slider_stroke_weight = renderer.triangleStroke;
		}
	}
}

void Application::button_circle(bool& value) {
	if (value) {
		draw_circle = !draw_circle;
		draw_triangle = draw_rectangle = draw_line = draw_ellipse = draw_bezier = false;
		drawTriangle = drawRectangle = drawLine = drawEllipse = drawBezier = false;
		if (!renderer.cercleColors.empty()) {// Conserve les parametres de la forme pour la reselection
			renderer.interface.colorPickerFill = renderer.cercleColors[1];
			renderer.interface.color_picker_stroke = renderer.cercleColors[0];
			if (renderer.interface.fillButton != renderer.cercleFill) {
				renderer.interface.fillButton = renderer.cercleFill;
			}
			renderer.interface.slider_stroke_weight = renderer.cercleStroke;
		}
	}
}

void Application::button_rectangle(bool& value) {
	if (value) {
		draw_rectangle = !draw_rectangle;
		draw_triangle = draw_circle = draw_line = draw_ellipse = draw_bezier = false;
		drawTriangle = drawCircle = drawLine = drawEllipse = drawBezier = false;
		if (!renderer.rectangleColors.empty()) { // Conserve les parametres de la forme pour la reselection
			renderer.interface.colorPickerFill = renderer.rectangleColors[1];
			renderer.interface.color_picker_stroke = renderer.rectangleColors[0];
			if (renderer.interface.fillButton != renderer.rectangleFill) {
				renderer.interface.fillButton = renderer.rectangleFill;
			}
			renderer.interface.slider_stroke_weight = renderer.rectangleStroke;
		}
	}
}

void Application::button_line(bool& value) {
	if (value) {
		draw_line = !draw_line;
		draw_triangle = draw_rectangle = draw_circle = draw_ellipse = draw_bezier = false;
		drawTriangle = drawRectangle = drawCircle = drawEllipse = drawBezier = false;
		renderer.interface.color_picker_stroke = renderer.ligneColor;
		renderer.interface.slider_stroke_weight = renderer.ligneStroke;
		
	}
}

void Application::button_ellipse(bool& value) {
	if (value) {
		draw_ellipse = !draw_ellipse;
		draw_triangle = draw_rectangle = draw_circle = draw_line = draw_bezier = false;
		drawTriangle = drawRectangle = drawCircle = drawLine = drawBezier = false;
		if (!renderer.ellipseColors.empty()) { // Conserve les parametres de la forme pour la reselection
			renderer.interface.colorPickerFill = renderer.ellipseColors[1];
			renderer.interface.color_picker_stroke = renderer.ellipseColors[0];
			if (renderer.interface.fillButton != renderer.ellipseFill) {
				renderer.interface.fillButton = renderer.ellipseFill;
			}
			renderer.interface.slider_stroke_weight = renderer.ellipseStroke;
		}
	}
}

void Application::button_bezier(bool& value) {
	if (value) {
		draw_bezier = !draw_bezier;
		draw_triangle = draw_rectangle = draw_circle = draw_line = draw_ellipse = false;
		drawTriangle = drawRectangle = drawCircle = drawLine = drawEllipse = false;
		if (!renderer.bezierColors.empty()) { // Conserve les parametres de la forme pour la reselection
			renderer.interface.colorPickerFill = renderer.bezierColors[1];
			renderer.interface.color_picker_stroke = renderer.bezierColors[0];
			if (renderer.interface.fillButton != renderer.bezierFill) {
				renderer.interface.fillButton = renderer.bezierFill;
			}
			renderer.interface.slider_stroke_weight = renderer.bezierStroke;
		}
	}
}

void Application::reset(bool& value) {
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
		meshAnimationButton = false;
	}
}

void Application::buttons_list(bool& value)
{
	if (value) {
		// Vecteur temporaire pour stocker les indices des boutons � supprimer
		//vector<int> buttonsToDelete;
		//// Vecteur pour stocker l'�tat de chaque bouton
		//vector<bool> buttonStates;

		cout << "Je t'ecoute!!" << endl;
		shapeBool = !shapeBool; 
		//if (!shapeBool)
		//{
		//	for(int i = 0; v_buttons.size(); i++)
		//	{
		//		buttonsToDelete.push_back(i);

		//		if (!buttonsToDelete.empty())
		//		{
		//			cout << "Deletion is possible! " << endl;
		//			cout << "Size of the deletion list is " << buttonsToDelete.size() << endl;
		//		}

				// Parcourir tous les boutons dans la liste
				//for (int i = 0; i < buttonStates.size(); ++i)
				//{
				//	// V�rifier si le bouton est en �tat TRUE
				//	if (buttonStates[i] == TRUE)
				//	{
				//		// Ajouter l'index du bouton � supprimer dans le vecteur temporaire
				//		buttonsToDelete.push_back(i);
				//	}
				//}


				// Supprimer les boutons en parcourant le vecteur temporaire en sens inverse
				//for (int i = buttonsToDelete.size() - 1; i >= 0; --i)
				//{
				//	v_buttons.erase(v_buttons.begin() + buttonsToDelete[i]);
				//	buttonStates.erase(buttonStates.begin() + buttonsToDelete[i]); // Supprimer �galement l'�tat correspondant
				//}
			//}
		//}
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