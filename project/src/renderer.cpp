#include "Renderer.h"
#include "Interface.h"
#include "Forme.h"
#include "Constants.h"
#include "Application.h"
#include <string>

using namespace std;

void Renderer::setup() {
	ofSetFrameRate(60);
	interface.setup();
	
	gui.setup("Exportation");
	nameField.set("Image name", "name");
	gui.add(nameField);
	exportButton.setName("Export");
	gui.add(exportButton);
	gui.setPosition(0, 0);

	okDessiner = false; 

	//triangleColors= { interface.color_picker_stroke, interface.colorPickerFill };

	//Pour la capture dìmages
	frameCounter = 0;
	captureInterval = 60; // changer l'intervalle pour le nombre d'image exportee
	isRecording = false;
}


void Renderer::setup(vector<unique_ptr<Forme>>& v_formes) 
{
	v_formes_ptr = &v_formes;
}


void Renderer::draw() {
	//interface.draw();
	ofSetBackgroundColor(interface.color_picker_background);

	if (visible) {
		gui.draw();

		if (exportButton) {
			image_export(nameField, "png");
			exportButton = false;
			visible = false;
		}
	}

	auto currImg = imgPosList.begin();
	for (list<ofImage>::iterator iter = imageList.begin(); iter != imageList.end(); ++iter) {
		vector temp = *currImg;
		iter->draw(temp[0], temp[1]);
		++currImg;

	}

	//////////////////////////////////////////////////////////////////
	if (okDessiner)
	{
		dessinerTriangle();
		dessinerCercle(); 
		dessinerRectangle();
		dessinerLigne(); 
		dessinerEllipse(); 
		dessinerBezier(); 
	}
	//////////////////////////////////////////////////////////////////

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

	// Afficher un message si l'enregistrement est activé
	if (isRecording)
		ofDrawBitmapString("Enregistrement enmouse cours...", 20, 20);
}



void Renderer::dessinerTriangle() 
{
	if (v_formes_ptr) 
	{ 
		for (const auto& formeCourante : *v_formes_ptr) 
		{ 
			if (formeCourante->getType() == Forme::TRIANGLE) 
			{
				if (triangleFill) { // Remplissage
					ofFill();
					ofSetColor(triangleColors[1]);
					ofDrawTriangle(formeCourante->getX1(), formeCourante->getY1(),
						formeCourante->getX2(), formeCourante->getY2(),
						formeCourante->getX3(), formeCourante->getY3());
				}
				ofNoFill(); // Outline
				ofSetLineWidth(triangleStroke);
				ofSetColor(triangleColors[0]);
				ofDrawTriangle(formeCourante->getX1(), formeCourante->getY1(),
					formeCourante->getX2(), formeCourante->getY2(),
					formeCourante->getX3(), formeCourante->getY3());
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
				if (cercleFill) { // Remplissage
					ofFill();
					ofSetColor(cercleColors[1]);
					ofDrawCircle(formeCourante->getXC(), formeCourante->getYC(), formeCourante->getRayon());
				}
				ofNoFill(); // Outline
				ofSetLineWidth(cercleStroke);
				ofSetColor(cercleColors[0]);
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
				if (rectangleFill) { // Remplissage
					ofFill();
					ofSetColor(rectangleColors[1]);
					ofDrawRectangle(formeCourante->getXR(), formeCourante->getYR(),
						formeCourante->getWidth(), formeCourante->getHeight());
				}
				ofNoFill(); // Outline
				ofSetLineWidth(rectangleStroke);
				ofSetColor(rectangleColors[0]);
				ofDrawRectangle(formeCourante->getXR(), formeCourante->getYR(),
						formeCourante->getWidth(), formeCourante->getHeight());
			}
		}
	}

}

void Renderer::dessinerLigne()
{
	ofSetColor(0);

	if(!vecteur_lignes.empty())
	{ 
		// Dessiner sans ptr 
		for (auto& polyline : vecteur_lignes) 
		{
			ofNoFill(); // Outline
			ofSetLineWidth(ligneStroke);
			ofSetColor(ligneColor);
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
				if (ellipseFill) { // Remplissage
					ofFill();
					ofSetColor(ellipseColors[1]);
					ofDrawEllipse(formeCourante->getXR(), formeCourante->getYR(),
						formeCourante->getWidth(), formeCourante->getHeight());
				}
				ofNoFill(); // Outline
				ofSetLineWidth(ellipseStroke);
				ofSetColor(ellipseColors[0]);
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
				if (bezierFill) { // Remplissage
					ofFill();
					ofSetColor(bezierColors[1]);
					ofDrawBezier(formeCourante->getX1(), formeCourante->getX2(), formeCourante->getXB1(), formeCourante->getYB1(),
						formeCourante->getXB2(), formeCourante->getYB2(), formeCourante->getX2(), formeCourante->getY2());
				}
				ofNoFill(); // Outline
				ofSetLineWidth(bezierStroke);
				ofSetColor(bezierColors[0]);
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



void Renderer::update()
{
	if (isRecording)
	{
		frameCounter++;
		//captureInterval++; 
		//compteur++;
		if (frameCounter == captureInterval)
		{
			ofSaveScreen(ofToString(frameCounter / captureInterval) + ".png");
			//ofSaveScreen(ofToString(frameCounter) + ".png");
			frameCounter = 0;
			//captureInterval -= compteur;
		}
	}
}

void Renderer::toggleColorWheelGUI() {
	interface.toggleColorWheel();
}

void Renderer::toggleExportGUI() {
	visible = !visible;
}

void Renderer::image_export(const string name, const string extension) const {
	ofImage image;

	// extraire des données temporelles formatées
	string time_stamp = ofGetTimestampString("-%y%m%d-%H%M%S-%i");

	// générer un nom de fichier unique et ordonné
	string file_name = name + time_stamp + "." + extension;

	// capturer le contenu du framebuffer actif
	image.grabScreen(0, 0, ofGetWindowWidth(), ofGetWindowHeight());

	// sauvegarder le fichier image
	image.save(file_name);

	ofLog() << "<export image: " << file_name << ">";
}

void Renderer::captureImage() {
	// Exporter l'image
	ofSaveScreen(ofToString(frameCounter) + ".png");
}