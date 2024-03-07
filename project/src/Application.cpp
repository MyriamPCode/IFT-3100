#include "Application.h"
#include "Constants.h"
#include <cmath>
#include <iostream>

using namespace std;

void Application::setup(){
	ofSetWindowTitle("Team 7");
	ofBackground(backgroundColor);
	renderer.setup();
	//toggleDraw, toggleTransform
	gui.setup();
	gui.setPosition(300, 40);
	primitivesMode.setup("Mode");
	primitivesMode.add(toggleDraw.setup("Drawing", false));
	primitivesMode.add(toggleTransform.setup("Transformation", false));
	renderer.inputIndex.setup("Index de la liste: ", 0);
	gui.add(&renderer.inputIndex);
	//gui.add(renderer.uiIndex.set("Index de la liste: ", int(0))); 
	toggleDraw.addListener(this, &Application::button_modeDraw);
	toggleTransform.addListener(this, &Application::button_modeTransform);
	gui.add(&primitivesMode);

	primitivesGroupe.setup("Primitives");

	// Ajout des boutons pour les primitives
	primitivesGroupe.add(drawTriangle.setup("Triangle",false));
	primitivesGroupe.add(drawCircle.setup("Circle",false));
	primitivesGroupe.add(drawRectangle.setup("Rectangle",false));
	primitivesGroupe.add(drawLine.setup("Line", false));
	primitivesGroupe.add(drawEllipse.setup("Ellipse", false));
	primitivesGroupe.add(drawBezier.setup("Bezier", false));
	
	// Associer des fonctions de rappel aux boutons
	drawTriangle.addListener(this, &Application::button_triangle);
	drawCircle.addListener(this, &Application::button_circle);
	drawRectangle.addListener(this, &Application::button_rectangle);
	drawLine.addListener(this, &Application::button_line);
	drawEllipse.addListener(this, &Application::button_ellipse);
	drawBezier.addListener(this, &Application::button_bezier);

	gui.add(&primitivesGroupe);
	gui.add(renderer.uiPosition.set("position", ofVec2f(0), ofVec2f(0), ofVec2f(ofGetWidth(), ofGetHeight()))); // La position des primitives
	gui.add(uiAmount.set("amount", renderer.v_formes.size(), 0, renderer.v_formes.max_size()));
	gui.add(renderer.uiStep.set("step", ofVec2f(0), ofVec2f(0), ofVec2f(300)));
	gui.add(renderer.uiRotate.set("rotate", ofVec3f(0), ofVec3f(-180), ofVec3f(180))); // La rotation des primitives
	gui.add(renderer.uiShift.set("shift", ofVec2f(0), ofVec2f(0), ofVec2f(300)));
	gui.add(renderer.uiSize.set("size", ofVec2f(1), ofVec2f(1), ofVec2f(5)));

	reinitialisationGroupe.setup("Reinitialisation");
	reinitialisationGroupe.add(resetButton.setup("Reset", false));
	resetButton.addListener(this, &Application::reset);
	gui.add(&reinitialisationGroupe);

	renderer.setup(renderer.v_formes);
	forme.setup();
	diffX = (abs(forme.getX2() - forme.getX3())) / 2;
	diffY = abs(forme.getY1() - forme.getY2());
	newX2 = 0;
	newY2 = 0;
	newX3 = 0;
	newY3 = 0;

	// état par défaut des bool
	draw_triangle = draw_circle = draw_rectangle =
		draw_line = draw_ellipse = draw_bezier = false;
	

	shapeBool = false; 
	v_buttons_ptr = &v_buttons;
	guiScene.setup();
	guiScene.setPosition(0, 40);
}

void Application::update()
{
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

	renderer.draw();
	ofPopMatrix();
	gui.draw();
	guiScene.draw();
}


// a modifier ou effacer 
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

	if (!buttonsToDelete.empty())
	{
		cout << "Deletion is possible! " << endl;
		cout << "Size of the deletion list is " << buttonsToDelete.size() << endl;
	}
}

void Application::keyPressed(int key) 
{
	if (key == 'd') { // 105 = key "d"
		//deleteShapeSelected();
		int index = renderer.inputIndex;


		if (index >= 0 && index < v_buttons.size() && index < renderer.v_formes.size()) {
			v_buttons.erase(v_buttons.begin() + index); // Supprimer l'élément du vecteur v_buttons
			//renderer.v_formes.erase(renderer.v_formes.begin() + index); // Supprimer l'élément du vecteur renderer.v_formes
		}


		//if (index >= 0 && index < v_buttons_ptr->size() && index < renderer.v_formes_ptr->size()) {
		//	v_buttons_ptr->at(index).reset(); // Libérer la mémoire associée à l'élément de v_buttons_ptr
		//	renderer.v_formes_ptr->at(index).reset(); // Libérer la mémoire associée à l'élément de renderer.v_formes_ptr

		//	v_buttons_ptr->erase(v_buttons_ptr->begin() + index); // Supprimer le pointeur du vecteur v_buttons_ptr
		//	renderer.v_formes_ptr->erase(renderer.v_formes_ptr->begin() + index); // Supprimer le pointeur du vecteur renderer.v_formes_ptr
		//}

		//if (index >= 0 && index < v_buttons_ptr->size() && index < renderer.v_formes_ptr->size()) {
		//	v_buttons_ptr->erase(v_buttons_ptr->begin() + index); // Supprimer le pointeur du vecteur v_buttons_ptr
		//	renderer.v_formes_ptr->erase(renderer.v_formes_ptr->begin() + index); // Supprimer le pointeur du vecteur renderer.v_formes_ptr
		//}


		//if (index >= 0 && index < v_buttons_ptr->size() && index < renderer.v_formes_ptr->size()) {
		//	// Extraire les pointeurs bruts
		//	ofxToggle* buttonPtr = v_buttons_ptr->at(index).release();
		//	Forme* formePtr = renderer.v_formes_ptr->at(index).release();

		//	// Supprimer les pointeurs du vecteur
		//	v_buttons_ptr->erase(v_buttons_ptr->begin() + index);
		//	renderer.v_formes_ptr->erase(renderer.v_formes_ptr->begin() + index);

		//	// Libérer manuellement la mémoire
		//	delete buttonPtr;
		//	delete formePtr;
		//}

	}
	// Pour tester les pointeurs 
	if (key == 'f') {
		for (const auto& ptr : *renderer.v_formes_ptr) {
			cout << "Adresse du pointeur : " << &ptr << endl;
			cout << "Adresse de l'objet Forme : " << ptr.get() << endl; // Obtenir l'adresse de l'objet pointé
			cout << "Valeur de l'objet Forme : " << ptr << endl;
			cout << endl;
		}		
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
	

	if (renderer.modeDrawState && toggleDraw)
	{
		if (draw_triangle && drawTriangle)
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
			auto button = make_unique<ofxToggle>();
			//button->addListener(this, &Application::buttons_list);
			guiScene.add(button->setup("TRIANGLE", false)); // Nom du bouton shapeBool
			v_buttons.push_back(move(button)); // Ajoutez le bouton � la liste des boutons
		}

		if (draw_circle && drawCircle)
		{
			forme.setXC(renderer.mouse_press_x);
			forme.setYC(renderer.mouse_press_y);
			renderer.v_formes.push_back(make_unique<Forme>(Forme::CERCLE, forme.getXC(), forme.getYC(), forme.getRayon()));
			//ofSetCircleResolution(55);
			renderer.okDessiner = true;
			renderer.cercleColors = { renderer.interface.color_picker_stroke, renderer.interface.colorPickerFill }; // Ajuste les parametres
			renderer.cercleFill = renderer.interface.fillEnabled;
			renderer.cercleStroke = renderer.interface.slider_stroke_weight;
			auto button = make_unique<ofxToggle>();
			guiScene.add(button->setup("CERCLE", false)); // Nom du bouton
			v_buttons.push_back(move(button)); // Ajoutez le bouton � la liste des boutons
		}

		if (draw_rectangle && drawRectangle)
		{
			forme.setXR(renderer.mouse_current_x);
			forme.setYR(renderer.mouse_current_y);
			renderer.v_formes.push_back(make_unique<Forme>(Forme::RECTANGLE, forme.getXR(), forme.getYR(), forme.getWidth(), forme.getHeight()));
			renderer.okDessiner = true;
			renderer.rectangleColors = { renderer.interface.color_picker_stroke, renderer.interface.colorPickerFill }; // Ajuste les parametres
			renderer.rectangleFill = renderer.interface.fillEnabled;
			renderer.rectangleStroke = renderer.interface.slider_stroke_weight;
			auto button = make_unique<ofxToggle>();
			guiScene.add(button->setup("RECTANGLE", false)); // Nom du bouton
			v_buttons.push_back(move(button)); // Ajoutez le bouton � la liste des boutons
		}

		// DRAW LINE
		///////////////////////////////
		if (draw_line && drawLine)
		{
			ofVec3f v(renderer.mouse_press_x, renderer.mouse_press_y, 0);
			renderer.addPoint(v);

			// Ajoutez le bouton pour la ligne
			renderer.v_formes.push_back(make_unique<Forme>(Forme::LIGNE, v));

			renderer.okDessiner = true;
			renderer.ligneColor = renderer.interface.color_picker_stroke; // Ajuste les parametres
			renderer.ligneStroke = renderer.interface.slider_stroke_weight;

			// Ajoutez également un bouton pour cette ligne
			auto button = make_unique<ofxToggle>();
			guiScene.add(button->setup("LIGNE", false));
			v_buttons.push_back(move(button));
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

		if (draw_bezier && drawBezier)
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


	//showButtonsList();
	int nombre = renderer.v_formes.size();
	int max = renderer.v_formes.max_size();
	uiAmount.set(nombre);
}

void Application::showButtonsList(){
	if(v_buttons_ptr)
	{ 
	// Parcourir la liste des boutons
	for (size_t i = 0; i < v_buttons.size(); ++i) {
		auto& button = v_buttons[i];
		auto& forme = *renderer.v_formes[i];

		// Déterminer le type de forme
		string formeType;
		switch (forme.getType()) {
		case Forme::TRIANGLE:
			formeType = "TRIANGLE";
			break;
		case Forme::CERCLE:
			formeType = "CERCLE";
			break;
		case Forme::RECTANGLE:
			formeType = "RECTANGLE";
			break;
		//case Forme::LIGNE:
		//	formeType = "LIGNE";
		//	break;
		case Forme::ELLIPSE:
			formeType = "ELLIPSE";
			break;
		case Forme::BEZIER:
			formeType = "BEZIER";
			break;
			// Ajoutez d'autres cas pour les autres types de forme
		default:
			formeType = "INCONNU";
			break;
		}

		// Créer l'étiquette en fonction du type et de l'index de la forme
		string label = to_string(i) + " " + formeType;

		// Ajouter le bouton avec l'étiquette dynamique
		guiScene.add(button->setup(label, false));
		}
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
		switch (static_cast<int>(floor(x / iconWidth))) {
			case 0:
				//call to import image method
				cout << "import \n";
				isImportable = !isImportable;
				break;
			case 1:
				//call to export method
				cout << "export \n";
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
			cout << "color wheel \n";
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
		renderer.uiPosition.set(ofVec2f(0));
		uiAmount.set(1);
		renderer.uiStep.set(ofVec2f(0));
		renderer.uiRotate.set(ofVec3f(0));
		renderer.uiShift.set(ofVec2f(0));
		renderer.uiSize.set(ofVec2f(6));

		draw_triangle = false;
		drawTriangle = false;
		draw_circle = false;
		drawCircle = false;
		draw_rectangle = false;
		drawRectangle = false;
		resetButton = false;
	}
}

void Application::button_modeDraw(bool& value) {
	if (value) {
		renderer.modeDrawState = !renderer.modeDrawState;
		renderer.modeTransformState = toggleTransform = false;
	}
}

void Application::button_modeTransform(bool& value) {
	if (value) {
		renderer.modeTransformState = !renderer.modeTransformState;
		renderer.modeDrawState = toggleDraw = false;
	}
}


