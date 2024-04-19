#include "Application.h"
#include "Constants.h"
#include <cmath>
#include <iostream>

using namespace std;

void Application::setup(){
	ofSetWindowTitle("BIBO");
	ofBackground(backgroundColor);
	renderer.setup();

	//toggleDraw, toggleTransform
	drawingGUI.setup();
	drawingGUI.setPosition(300, 40);
	primitivesMode.setup("Mode");
	primitivesMode.add(toggleDraw.setup("Drawing", false));
	primitivesMode.add(toggleTransform.setup("Transformation", false));
	renderer.inputIndex.setup("Index de la liste: ", 0);
	drawingGUI.add(&renderer.inputIndex);
	//gui.add(renderer.uiIndex.set("Index de la liste: ", int(0))); 
	toggleDraw.addListener(this, &Application::button_modeDraw);
	toggleTransform.addListener(this, &Application::button_modeTransform);
	drawingGUI.add(&primitivesMode);

	primitivesGroupe.setup("Primitives");

	// Ajout des boutons pour les primitives
	primitivesGroupe.add(toggleDrawTriangle.setup("Triangle",false));
	primitivesGroupe.add(toggleDrawCircle.setup("Circle",false));
	primitivesGroupe.add(toggleDrawRectangle.setup("Rectangle",false));
	primitivesGroupe.add(toggleDrawLine.setup("Line", false));
	primitivesGroupe.add(toggleDrawEllipse.setup("Ellipse", false));
	primitivesGroupe.add(toggleDrawBezier.setup("Bezier", false));
	primitivesGroupe.add(drawSphere.setup("Sphere", false)); 
	primitivesGroupe.add(drawCube.setup("Cube", false));
	
	// Associer des fonctions de rappel aux boutons
	toggleDrawTriangle.addListener(this, &Application::button_triangle);
	toggleDrawCircle.addListener(this, &Application::button_circle);
	toggleDrawRectangle.addListener(this, &Application::button_rectangle);
	toggleDrawLine.addListener(this, &Application::button_line);
	toggleDrawEllipse.addListener(this, &Application::button_ellipse);
	toggleDrawBezier.addListener(this, &Application::button_bezier);
	drawSphere.addListener(this, &Application::button_sphere); 
	drawCube.addListener(this, &Application::button_cube);


	drawingGUI.add(&primitivesGroupe);
	drawingGUI.add(renderer.uiPosition.set("position", ofVec2f(0), ofVec2f(0), ofVec2f(ofGetWidth(), ofGetHeight()))); // La position des primitives
	drawingGUI.add(renderer.uiAmount.set("amount", 1, 0, 5));
	drawingGUI.add(renderer.uiStep.set("step", ofVec2f(0), ofVec2f(0), ofVec2f(300)));
	drawingGUI.add(renderer.uiRotate.set("rotate", ofVec3f(0), ofVec3f(-180), ofVec3f(180))); // La rotation des primitives
	drawingGUI.add(renderer.uiShift.set("shift", ofVec2f(0), ofVec2f(0), ofVec2f(300)));
	drawingGUI.add(renderer.uiSize.set("size", ofVec2f(1), ofVec2f(1), ofVec2f(10)));


	camera_setup_perspective(WIDTH, HEIGHT, 60.0f, 0.0f, 0.0f);
	cam.enableOrtho();
	orthoEnabled = true;
	reset_cam();
	setupCamera();
	is_visible_camera = true;
	

	draw_triangle = false;
	draw_circle = false;
	draw_rectangle = false;
	rotation_activate = false;
	mesh_activate = false;
	noise_activate = false;
	catmullRom_activate = false;
	catmullRom6_activate = false;
	hermite_activate = false;

	reinitialisationGroupe.setup("Reinitialisation");
	reinitialisationGroupe.add(resetButton.setup("Reset", false));
	resetButton.addListener(this, &Application::reset);
	drawingGUI.add(&reinitialisationGroupe);

	animationGroupe.setup("Animations");
	animationGroupe.add(rotationButton.setup("Rotation", false));
	rotationButton.addListener(this, &Application::button_rotation);
	drawingGUI.add(&animationGroupe);

	curveGui.setup("Curve");
	curveGui.loadFont("roboto/Roboto-Regular.ttf", 10);
	curveGui.setPosition(600, 50);
	curveGui.add(segments.set("Segments", 150, 150, 400));
	catmullRomGroupe.setup("Catmull-Rom's curve");
	catmullRomButton.setName("5 points");
	catmullRomGroupe.add(catmullRomButton);
	catmullRomButton.addListener(this, &Application::button_catmullRom);
	catmullRomButton6.setName("6 points");
	catmullRomGroupe.add(catmullRomButton6);
	catmullRomButton6.addListener(this, &Application::button_catmullRom6);
	curveGui.add(&catmullRomGroupe);
	hermiteGroupe.setup("Hermite's curve");
	hermiteButton.setName("3 curves");
	hermiteGroupe.add(hermiteButton);
	hermiteButton.addListener(this, &Application::button_hermite);
	curveGui.add(&hermiteGroupe);

	// CrÃƒÂ©ation de la maille
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
		draw_line = draw_ellipse = draw_bezier = draw_sphere = false;
	

	shapeBool = false; 
	v_buttons_ptr = &v_buttons;
	guiScene.setup();
	guiScene.setPosition(0, 40);

	addAction([this]() { undo(); }, [this]() { redo(); });

	controlPoints.push_back(ofVec2f(100, 300));
	controlPoints.push_back(ofVec2f(200, 100));
	controlPoints.push_back(ofVec2f(300, 500));
	controlPoints.push_back(ofVec2f(400, 200));
	controlPoints.push_back(ofVec2f(500, 400));
	controlPoints.push_back(ofVec2f(600, 100));
	controlPoints.push_back(ofVec2f(700, 300));
	controlPoints.push_back(ofVec2f(800, 200));
	controlPoints.push_back(ofVec2f(900, 200));
	controlPoints.push_back(ofVec2f(1000, 100));
	controlPoints.push_back(ofVec2f(1100, 100));
	controlPoints.push_back(ofVec2f(1200, 200));

	// Aucun point de contrôle n'est sélectionné au début
	selectedPointIndex = -1;

	// Modele Illumination
	is_key_press_up = is_key_press_down = is_key_press_left = is_key_press_right = false; 
}

void Application::update()
{
	// Rotations des primitives vectorielles
	rotate++;

	
	if (renderer.isRecording) {
		// Mettez Ã¯Â¿Â½ jour et capturez l'image Ã¯Â¿Â½ intervalles rÃ¯Â¿Â½guliers
		renderer.captureImage();
	}


	if (moveCameraLeft) {
		cam.move(-1, 0, 0); // DÃ©placer la camÃ©ra vers la gauche
	}

	if (moveCameraRight) {
		cam.move(1, 0, 0); // DÃ©placer la camÃ©ra vers la droite
	}

	if (moveCameraUp) {
		cam.move(0, 1, 0); // DÃ©placer la camÃ©ra vers le haut
	}

	if (moveCameraDown) {
		cam.move(0, -1, 0); // DÃ©placer la camÃ©ra vers le bas
	}
	if (moveCameraNear) {
		cam.move(0, 0, 1); // DÃ©placer la camÃ©ra en s'approchant
	}

	if (moveCameraFar) {
		cam.move(0, 0, -1); // DÃ©placer la camÃ©ra en s'eloignant
	}

	/// Modele Illumination
	time_current = ofGetElapsedTimef();
	time_elapsed = time_current - time_last;
	time_last = time_current;
	if (is_key_press_up)
		renderer.offset_z += renderer.delta_z * time_elapsed;
	if (is_key_press_down)
		renderer.offset_z -= renderer.delta_z * time_elapsed;
	if (is_key_press_left)
		renderer.offset_x += renderer.delta_x * time_elapsed;
	if (is_key_press_right)
		renderer.offset_x -= renderer.delta_x * time_elapsed;

	renderer.update();
}


void Application::draw(){
	renderer.interface.drawBackground();
	if (isImportable) {
		renderer.interface.import_activate = true;
		ofDrawBitmapString("Please drag an image to import it.", 30, 70);
	}
	//cam.begin(); //TODO: ***TROUVER UN MOYEN DE RELIER LES DEUX CAMERA POUR PASSER DU CIRCUIT A CELLE ORTHOGRAPHIQUE***
	if (renderer.interface.orthoIsActive) {
		if (renderer.interface.orthoRendering) {
			cam.enableOrtho();
		}
		else if (renderer.interface.perspRendering) {
			cam.disableOrtho();
		}
		cam.begin();
	}
	else if (renderer.interface.angleIsActive) {
		if (renderer.interface.frontCamRendering) {
			camera_active = Camera::front;
			setupCamera();
		}
		else if (renderer.interface.backCamRendering) {
			camera_active = Camera::back;
			setupCamera();
		}
		else if (renderer.interface.leftCamRendering) {
			camera_active = Camera::left;
			setupCamera();
		}
		else if (renderer.interface.rightCamRendering) {
			camera_active = Camera::right;
			setupCamera();
		}
		else if (renderer.interface.topCamRendering) {
			camera_active = Camera::top;
			setupCamera();
		}
		else if (renderer.interface.bottomCamRendering) {
			camera_active = Camera::down;
			setupCamera();
		}

		camera->begin();

		if (is_visible_camera)
		{
			if (camera_active != Camera::front)
				camFront.draw();
			if (camera_active != Camera::back)
				camBack.draw();
			if (camera_active != Camera::left)
				camLeft.draw();
			if (camera_active != Camera::right)
				camRight.draw();
			if (camera_active != Camera::top)
				camTop.draw();
			if (camera_active != Camera::down)
				camBottom.draw();
		}
	}

	if (renderer.interface.import_activate) {
		filterGUI.setup();
		filterGUI.setPosition(800, 40);
		textureGroupe.setup("Filtres");
		textureGroupe.add(sphereTextureButton.setup("Sphere", false));
		sphereTextureButton.addListener(this, &Application::button_sphereTexture);
		filterGUI.add(&textureGroupe);
	}

	renderer.interface.backgroundLine();

	renderer.draw();
	
	//ofPopMatrix();
	if (renderer.interface.orthoIsActive) {
		cam.end();
	}
	else if (renderer.interface.angleIsActive) {
		camera->end();
	}

	renderer.interface.draw();
	drawingGUI.draw();
	//cam.end();
	ofPopMatrix();

	renderer.interface.draw();

	if (drawingGUIPressed) {
		drawingGUI.draw();
	}

	if (renderer.interface.curve_activate) {
		curveGui.draw();
		if (catmullRom_activate) {
			ofSetColor(255);
			for (auto& p : controlPoints) {
				ofDrawCircle(p, 5);
			}

			ofSetColor(255, 0, 0);
			for (auto& p : controlPoints) {
				ofDrawCircle(p, 2);
			}

			ofSetColor(255, 0, 0);
			for (unsigned int i = 0; i < controlPoints.size() - 4; i ++) { 
				for (int j = 0; j <= segments; j++) {
					float t = (float)j / segments;
					ofVec2f p0 = controlPoints[i];
					ofVec2f p1 = controlPoints[i + 1];
					ofVec2f p2 = controlPoints[i + 2];
					ofVec2f p3 = controlPoints[i + 3];
					ofVec2f p4 = controlPoints[i + 4];
					ofVec2f p = catmullRom(t, p0, p1, p2, p3, p4);
					ofDrawCircle(p, 2);
			    }
		    }
		}
		if (catmullRom6_activate) {
			ofSetColor(255);
			for (auto& p : controlPoints) {
				ofDrawCircle(p, 5);
			}

			ofSetColor(255, 0, 0);
			for (auto& p : controlPoints) {
				ofDrawCircle(p, 2);
			}

			ofSetColor(255, 0, 0);
			for (unsigned int i = 1; i < controlPoints.size() - 4; i ++) {
				for (int j = 0; j <= segments; j++) {
					float t = (float)j / segments;
					ofVec2f p0 = controlPoints[i - 1];
					ofVec2f p1 = controlPoints[i];
					ofVec2f p2 = controlPoints[i + 1];
					ofVec2f p3 = controlPoints[i + 2];
					ofVec2f p4 = controlPoints[i + 3];
					ofVec2f p5 = controlPoints[i + 4];
					ofVec2f p = catmullRom6(t, p0, p1, p2, p3, p4, p5);
					ofDrawCircle(p, 2);
				}
			}
		}
		if (hermite_activate) {
			ofSetColor(255);
			for (auto& p : controlPoints) {
				ofDrawCircle(p, 5);
			}

			ofSetColor(255, 0, 0);
			for (auto& p : controlPoints) {
				ofDrawCircle(p, 2);
			}

			ofSetColor(255, 0, 0);
			for (int i = 0; i < controlPoints.size() - 4; i++) { 
				for (int j = 0; j <= segments; j++) {
					float t = (float)j / segments;
					ofVec2f p0 = controlPoints[i];
					ofVec2f p1 = controlPoints[i + 1];
					ofVec2f p2 = controlPoints[i + 2];
					ofVec2f p3 = controlPoints[i + 3];
					ofVec2f p4 = controlPoints[i + 4];
					float x, y;
					hermite(t, p0, p1, p2, p3, p4, x, y);
					ofDrawCircle(x, y, 2);
				}
			}
		}
	}

	guiScene.draw();
}

void Application::toggleDrawingGUI(Forme::TypeForme drawingShape) {
	if (lastShape == drawingShape) {
		drawingGUIPressed = !drawingGUIPressed;
	}
	else {
		lastShape = drawingShape;
		drawingGUIPressed = true;
	}
}


// a modifier ou effacer 
void Application::deleteShapeSelected()
{
	// Vecteur temporaire pour stocker les indices des boutons Ã¯Â¿Â½ supprimer
	vector<int> buttonsToDelete;
	// Vecteur pour stocker l'Ã¯Â¿Â½tat de chaque bouton
	vector<bool> buttonStates;

	for (const auto& b : *v_buttons_ptr)
	{
		for (int i = 0; i < v_buttons.size(); ++i)
		{
			// VÃ¯Â¿Â½rifier si le bouton est en Ã¯Â¿Â½tat TRUE
			if (i == shapeBool) // v_buttons[i] AccÃ¯Â¿Â½der Ã¯Â¿Â½ l'Ã¯Â¿Â½tat boolÃ¯Â¿Â½en du bouton
			{
				//cout << "Il est cense avoir " << i << " forme a effacer" << endl;
				// Ajouter l'index du bouton Ã¯Â¿Â½ supprimer dans le vecteur temporaire
				buttonsToDelete.push_back(i);
			}
		}

	}


	if (!buttonsToDelete.empty())
	{
		//cout << "Deletion is possible! " << endl;
		//cout << "Size of the deletion list is " << buttonsToDelete.size() << endl;
	}

}

void Application::keyPressed(int key) 
{

	// Pour tester les pointeurs 
	if (key == 'f') 
	{
		for (const auto& ptr : *renderer.v_formes_ptr) 
		{
			//cout << "Adresse du pointeur : " << &ptr << endl;
			//cout << "Adresse de l'objet Forme : " << ptr.get() << endl; // Obtenir l'adresse de l'objet pointé
			//cout << "Valeur de l'objet Forme : " << ptr << endl;
			//cout << endl;
		}		
	}

	// DÃ¯Â¿Â½marrer/arrÃ¯Â¿Â½ter l'enregistrement lors de l'appui sur la touche 'r'
	if (key == 'r') 
	{
		renderer.isRecording = !renderer.isRecording;
		if (renderer.isRecording) 
		{
			renderer.frameCounter = 0; // RÃ¯Â¿Â½initialiser le compteur de frames lors du dÃ¯Â¿Â½marrage de l'enregistrement
			std::cout << "Enregistrement demarrer." << endl;
			//renderer.update();
			//renderer.captureImage();
		}
		else {
			std::cout << "Enregistrement arreter." << endl;
		}
	}


	if(key == 'z')
	{ 
		if(!v_buttons.empty() && !renderer.v_formes.empty())
		{ 
			//undo();
		}
	}
	if(key == 'x')
	{ 
		if (!v_buttons.empty() && !renderer.v_formes.empty())
		{
			//redo(); 
		}
	}

	if(renderer.isModeIllumination == false)
	{ 
		if (key == OF_KEY_LEFT) {
			moveCameraLeft = true;
		}
		if (key == OF_KEY_RIGHT) {
			moveCameraRight = true;
		}
		if (key == OF_KEY_UP) {
			moveCameraUp = true;
		}
		if (key == OF_KEY_DOWN) {
			moveCameraDown = true;
		}
		if (key == 49) {
			moveCameraNear = true;
		}
		if (key == 50) {
			moveCameraFar = true;
		}
	}
	// Modele illumination
	if (renderer.isModeIllumination)
	{
		switch (key)
		{
		case OF_KEY_LEFT: // touche ?
			is_key_press_left = true;
			break;

		case OF_KEY_UP: // touche ?
			is_key_press_up = true;
			break;

		case OF_KEY_RIGHT: // touche ?
			is_key_press_right = true;
			break;

		case OF_KEY_DOWN: // touche ?
			is_key_press_down = true;
			break;

		default:
			break;
		}
	}

}

void Application::keyReleased(int key){
	// C'est pour activer ou desactiver le modeIllumination, 
	// tout autre option alternative est envisageable ! 
	//if ((key = '`') || (key = '#')) {
	//	/*renderer.isModeIllumination = !renderer.isModeIllumination;
	//	cout << "le bouton est clique" << endl;*/
	//}
	
	if(renderer.isModeIllumination == false)
	{ 
		if (key == 105) { // 105 = key "i"
			isImportable = !isImportable;
			renderer.interface.import_activate = !renderer.interface.import_activate;
		}
		if (key == OF_KEY_LEFT) {
			moveCameraLeft = false;
		}
		if (key == OF_KEY_RIGHT) {
			moveCameraRight = false;
		}
		if (key == OF_KEY_UP) {
			moveCameraUp = false;
		}
		if (key == OF_KEY_DOWN) {
			moveCameraDown = false;
		}
		if (key == 49) { // 49 = touche 1
			moveCameraNear = false;
		}
		if (key == 50) { // 50 = touche 2
			moveCameraFar = false;
		}
	}
	/// Modele illumination 
	if (renderer.isModeIllumination)
	{
		switch (key)
		{
		case 49: // touche 1
			renderer.shader_active = ShaderType::color_fill;
			ofLog() << "<shader: color fill>";
			break;

		case 50: // touche 2
			renderer.shader_active = ShaderType::lambert;
			ofLog() << "<shader: lambert>";
			break;

		case 51: // touche 3
			renderer.shader_active = ShaderType::gouraud;
			ofLog() << "<shader: gouraud>";
			break;

		case 52: // touche 4
			renderer.shader_active = ShaderType::phong;
			ofLog() << "<shader: phong>";
			break;

		case 53: // touche 5
			renderer.shader_active = ShaderType::blinn_phong;
			ofLog() << "<shader: blinn-phong>";
			break;

		case 114: // touche r
			renderer.reset();
			break;

		case OF_KEY_LEFT: // touche ?
			is_key_press_left = false;
			break;

		case OF_KEY_UP: // touche ?
			is_key_press_up = false;
			break;

		case OF_KEY_RIGHT: // touche ?
			is_key_press_right = false;
			break;

		case OF_KEY_DOWN: // touche ?
			is_key_press_down = false;
			break;

		default:
			break;
		}
	}

	/*if (key == 'n') {
		if (orthoEnabled) {
			cam.disableOrtho();
			orthoEnabled = false;
		}
		else {
			cam.enableOrtho();
			orthoEnabled = true;
		}
	}
	switch (key) {
		case 51: // touche 3
			camera_active = Camera::front;
			setupCamera();
			break;

		case 52: // touche 4
			camera_active = Camera::back;
			setupCamera();
			break;

		case 53: // touche 5
			camera_active = Camera::left;
			setupCamera();
			break;

		case 54: // touche 6
			camera_active = Camera::right;
			setupCamera();
			break;

		case 55: // touche 7
			camera_active = Camera::top;
			setupCamera();
			break;

		case 56: // touche 8
			camera_active = Camera::down;
			setupCamera();
			break;

		default:
			break;

	}*/
}

void Application::mouseMoved(int x, int y ){
	renderer.interface.mouse_current_x = x;
	renderer.interface.mouse_current_y = y;
}

void Application::mouseDragged(int x, int y, int button){
	renderer.interface.mouse_current_x = x;
	renderer.interface.mouse_current_y = y;

	renderer.interface.mouse_drag_x = x;
	renderer.interface.mouse_drag_y = y;

	renderer.interface.is_mouse_button_dragged = true;
	renderer.interface.is_mouse_button_pressed = false;

	if (draw_line)
	{
		renderer.ligne.addVertex(renderer.interface.mouse_drag_x, renderer.interface.mouse_drag_y);
	}

	if (selectedPointIndex != -1) {
		controlPoints[selectedPointIndex].set(x, y);
	}

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

	for (int i = 0; i < controlPoints.size(); i++) {
		float distance = ofDist(x, y, controlPoints[i].x, controlPoints[i].y);
		if (distance < 5) {
			selectedPointIndex = i;
			break;
		}
	}

	renderer.interface.is_mouse_button_pressed = true;
	renderer.interface.is_mouse_button_dragged = false;
	renderer.interface.mouse_current_x = x;
	renderer.interface.mouse_current_y = y;
	renderer.interface.mouse_press_x = x;
	renderer.interface.mouse_press_y = y;

	// Pour les spheres et cubes, peuvent etre ameliore plus tard
	renderer.interface.mouse_press_x = x;
	renderer.interface.mouse_press_y = y;

	if(draw_triangle) {
		// A partir du mouse click, calcul des 2 autres sommets 
		newX2 = renderer.interface.mouse_press_x - diffX;
		newY2 = renderer.interface.mouse_press_y + diffY;
		newX3 = renderer.interface.mouse_press_x + diffX;
		newY3 = renderer.interface.mouse_press_y + diffY;
		forme.setX1(x);
		forme.setY1(y);
		forme.setX2(newX2);
		forme.setY2(newY2);
		forme.setX3(newX3);
		forme.setY3(newY3);
		forme.setColors(renderer.interface.color_picker_stroke, renderer.interface.colorPickerFill);
		forme.setIsFilled(renderer.interface.fillEnabled);
		forme.setOutlineWeight(renderer.interface.slider_stroke_weight);

		renderer.v_formes.push_back(make_unique<Forme>(Forme::TRIANGLE, forme.getX1(), forme.getY1(),
			forme.getX2(), forme.getY2(),
			forme.getX3(), forme.getY3(), forme.getColors(), forme.getIsFilled(), forme.getOutlineWeight()));

		renderer.okDessiner = true;
		renderer.triangleColors = { renderer.interface.color_picker_stroke, renderer.interface.colorPickerFill }; // Ajuste les parametres
		renderer.triangleFill = renderer.interface.fillEnabled;
		renderer.triangleStroke = renderer.interface.slider_stroke_weight;
		//ofxToggle button;
		auto button = make_unique<ofxToggle>();
		//guiScene.add(button->setup("TRIANGLE", shapeBool)); // Nom du bouton
		v_buttons.push_back(move(button)); // Ajoutez le bouton ï¿½ la liste des boutons
		guiScene.add(triangle);
	}

	if(draw_circle)
	{ 
		forme.setXC(renderer.interface.mouse_press_x);
		forme.setYC(renderer.interface.mouse_press_y);
		forme.setColors(renderer.interface.color_picker_stroke, renderer.interface.colorPickerFill);
		forme.setIsFilled(renderer.interface.fillEnabled);
		forme.setOutlineWeight(renderer.interface.slider_stroke_weight);
		renderer.v_formes.push_back(make_unique<Forme>(Forme::CERCLE, forme.getXC(), forme.getYC(), forme.getRayon(), 
			forme.getColors(), forme.getIsFilled(), forme.getOutlineWeight()));
		//ofSetCircleResolution(55);
		renderer.okDessiner = true; 
		renderer.cercleColors = { renderer.interface.color_picker_stroke, renderer.interface.colorPickerFill }; // Ajuste les parametres
		renderer.cercleFill = renderer.interface.fillEnabled;
		renderer.cercleStroke = renderer.interface.slider_stroke_weight;
		//ofxToggle button;
		auto button = make_unique<ofxToggle>();
		//guiScene.add(button->setup("CERCLE", false)); // Nom du bouton
		v_buttons.push_back(move(button)); // Ajoutez le bouton ï¿½ la liste des boutons

		guiScene.add(circle);
	}

	if(draw_rectangle)
	{ 
		forme.setXR(renderer.interface.mouse_current_x);
		forme.setYR(renderer.interface.mouse_current_y);
		forme.setColors(renderer.interface.color_picker_stroke, renderer.interface.colorPickerFill);
		forme.setIsFilled(renderer.interface.fillEnabled);
		forme.setOutlineWeight(renderer.interface.slider_stroke_weight);
		renderer.v_formes.push_back(make_unique<Forme>(Forme::RECTANGLE, forme.getXR(), forme.getYR(), forme.getWidth(), forme.getHeight(),
			forme.getColors(), forme.getIsFilled(), forme.getOutlineWeight()));
		renderer.okDessiner = true;
		renderer.rectangleColors = { renderer.interface.color_picker_stroke, renderer.interface.colorPickerFill }; // Ajuste les parametres
		renderer.rectangleFill = renderer.interface.fillEnabled;
		renderer.rectangleStroke = renderer.interface.slider_stroke_weight;
		//ofxToggle button;
		auto button = make_unique<ofxToggle>();
		//guiScene.add(button->setup("RECTANGLE", false)); // Nom du bouton
		v_buttons.push_back(move(button)); // Ajoutez le bouton ï¿½ la liste des boutons
		
		guiScene.add(rectangle);
	}

	if (draw_line)
	{
		//renderer.vecteur_lignes_ptr->emplace_back(make_unique<ofPolyline>());
		//auto& polyline = renderer.vecteur_lignes_ptr->back();
		//polyline->addVertex(renderer.mouse_press_x, renderer.mouse_press_y);

		renderer.ligne.addVertex(renderer.interface.mouse_press_x, renderer.interface.mouse_press_y);
		renderer.okDessiner = true;
		renderer.ligneColor = renderer.interface.color_picker_stroke; // Ajuste les parametres
		renderer.ligneStroke = renderer.interface.slider_stroke_weight;
		auto button = make_unique<ofxToggle>();
		//guiScene.add(button->setup("LIGNE", false)); // Nom du bouton
		v_buttons.push_back(move(button)); // Ajoutez le bouton ï¿½ la liste des boutons
		
		guiScene.add(line);
	}

	if (draw_ellipse)
	{
		forme.setXR(renderer.interface.mouse_press_x);
		forme.setYR(renderer.interface.mouse_press_y);
		forme.setColors(renderer.interface.color_picker_stroke, renderer.interface.colorPickerFill);
		forme.setIsFilled(renderer.interface.fillEnabled);
		forme.setOutlineWeight(renderer.interface.slider_stroke_weight);
		renderer.v_formes.push_back(make_unique<Forme>(Forme::ELLIPSE, forme.getXR(), forme.getYR(), forme.getWidth(), forme.getHeight(),
			forme.getColors(), forme.getIsFilled(), forme.getOutlineWeight()));
		renderer.okDessiner = true;
		renderer.ellipseColors = { renderer.interface.color_picker_stroke, renderer.interface.colorPickerFill }; // Ajuste les parametres
		renderer.ellipseFill = renderer.interface.fillEnabled;
		renderer.ellipseStroke = renderer.interface.slider_stroke_weight;
		auto button = make_unique<ofxToggle>();
		//guiScene.add(button->setup("ELLIPSE", false)); // Nom du bouton
		v_buttons.push_back(move(button)); // Ajoutez le bouton ï¿½ la liste des boutons
		
		guiScene.add(ellipse);
	}

	if(draw_bezier)
	{
		float x1 = renderer.interface.mouse_press_x;
		float y1 = renderer.interface.mouse_press_y;
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
		forme.setColors(renderer.interface.color_picker_stroke, renderer.interface.colorPickerFill);
		forme.setIsFilled(renderer.interface.fillEnabled);
		forme.setOutlineWeight(renderer.interface.slider_stroke_weight);

		renderer.v_formes.push_back(make_unique<Forme>(Forme::BEZIER, forme.getX1(), forme.getY1(), forme.getXB1(), forme.getYB1(),
			forme.getXB2(), forme.getYB2(), forme.getX2(), forme.getY2(), forme.getColors(), forme.getIsFilled(), forme.getOutlineWeight()));
		renderer.okDessiner = true;
		renderer.bezierColors = { renderer.interface.color_picker_stroke, renderer.interface.colorPickerFill }; // Ajuste les parametres
		renderer.bezierFill = renderer.interface.fillEnabled;
		renderer.bezierStroke = renderer.interface.slider_stroke_weight;
		auto button = make_unique<ofxToggle>();
		//guiScene.add(button->setup("BEZIER", false)); // Nom du bouton
		v_buttons.push_back(move(button)); // Ajoutez le bouton ï¿½ la liste des boutons
		
		guiScene.add(bezier);
	}
  
		if (draw_sphere) //  && drawSphere
		{
			float x = renderer.interface.mouse_press_x / 1.00;
			float y = renderer.interface.mouse_press_y / 1.00;
			ofVec3f viktor(x, y, 0);
			forme.setVSphere(viktor); 
			//forme.setXS(x);
			//forme.setYS(y);
			renderer.v_formes.push_back(make_unique<Forme>(Forme::SPHERE, forme.getVSphere(), forme.getSphereRad()));
			//renderer.v_formes.push_back(make_unique<Forme>(Forme::SPHERE, forme.getXS(), forme.getYS(), 0, forme.getSphereRad()));
			renderer.okDessiner = true; 
			auto button = make_unique<ofxToggle>(); 
			//guiScene.add(button->setup("SPHERE", false)); 
			v_buttons.push_back(move(button)); 

			guiScene.add(sphere);
		}

		if (draw_cube) //  && drawCube
		{
			float x = renderer.interface.mouse_press_x;
			float y = renderer.interface.mouse_press_y;
			ofVec3f viktor(x, y, 0);
			forme.setVSphere(viktor);
			forme.setSizeCube(150);
			renderer.v_formes.push_back(make_unique<Forme>(Forme::CUBE, forme.getVSphere(), forme.getSizeCube()));
			renderer.okDessiner = true;
			auto button = make_unique<ofxToggle>();
			//guiScene.add(button->setup("CUBE", false));
			v_buttons.push_back(move(button));

			guiScene.add(cube);
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

	selectedPointIndex = -1;

	if (button == 0 && y < INTERACTION_BAR_HEIGHT) {
		int index = static_cast<int>(floor(x / (iconWidth)));
		switch (index) {
			case 0:
				//call to import image method
				isImportable = !isImportable;
				renderer.interface.import_activate = !renderer.interface.import_activate;
				break;
			case 1:
				//call to export method
				renderer.toggleExportGUI();
				break;
			case 2:
				//call to animation method
				cout << "animation \n";
				break;
			case 3:
				//call to mesh
				cout << "mesh \n";
				renderer.interface.toggleMailleGUI();
				break;
			case 4:
				renderer.interface.toggleModelOptions();
				cout << "modele \n";
				break;
			case 5:
				renderer.interface.toggleCamOptions();
				break;
			case 6:
				renderer.interface.toggleCurveOptions();
				renderer.interface.curve_activate = !renderer.interface.curve_activate;
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
			toggleDrawingGUI(forme.LIGNE);
			drawLine();
			break;
		case 2:
			//call to ellipse method
			toggleDrawingGUI(forme.CERCLE);
			drawCircle();
			break;
		case 3:
			//call to rectangle method
			toggleDrawingGUI(forme.RECTANGLE);
			drawRectangle();
			break;
		case 4:
			//call to triangle method
			toggleDrawingGUI(forme.TRIANGLE);
			drawTriangle();
			break;
		case 5:
			//call to ellipse method
			toggleDrawingGUI(forme.ELLIPSE);
			drawEllipse();
			break;
		}
	}

	renderer.interface.mouse_current_x = x;
	renderer.interface.mouse_current_y = y;


	renderer.mouse_current_x = x;
	renderer.mouse_current_y = y;


	if (draw_line)

	{
		renderer.interface.is_mouse_button_dragged = true;
		renderer.ligne.addVertex(x, y);
		renderer.vecteur_lignes.push_back(renderer.ligne);
		renderer.okDessiner = true;
		renderer.ligne.clear();
	}

	renderer.interface.is_mouse_button_pressed = false;
	renderer.interface.is_mouse_button_dragged = false;
}


void Application::mouseEntered(int x, int y){
	renderer.interface.mouse_current_x = x;
	renderer.interface.mouse_current_y = y;
}

void Application::mouseExited(int x, int y){

	renderer.interface.mouse_current_x = x;
	renderer.interface.mouse_current_y = y;
}

void Application::windowResized(int w, int h){
	WIDTH = w;
	HEIGHT = h;
}

void Application::gotMessage(ofMessage msg){
	//ofEasyCam cam;

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

void Application::button_triangle(bool& value) 
{ 
	if (value)
	{
		draw_triangle = !draw_triangle;
		draw_circle = draw_rectangle = draw_line = draw_ellipse = draw_bezier = draw_sphere = draw_cube = false;
		draw_sphere = draw_cube = false;
		//drawCircle = drawRectangle = drawLine = drawEllipse = drawBezier = drawSphere = drawCube = false;
		if (!renderer.triangleColors.empty())
		{ // Conserve les parametres de la forme pour la reselection
			renderer.interface.colorPickerFill = renderer.triangleColors[1];
			renderer.interface.color_picker_stroke = renderer.triangleColors[0];
			if (renderer.interface.fillButton != renderer.triangleFill)
			{
				renderer.interface.fillButton = renderer.triangleFill;
			}
			renderer.interface.slider_stroke_weight = renderer.triangleStroke;
		}
	}
}

void Application::drawTriangle() 
{
	cout << "drawTriangle 2 \n";
	draw_triangle = !draw_triangle;
	draw_circle = draw_rectangle = draw_line = draw_ellipse = draw_bezier = false;
	draw_sphere = draw_cube = false;
	if (!renderer.triangleColors.empty()) 
	{ // Conserve les parametres de la forme pour la reselection
		renderer.interface.colorPickerFill = renderer.triangleColors[1];
		renderer.interface.color_picker_stroke = renderer.triangleColors[0];
		if (renderer.interface.fillButton != renderer.triangleFill) 
		{
			renderer.interface.fillButton = renderer.triangleFill;

		}
		renderer.interface.slider_stroke_weight = renderer.triangleStroke;
	}
}

void Application::button_circle(bool& value)
{
	if (value)
	{
		draw_circle = !draw_circle;
		draw_triangle = draw_rectangle = draw_line = draw_ellipse = draw_bezier = draw_sphere = draw_cube = false;
		draw_sphere = draw_cube = false;
		//drawTriangle = drawRectangle = drawLine = drawEllipse = drawBezier = drawSphere = drawCube = false;
		if (!renderer.cercleColors.empty())
		{// Conserve les parametres de la forme pour la reselection
			renderer.interface.colorPickerFill = renderer.cercleColors[1];
			renderer.interface.color_picker_stroke = renderer.cercleColors[0];
			if (renderer.interface.fillButton != renderer.cercleFill) {
				renderer.interface.fillButton = renderer.cercleFill;
			}
			renderer.interface.slider_stroke_weight = renderer.cercleStroke;
		}
	}
}

void Application::drawCircle() 
{
	draw_circle = !draw_circle;
	draw_triangle = draw_rectangle = draw_line = draw_ellipse = draw_bezier = false;
	draw_sphere = draw_cube = false;
	if (!renderer.cercleColors.empty()) 
	{// Conserve les parametres de la forme pour la reselection
		renderer.interface.colorPickerFill = renderer.cercleColors[1];
		renderer.interface.color_picker_stroke = renderer.cercleColors[0];
		if (renderer.interface.fillButton != renderer.cercleFill) 
		{
			renderer.interface.fillButton = renderer.cercleFill;

		}
		renderer.interface.slider_stroke_weight = renderer.cercleStroke;
	}
}


void Application::button_rectangle(bool& value)
{
	if (value)
	{
		draw_rectangle = !draw_rectangle;
		draw_triangle = draw_circle = draw_line = draw_ellipse = draw_bezier = draw_sphere = draw_cube = false;
		draw_sphere = draw_cube = false;
		//drawTriangle = drawCircle = drawLine = drawEllipse = drawBezier = drawSphere = drawCube = false;
		if (!renderer.rectangleColors.empty())
		{ // Conserve les parametres de la forme pour la reselection
			renderer.interface.colorPickerFill = renderer.rectangleColors[1];
			renderer.interface.color_picker_stroke = renderer.rectangleColors[0];
			if (renderer.interface.fillButton != renderer.rectangleFill) {
				renderer.interface.fillButton = renderer.rectangleFill;
			}
			renderer.interface.slider_stroke_weight = renderer.rectangleStroke;
		}
	}
}

void Application::drawRectangle() 
{
	draw_rectangle = !draw_rectangle;
	draw_triangle = draw_circle = draw_line = draw_ellipse = draw_bezier = false;
	draw_sphere = draw_cube = false;
	if (!renderer.rectangleColors.empty()) 
	{ // Conserve les parametres de la forme pour la reselection
		renderer.interface.colorPickerFill = renderer.rectangleColors[1];
		renderer.interface.color_picker_stroke = renderer.rectangleColors[0];
		if (renderer.interface.fillButton != renderer.rectangleFill) 
		{
			renderer.interface.fillButton = renderer.rectangleFill;
		}
		renderer.interface.slider_stroke_weight = renderer.rectangleStroke;
	}
}


void Application::button_line(bool& value) 
{
	if (value) 
	{
		draw_line = !draw_line;
		draw_triangle = draw_rectangle = draw_circle = draw_ellipse = draw_bezier = draw_sphere = draw_cube = false;
		draw_sphere = draw_cube = false;
		//drawTriangle = drawRectangle = drawCircle = drawEllipse = drawBezier = drawSphere = drawCube = false;
		renderer.interface.color_picker_stroke = renderer.ligneColor;
		renderer.interface.slider_stroke_weight = renderer.ligneStroke;
		
	}
}

void Application::button_ellipse(bool& value)
{
	if (value) 
	{
		draw_ellipse = !draw_ellipse;
		draw_triangle = draw_rectangle = draw_circle = draw_line = draw_bezier = draw_sphere = draw_cube = false;
		draw_sphere = draw_cube = false;
		//drawTriangle = drawRectangle = drawCircle = drawLine = drawBezier = drawSphere = drawCube = false;
		if (!renderer.ellipseColors.empty()) 
		{ // Conserve les parametres de la forme pour la reselection
			renderer.interface.colorPickerFill = renderer.ellipseColors[1];
			renderer.interface.color_picker_stroke = renderer.ellipseColors[0];
			if (renderer.interface.fillButton != renderer.ellipseFill) {
				renderer.interface.fillButton = renderer.ellipseFill;
			}
			renderer.interface.slider_stroke_weight = renderer.ellipseStroke;
		}
	}
}

void Application::drawLine() {
	draw_line = !draw_line;
	draw_triangle = draw_rectangle = draw_circle = draw_ellipse = draw_bezier = false;
	draw_sphere = draw_cube = false;
	renderer.interface.color_picker_stroke = renderer.ligneColor;
	renderer.interface.slider_stroke_weight = renderer.ligneStroke;
}

void Application::drawEllipse() 
{
	draw_ellipse = !draw_ellipse;
	draw_triangle = draw_rectangle = draw_circle = draw_line = draw_bezier = false;
	draw_sphere = draw_cube = false;
	if (!renderer.ellipseColors.empty()) 
	{ // Conserve les parametres de la forme pour la reselection
		renderer.interface.colorPickerFill = renderer.ellipseColors[1];
		renderer.interface.color_picker_stroke = renderer.ellipseColors[0];
		if (renderer.interface.fillButton != renderer.ellipseFill) 
		{
			renderer.interface.fillButton = renderer.ellipseFill;

		}
		renderer.interface.slider_stroke_weight = renderer.ellipseStroke;
	}
}


void Application::button_bezier(bool& value)
{
	if (value)
	{
		draw_bezier = !draw_bezier;
		draw_triangle = draw_rectangle = draw_circle = draw_line = draw_ellipse = draw_cube = false;
		draw_sphere = draw_cube = false;
		//drawTriangle = drawRectangle = drawCircle = drawLine = drawEllipse = drawCube = false;
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

void Application::drawBezier() {
	draw_bezier = !draw_bezier;
	draw_triangle = draw_rectangle = draw_circle = draw_line = draw_ellipse = false;
	draw_sphere = draw_cube = false;
	if (!renderer.bezierColors.empty()) { // Conserve les parametres de la forme pour la reselection
		renderer.interface.colorPickerFill = renderer.bezierColors[1];
		renderer.interface.color_picker_stroke = renderer.bezierColors[0];
		if (renderer.interface.fillButton != renderer.bezierFill) {
			renderer.interface.fillButton = renderer.bezierFill;

		}
		renderer.interface.slider_stroke_weight = renderer.bezierStroke;
	}
}

void Application::button_sphere(bool& value) {
	if (value) {
		draw_sphere = !draw_sphere; 
		draw_triangle = draw_rectangle = draw_circle = draw_line = draw_ellipse = draw_bezier = draw_cube = false;
		//drawTriangle = drawRectangle = drawCircle = drawLine = drawEllipse = drawBezier = drawCube = false;
	}
}

void Application::button_sphereTexture(bool& value) {
	if (value)
	{
		draw_sphereTexture = !draw_sphereTexture;
		draw_circle = draw_rectangle = draw_line = draw_ellipse = draw_bezier = draw_sphere = draw_cube = draw_triangle = false;
		draw_sphere = draw_cube = false;

	}
}


void Application::button_cube(bool& value) {
	if (value) {
		draw_cube = !draw_cube; 
		draw_triangle = draw_rectangle = draw_circle = draw_line = draw_ellipse = draw_bezier = draw_sphere = false;
		//drawTriangle = drawRectangle = drawCircle = drawLine = drawEllipse = drawBezier = drawSphere = false;
	}
}

void Application::reset(bool& value) {
	if (value) {
		renderer.uiPosition.set(ofVec2f(0));
		renderer.uiAmount.set(1);
		renderer.uiStep.set(ofVec2f(0));
		renderer.uiRotate.set(ofVec3f(0));
		renderer.uiShift.set(ofVec2f(0));
		renderer.uiSize.set(ofVec2f(6));

		draw_sphere = false;
		draw_cube = false;
		draw_triangle = false;
		draw_circle = false;
		draw_rectangle = false;
		resetButton = false;
		rotationButton = false;
		rotation_activate = false;
		mesh_activate = false;
		meshButton = false;
		noise_activate = false;
		meshAnimationButton = false;
		sphereTextureButton = false;
		catmullRomButton = false;
		catmullRom_activate = false;
		hermiteButton = false;
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


void Application::reset_cam() {
	offset_camera = 500.0f * 3.5f * -1.0f;

	// position initiale de chaque camÃ©ra
	camFront.setPosition(0, 0, -offset_camera);
	camBack.setPosition(0, 0, offset_camera);
	camLeft.setPosition(-offset_camera, 0, 0);
	camRight.setPosition(offset_camera, 0, 0);
	camTop.setPosition(0, offset_camera, 0);
	camBottom.setPosition(0, -offset_camera, 0);

	// orientation de chaque camÃ©ra
	camFront.lookAt(camera_target);
	camBack.lookAt(camera_target);
	camLeft.lookAt(camera_target);
	camRight.lookAt(camera_target);
	camTop.lookAt(camera_target, ofVec3f(1, 0, 0));
	camBottom.lookAt(camera_target, ofVec3f(1, 0, 0));

	camFront.setVFlip(true);
	camBack.setVFlip(true);
	camLeft.setVFlip(true);
	camRight.setVFlip(true);
	camTop.setVFlip(true);
	camBottom.setVFlip(true);

	camera_active = Camera::front;
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


void Application::camera_setup_perspective(float width, float height, float fov, float n, float f)
{
	bool camera_projection_persp_or_ortho = true;
	bool camera_vertical_flip = true;

	int camera_viewport_x = width;
	int camera_viewport_y = height;
	float camera_aspect_ratio = camera_viewport_x / camera_viewport_y;

	float camera_fov = fov;
	float camera_zoom = compute_zoom_from_fov(camera_fov);

	float minimal_side = std::min(camera_viewport_x, camera_viewport_y);
	float fov_half = ofDegToRad(camera_fov / 2.0f);
	float distance = minimal_side / 2.0f / tanf(fov_half);

	glm::vec3 camera_position;

	camera_position.x = camera_viewport_x / 2.0f;
	camera_position.y = camera_viewport_y / 2.0f;
	camera_position.z = distance;

	//camera_clip_n = near > 0.0f ? near : distance / 10.0f;
	//camera_clip_f = far > 0.0f ? far : distance * 10.0f;

	float camera_clip_n = distance / 2.0f;
	float camera_clip_f = distance * 1.5f;

	float camera_depth_range = camera_clip_f - camera_clip_n;

	// configurer l'instance de camÃ©ra de openFrameworks (ofCamera)
	cam.setupPerspective(camera_vertical_flip, camera_fov, camera_clip_n, camera_clip_f, ofVec2f(0, 0));
	cam.setPosition(camera_position.x, camera_position.y, camera_position.z);

	bool camera_state_change = false;
}

float Application::compute_zoom_from_fov(float fov)
{
	return 1.0f / tanf(glm::radians(fov) / 2.0f);;
}

void Application::setupCamera() {
	switch (camera_active) {
		case Camera::front:
			camera = &camFront;
			renderer.interface.camera_name = "avant";
			break;

		case Camera::back:
			camera = &camBack;
			renderer.interface.camera_name = "arriÃ¨re";
			break;

		case Camera::left:
			camera = &camLeft;
			renderer.interface.camera_name = "gauche";
			break;

		case Camera::right:
			camera = &camRight;
			renderer.interface.camera_name = "droite";
			break;

		case Camera::top:
			camera = &camTop;
			renderer.interface.camera_name = "haut";
			break;

		case Camera::down:
			camera = &camBottom;
			renderer.interface.camera_name = "bas";
			break;

		default:
			break;
		}
		camera_position = camera->getPosition();
		camera_orientation = camera->getOrientationQuat();

		camera->setPosition(camera_position);
		camera->setOrientation(camera_orientation);
}

void Application::button_catmullRom(bool& value) {
	catmullRom_activate = value;
	if (value) {
		catmullRom_activate = true;
		catmullRomButton6 = false;
		catmullRom6_activate = false;
		hermiteButton = false;
		hermite_activate = false;
	}
}

void Application::button_catmullRom6(bool& value) {
	catmullRom6_activate = value;
	if (value) {
		catmullRom6_activate = true;
		catmullRomButton = false;
		catmullRom_activate = false;
		hermiteButton = false;
		hermite_activate = false;
	}
}

void Application::button_hermite(bool& value) {
	hermite_activate = value;
	if (value) {
		hermite_activate = true;
		catmullRomButton6 = false;
		catmullRom6_activate = false;
		catmullRomButton = false;
		catmullRom_activate = false;
	}
}

void Application::hermite(float t, const ofVec2f& p0, const ofVec2f& p1, const ofVec2f& p2, const ofVec2f& p3, const ofVec2f& p4, float& x, float& y)
{
	float u = 1 - t;
	float uu = u * u;
	float uuu = uu * u;
	float tt = t * t;
	float ttt = tt * t;

	x = (2 * ttt - 3 * tt + 1) * p1.x + (ttt - 2 * tt + t) * p2.x + (ttt - tt) * p3.x + (-2 * ttt + 3 * tt) * p4.x;
	y = (2 * ttt - 3 * tt + 1) * p1.y + (ttt - 2 * tt + t) * p2.y + (ttt - tt) * p3.y + (-2 * ttt + 3 * tt) * p4.y;
}

ofVec2f Application::catmullRom(float t, const ofVec2f& p0, const ofVec2f& p1, const ofVec2f& p2, const ofVec2f& p3, const ofVec2f& p4) {
	float t2 = t * t;
	float t3 = t2 * t;

	float b0 = 0.5f * (-t3 + 2 * t2 - t);
	float b1 = 0.5f * (3 * t3 - 5 * t2 + 2);
	float b2 = 0.5f * (-3 * t3 + 4 * t2 + t);
	float b3 = 0.5f * (t3 - t2);

	return p0 * b0 + p1 * b1 + p2 * b2 + p3 * b3 + p4 * b3;
}

ofVec2f Application::catmullRom6(float t, const ofVec2f& p0, const ofVec2f& p1, const ofVec2f& p2, const ofVec2f& p3, const ofVec2f& p4, const ofVec2f& p5) {
	float t2 = t * t;
	float t3 = t2 * t;

	float b0 = 0.5f * (-t3 + 2 * t2 - t);
	float b1 = 0.5f * (3 * t3 - 5 * t2 + 2);
	float b2 = 0.5f * (-3 * t3 + 4 * t2 + t);
	float b3 = 0.5f * (t3 - t2);

	return p0 * b0 + p1 * b1 + p2 * b2 + p3 * b3 + p4 * b3 + p5 * b3; 
}