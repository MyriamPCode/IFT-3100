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
	


	forme.setup();
	diffX = (abs(forme.getX2() - forme.getX3())) / 2;
	diffY = abs(forme.getY1() - forme.getY2());
	newX2 = 0;
	newY2 = 0;
	newX3 = 0;
	newY3 = 0;

	//draw_line = draw_triangle && draw_circle;
	//draw_ellipse = draw_triangle && draw_rectangle;
	//draw_bezier = draw_rectangle && draw_circle;
	draw_line = draw_ellipse = draw_bezier = false; 
}


void Application::update() {}


void Application::draw(){
	if (isImportable) {
		renderer.import_activate = true;
		ofDrawBitmapString("Please drag an image to import it.", 30, 30);
	}
	

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
		ofBeginShape();
		if (draw_triangle) {
			//ofDrawTriangle(0, 0, -16, 32, 16, 32);
		} 
		if (draw_circle) {
			//ofDrawCircle(100, 100, 50);
			//ofSetCircleResolution(55);
		}
		if (draw_rectangle) {
			/*ofDrawRectangle(50, 50, 100, 200);*/
		}
		ofEndShape();
		ofPopMatrix();

	}

	if(draw_triangle)
	{ 
		for (int i = 0; i < forme.v_formes.size(); i++)
		{
			Forme* formeCourante = forme.v_formes[i];
			if(formeCourante->getType() == Forme::TRIANGLE)
			{
				ofDrawTriangle(formeCourante->getX1(), formeCourante->getY1(),
					formeCourante->getX2(), formeCourante->getY2(),
					formeCourante->getX3(), formeCourante->getY3());
			}
		}
	}

	if(draw_circle)
	{ 
		for (int i = 0; i < forme.v_formes.size(); i++)
		{
			Forme* formeCourante = forme.v_formes[i];
			if (formeCourante->getType() == Forme::CERCLE)
			{
				ofDrawCircle(formeCourante->getXC(), formeCourante->getYC(), formeCourante->getRayon());
			}
		}
	}

	if(draw_rectangle)
	{ 
		for (int i = 0; i < forme.v_formes.size(); i++)
		{
			Forme* formeCourante = forme.v_formes[i];
			if (formeCourante->getType() == Forme::RECTANGLE)
			{
				ofDrawRectangle(formeCourante->getXR(), formeCourante->getYR(),
					formeCourante->getWidth(), formeCourante->getHeight());
			}
		}
	}

	//if (draw_triangle && draw_circle)
	if (draw_bezier)
	{
		for (int i = 0; i < forme.v_formes.size(); i++)
		{
			Forme* formeCourante = forme.v_formes[i];
			if (formeCourante->getType() == Forme::BEZIER)
			{
				ofDrawBezier(formeCourante->getX1(), formeCourante->getX2(), formeCourante->getXB1(), formeCourante->getYB1(),
					formeCourante->getXB2(), formeCourante->getYB2(), formeCourante->getX2(), formeCourante->getY2());
			}
		}
	}

	if (draw_ellipse)
	{
		for (int i = 0; i < forme.v_formes.size(); i++)
		{
			Forme* formeCourante = forme.v_formes[i];
			if (formeCourante->getType() == Forme::ELLIPSE)
			{
				ofDrawEllipse(formeCourante->getXR(), formeCourante->getYR(),
					formeCourante->getHeight(), formeCourante->getWidth());
			}
		}
	}

	renderer.draw();
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
	
	//if (draw_triangle && drawCircle == false && drawRectangle == false)
		//if (draw_triangle && !draw_circle && !draw_rectangle)
	if(draw_triangle)
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

		// Ajout de l'objet au vecteur
		Forme* newTriangle = new Forme(Forme::TRIANGLE, forme.getX1(), forme.getY1(),
			forme.getX2(), forme.getY2(), forme.getX3(), forme.getY3());
		forme.v_formes.push_back(newTriangle);

		// Affichage au terminal des objets triangles et leurs sommets
		// Decommenter pour tester
		/*for (int i = 0; i < forme.v_formes.size(); i++)
		{
			Forme* formeCourante = forme.v_formes[i];
			cout << "Adresse mémoire de l'objet " << i << " : " << formeCourante << endl;
			cout << "Coordonnées du premier sommet : (" << formeCourante->getX1() << ", " << formeCourante->getY1() << ")" << endl;
			cout << "Coordonnées du deuxième sommet : (" << formeCourante->getX2() << ", " << formeCourante->getY2() << ")" << endl;
			cout << "Coordonnées du troisième sommet : (" << formeCourante->getX3() << ", " << formeCourante->getY3() << ")" << endl;
		}
		cout << endl;*/
	}
	//if(draw_circle && drawTriangle == false && drawRectangle == false)
		//if (draw_circle && !draw_triangle && !draw_rectangle)
	if(draw_circle)
	{ 
		forme.setXC(renderer.mouse_press_x); 
		forme.setYC(renderer.mouse_press_y); 
		Forme* newCercle = new Forme(Forme::CERCLE, forme.getXC(), forme.getYC(), forme.getRayon());
		forme.v_formes.push_back(newCercle);
		//ofSetCircleResolution(55);
	}
	//if(draw_rectangle && drawCircle == false && drawTriangle == false)
		//if (draw_rectangle && !draw_circle && !draw_triangle)
	if(draw_rectangle)
	{ 
		forme.setXR(renderer.mouse_current_x);
		forme.setYR(renderer.mouse_current_y); 
		Forme* newRectangle = new Forme(Forme::RECTANGLE, forme.getXR(), forme.getYR(), forme.getWidth(), forme.getHeight());
		forme.v_formes.push_back(newRectangle);
	}
	// BEZIER
	//if (!draw_triangle && !draw_circle && !draw_rectangle)
	if(draw_bezier)
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

		Forme* newBezier = new Forme(Forme::BEZIER, forme.getX1(), forme.getY1(), forme.getXB1(), forme.getYB1(),
			forme.getXB2(), forme.getYB2(), forme.getX2(), forme.getY2());
		forme.v_formes.push_back(newBezier); 
	}
	if (draw_ellipse)
	{
		forme.setXR(renderer.mouse_press_x);
		forme.setYR(renderer.mouse_press_y);
		Forme* newEllipse = new Forme(Forme::ELLIPSE, forme.getXR(), forme.getYR(), forme.getHeight(), forme.getWidth());
	    forme.v_formes.push_back(newEllipse);
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

void Application::button_triangle(bool& value) { 
	if (value) {
		//draw_triangle = !draw_triangle;
		draw_triangle = true;
		draw_circle = draw_rectangle = draw_line = draw_ellipse = draw_bezier = drawCircle = false;
		drawRectangle = drawLine = drawEllipse = drawBezier = false;
	}
}

void Application::button_circle(bool& value) {
	if (value) {
		//draw_circle = !draw_circle;
		draw_circle = true;
		draw_triangle = draw_rectangle = draw_line = draw_ellipse = draw_bezier = drawCircle = false;
		drawTriangle = drawRectangle = drawLine = drawEllipse = drawBezier = false;
	}
}

void Application::button_rectangle(bool& value) {
	if (value) {
		//draw_rectangle = !draw_rectangle;
		draw_rectangle = true;
		draw_circle = draw_triangle = draw_line = draw_ellipse = draw_bezier = drawCircle = false;
		drawTriangle = drawCircle = drawLine = drawEllipse = drawBezier = false;
	}
}

void Application::button_line(bool& value) {
	if (value) {
		draw_line = true;
		draw_triangle = false;
		draw_circle = draw_rectangle = draw_triangle = draw_ellipse = draw_bezier = drawCircle = false;
		drawTriangle = drawCircle = drawRectangle = drawEllipse = drawBezier = false;
	}
}

void Application::button_ellipse(bool& value) {
	if (value) {
		draw_ellipse = true;
		draw_circle = draw_rectangle = draw_line = draw_triangle = draw_bezier = drawCircle = false;
		drawTriangle = drawCircle = drawRectangle = drawLine = drawBezier = false;
	}
}

void Application::button_bezier(bool& value) {
	if (value) {
		draw_bezier = true;
		draw_circle = draw_rectangle = draw_line = draw_ellipse = draw_triangle = drawCircle = false;
		drawTriangle = drawCircle = drawRectangle = drawLine = drawEllipse = false;
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
	}
}