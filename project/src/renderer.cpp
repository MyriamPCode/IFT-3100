#include "Renderer.h"
#include "Interface.h"
#include "Forme.h"
#include "Constants.h"
#include "Application.h"

using namespace std;

void Renderer::setup() {
	ofSetFrameRate(60);
	interface.setup();

	is_mouse_button_pressed = false;
	is_mouse_button_dragged = false;
	import_activate = false;

	mouse_current_x = mouse_current_y = mouse_press_x = mouse_press_y = mouse_drag_x = mouse_drag_y = 0;

	okDessiner = false; 
}


void Renderer::setup(vector<unique_ptr<Forme>>& v_formes) 
{
	v_formes_ptr = &v_formes;
}
//void Renderer::setup(vector<unique_ptr<ofPolyline>>& vecteur_lignes)
//{
//	vecteur_lignes_ptr = &vecteur_lignes;
//}

void Renderer::draw() {
	interface.draw();

	auto currImg = imgPosList.begin();
	for (list<ofImage>::iterator iter = imageList.begin(); iter != imageList.end(); ++iter) {
		vector temp = *currImg;
		iter->draw(temp[0], temp[1]);
		++currImg;

	}

	//////////////////////////////////////////////////////////////////
	if (okDessiner)
	//if(v_formes_ptr > 0)
	{
		//cout << "DRAW TRIANGLE DANS IF RENDERER.CPP" << endl;
		dessinerTriangle();
		dessinerCercle(); 
		dessinerRectangle();
		dessinerLigne(); 
		dessinerEllipse(); 
		dessinerBezier(); 
	}
	//okDessiner = false;
	//////////////////////////////////////////////////////////////////

	draw_cursor(mouse_current_x, mouse_current_y);

	int gridSize = 50; // Espacement de la grille
	// Obtenir les coordonnées de la souris
	int mouseX = ofGetMouseX();
	int mouseY = ofGetMouseY();

	// Convertir les coordonnées de la souris dans l'espace de la grille
	int gridX = mouseX / gridSize;
	int gridY = mouseY / gridSize;

	// Dessiner les coordonnées de la souris sur la grille
	ofSetColor(255); // Couleur blanc
	ofDrawBitmapString("Mouse X: " + ofToString(mouseX) + ", Mouse Y: " + ofToString(mouseY), 1730, 65);
	ofDrawBitmapString("Grid X: " + ofToString(gridX) + ", Grid Y: " + ofToString(gridY), 1730, 85);
}

//void Renderer::dessinerTriangle()
//{
//	//cout << "DESSINERTRIANGLE RENDERER.CPP" << endl;
//	cout << "SIZE DU VECTEUR : " << v_formes_ptr->size() << endl;
//	//for (int i = 0; i < forme.v_formes.size(); i++)
//		for (int i = 0; i < v_formes_ptr->size(); i++)
//	{
//		cout << "DANS LE FOR RENDERER.CPP" << endl;
//		//Forme* formeCourante = forme.v_formes[i];
//		Forme* formeCourante = (*v_formes_ptr)[i];
//		if (formeCourante->getType() == Forme::TRIANGLE)
//		{
//			cout << "DANS LE IF RENDERER.CPP" << endl;
//			ofDrawTriangle(formeCourante->getX1(), formeCourante->getY1(),
//				formeCourante->getX2(), formeCourante->getY2(),
//				formeCourante->getX3(), formeCourante->getY3());
//
//			cout << "VOICI LES TRIANGLES: " << i << endl;
//		}
//	}
//}

//void Renderer::dessinerTriangle() {
//
//	cout << "SIZE DU VECTEUR : " << v_formes.size() << endl;
//
//	for (const auto& formeCourante : v_formes) {
//		if (formeCourante->getType() == Forme::TRIANGLE) {
//			cout << "DANS LE IF RENDERER.CPP" << endl;
//			ofDrawTriangle(formeCourante->getX1(), formeCourante->getY1(),
//				formeCourante->getX2(), formeCourante->getY2(),
//				formeCourante->getX3(), formeCourante->getY3());
//
//			cout << "VOICI LES TRIANGLES: " << formeCourante << endl;
//		}
//	}
//}
void Renderer::dessinerTriangle() 
{
	ofSetColor(0);
	if (v_formes_ptr) 
	{ // Vérifiez si le pointeur est valide
		//cout << "SIZE DU VECTEUR : " << v_formes_ptr->size() << endl;

		for (const auto& formeCourante : *v_formes_ptr) 
		{ // Déreferencez le pointeur pour accéder au vecteur
			if (formeCourante->getType() == Forme::TRIANGLE) 
			{
				//cout << "DANS LE IF RENDERER.CPP" << endl;
				//float x1 = formeCourante->getX1();
				//float y1 = formeCourante->getY1();
				//float x2 = formeCourante->getX2();
				//float y2 = formeCourante->getY2();
				//float x3 = formeCourante->getX3();
				//float y3 = formeCourante->getY3();
				//ofDrawTriangle(x1, y1, x2, y2, x3, y3);
				ofDrawTriangle(formeCourante->getX1(), formeCourante->getY1(),
					formeCourante->getX2(), formeCourante->getY2(),
					formeCourante->getX3(), formeCourante->getY3());

				//cout << "VOICI LES TRIANGLES: " << formeCourante << endl;
				// DEBUGGER
					//cout << "Adresse mémoire de l'objet " << formeCourante << endl;
					//cout << "Coordonnées du premier sommet : (" << formeCourante->getX1() << ", " << formeCourante->getY1() << ")" << endl;
					//cout << "Coordonnées du deuxième sommet : (" << formeCourante->getX2() << ", " << formeCourante->getY2() << ")" << endl;
					//cout << "Coordonnées du troisième sommet : (" << formeCourante->getX3() << ", " << formeCourante->getY3() << ")" << endl;
			}
		}
	}
	else {
		cout << "Le pointeur vers le vecteur de formes n'est pas valide." << endl;
	}
}

void Renderer::dessinerCercle()
{
	ofSetColor(0);
	if (v_formes_ptr)
	{
		for (const auto& formeCourante : *v_formes_ptr)
		{
			if (formeCourante->getType() == Forme::CERCLE)
			{
				ofDrawCircle(formeCourante->getXC(), formeCourante->getYC(), formeCourante->getRayon());
			}
		}
	}
}

void Renderer::dessinerRectangle()
{
	ofSetColor(0);
	if (v_formes_ptr)
	{
		for (const auto& formeCourante : *v_formes_ptr)
		{
			if (formeCourante->getType() == Forme::RECTANGLE)
			{
				ofDrawRectangle(formeCourante->getXR(), formeCourante->getYR(),
						formeCourante->getWidth(), formeCourante->getHeight());
			}
		}
	}

}

void Renderer::dessinerLigne()
{
	ofSetColor(0);
	//if (vecteur_lignes_ptr) { // Vérifiez si le pointeur vers le vecteur est valide
	//	for (const auto& polyline_ptr : *vecteur_lignes_ptr) { // Déreferencez le pointeur pour accéder au vecteur
	//		if (polyline_ptr) { // Vérifiez si le pointeur est valide
	//			ofPolyline* polyline_raw_ptr = polyline_ptr.get(); // Obtenir le pointeur brut
	//			if (polyline_raw_ptr) { // Vérifiez si le pointeur brut est valide
	//				polyline_raw_ptr->draw(); // Appeler la méthode draw()
	//			}
	//		}
	//	}
	//}
	//else {
	//	cout << "Le pointeur vers le vecteur de polylignes n'est pas valide." << endl;
	//}
	if(!vecteur_lignes.empty())
	{ 
		// Dessiner sans ptr 
		for (auto& polyline : vecteur_lignes) 
		{
			polyline.draw();
		
		}
	}
}

void Renderer::dessinerEllipse()
{
	ofSetColor(0);
	if (v_formes_ptr)
	{
		for (const auto& formeCourante : *v_formes_ptr)
		{
			if (formeCourante->getType() == Forme::ELLIPSE)
			{
				ofDrawEllipse(formeCourante->getXR(), formeCourante->getYR(),
				 formeCourante->getWidth(), formeCourante->getHeight());
			}
		}
	}

}

void Renderer::dessinerBezier()
{
	ofSetColor(0);
	if (v_formes_ptr)
	{
		for (const auto& formeCourante : *v_formes_ptr)
		{
			if (formeCourante->getType() == Forme::BEZIER)
			{
				ofDrawBezier(formeCourante->getX1(), formeCourante->getX2(), formeCourante->getXB1(), formeCourante->getYB1(),
				formeCourante->getXB2(), formeCourante->getYB2(), formeCourante->getX2(), formeCourante->getY2());
			}
		}
	}

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
