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

	is_mouse_button_pressed = false;
	is_mouse_button_dragged = false;
	import_activate = false;

	mouse_current_x = mouse_current_y = mouse_press_x = mouse_press_y = mouse_drag_x = mouse_drag_y = 0;

	okDessiner = false; 

	//triangleColors= { interface.color_picker_stroke, interface.colorPickerFill };

	//Pour la capture d�mages
	frameCounter = 0;
	captureInterval = 60; // changer l'intervalle pour le nombre d'image exportee
	isRecording = false;

	modeDrawState = modeTransformState = false;
}


void Renderer::setup(vector<unique_ptr<Forme>>& v_formes) 
{
	v_formes_ptr = &v_formes;
}


void Renderer::draw() {
	interface.draw();
	ofSetBackgroundColor(interface.color_picker_background);

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
		//if (modeTransformState)
		//{
		//	transformation();
		//}
	}
	//////////////////////////////////////////////////////////////////

	draw_cursor(mouse_current_x, mouse_current_y);

	int gridSize = 50; // Espacement de la grille
	// Obtenir les coordonn�es de la souris
	int mouseX = ofGetMouseX();
	int mouseY = ofGetMouseY();

	// Convertir les coordonn�es de la souris dans l'espace de la grille
	int gridX = mouseX / gridSize;
	int gridY = mouseY / gridSize;

	// Dessiner les coordonn�es de la souris sur la grille
	ofSetColor(255); // Couleur blanc
	ofDrawBitmapString("Mouse X: " + ofToString(mouseX) + ", Mouse Y: " + ofToString(mouseY), 1630, 65);
	ofDrawBitmapString("Grid X: " + ofToString(gridX) + ", Grid Y: " + ofToString(gridY), 1630, 85);

	// Afficher un message si l'enregistrement est activ�
	if (isRecording)
		ofDrawBitmapString("Enregistrement enmouse cours...", 20, 20);
}

// Partie Myriam 
//** 
//ofPushMatrix();
//ofTranslate(uiPosition->x, uiPosition->y);
//for (int i = 0; i < uiAmount; i++) {
//    ofPushMatrix();
//    ofTranslate(i * uiStep->x, i * uiStep->y);
//    ofRotateXDeg(i * uiRotate->x);
//    ofRotateYDeg(i * uiRotate->y);
//    ofRotateZDeg(i * uiRotate->z);
//    ofTranslate(i * uiShift->x, i * uiShift->y);
//    ofScale(uiSize->x, uiSize->y);
//    ofBeginShape();
//    if (draw_triangle) {
//        //ofDrawTriangle(0, 0, -16, 32, 16, 32);
//    } 
//    if (draw_circle) {
//        //ofDrawCircle(100, 100, 50);
//        //ofSetCircleResolution(55);
//    }
//    if (draw_rectangle) {
//        /ofDrawRectangle(50, 50, 100, 200);/
//    }
//    ofEndShape();
//    ofPopMatrix();

//}


void Renderer::dessinerTriangle() {
	if (v_formes_ptr) {
		for (int i = 0; i < v_formes_ptr->size(); ++i) {
			const auto& formeCourante = (*v_formes_ptr)[i];
			if (formeCourante->getType() == Forme::TRIANGLE) {
				//if (triangleFill) {
					//ofFill();
					ofSetLineWidth(triangleStroke);
					ofSetColor(triangleColors[1]);
					// Dessin du triangle avec translation si l'index correspond et le mode de transformation est activ�
					if (i == inputIndex && modeTransformState) {
						ofPushMatrix();
						ofScale(uiSize->x, uiSize->y);
						ofTranslate(uiPosition->x, uiPosition->y); // translation 
						ofRotateXDeg(uiRotate->x); // pivoter sur x 
						ofRotateYDeg(uiRotate->y); // pivoter sur y 
						ofRotateZDeg(uiRotate->z); // pivoter sur z 
						//ofTranslate(uiStep->x, uiStep->y);
						//ofTranslate(uiShift->x, uiShift->y);

						ofNoFill();
						ofSetColor(255, 0, 0); // Rouge
						ofBeginShape();
						ofDrawTriangle(formeCourante->getX1(), formeCourante->getY1(),
							formeCourante->getX2(), formeCourante->getY2(),
							formeCourante->getX3(), formeCourante->getY3());
						ofEndShape();
						ofPopMatrix();
					}
					else {
						// Dessin du triangle sans translation
						ofSetColor(triangleColors[0]); // Couleur de contour normale
						ofDrawTriangle(formeCourante->getX1(), formeCourante->getY1(),
							formeCourante->getX2(), formeCourante->getY2(),
							formeCourante->getX3(), formeCourante->getY3());
					}
			}
		}
	}
}


// Ne fonctionne pas pour le moment 
void Renderer::updateCoordinates(int x, int y) {
	if (v_formes_ptr && inputIndex >= 0 && inputIndex < v_formes_ptr->size()) {
		auto& formeCourante = (*v_formes_ptr)[inputIndex];
		int diffX, diffY;
		if (formeCourante->getType() == Forme::TRIANGLE) {
			// Nouvelles coordonn�es pour le triangle
			int newX1 = formeCourante->getX1() + x;
			int newY1 = formeCourante->getY1() + y;
			int newX2 = formeCourante->getX2() + x;
			int newY2 = formeCourante->getY2() + y;
			int newX3 = formeCourante->getX3() + x;
			int newY3 = formeCourante->getY3() + y;

			// Mettre � jour les coordonn�es de l'objet
			formeCourante->setX1(newX1);
			formeCourante->setY1(newY1);
			formeCourante->setX2(newX2);
			formeCourante->setY2(newY2);
			formeCourante->setX3(newX3);
			formeCourante->setY3(newY3);
		}
	}
}

void Renderer::transformation(){
	if (v_formes_ptr) {
		for (int i = 0; i < v_formes_ptr->size(); ++i) {
			const auto& formeCourante = (*v_formes_ptr)[i];
			if (formeCourante->getType() == Forme::CERCLE) {
				//if (cercleFill) {
					ofFill();
					ofSetColor(cercleColors[255,0,0]);
					
					// Dessiner le cercle avec translation si l'index correspond et le mode de transformation est activ�
					if (i == inputIndex) {
						ofPushMatrix();
						//ofTranslate(uiPosition->x, uiPosition->y);
						ofRotateXDeg(uiRotate->x);
						ofRotateYDeg(uiRotate->y);
						ofRotateZDeg(uiRotate->z);
						ofNoFill();
						ofSetLineWidth(cercleStroke);
						ofSetColor(255, 0, 0); // Rouge
						ofDrawCircle(formeCourante->getXC(), formeCourante->getYC(), formeCourante->getRayon());

						ofPopMatrix();
					}

				}
				 //Dessin du contour du cercle
				/*ofNoFill();
				ofSetLineWidth(cercleStroke);*/
				// Si l'index correspond et le mode de transformation est activ�, dessinez le contour en rouge
				//if (i == inputIndex && modeTransformState) {
					//ofSetColor(255, 0, 0); // Rouge
					//ofDrawCircle(formeCourante->getXC(), formeCourante->getYC(), formeCourante->getRayon());
					//ofDrawCircle(p1.x, p1.y, p1.z);
				//}
			//}
				
		}
	}
}

void Renderer::dessinerCercle() {
	if (v_formes_ptr) {
		for (int i = 0; i < v_formes_ptr->size(); ++i) {
			const auto& formeCourante = (*v_formes_ptr)[i];
			if (formeCourante->getType() == Forme::CERCLE) {
				//if (cercleFill) {
					//ofFill();
					ofSetLineWidth(cercleStroke);
					ofSetColor(cercleColors[1]);
					// Dessiner le cercle avec transformation si l'index correspond et le mode de transformation est activ�
					if (i == inputIndex && modeTransformState) {
						ofPushMatrix();
						ofScale(uiSize->x, uiSize->y);
						ofTranslate(uiPosition->x, uiPosition->y); // translation 
						ofRotateXDeg(uiRotate->x); // pivoter sur x 
						ofRotateYDeg(uiRotate->y); // pivoter sur y 
						ofRotateZDeg(uiRotate->z); // pivoter sur z 
						//ofTranslate(uiStep->x, uiStep->y);
						//ofTranslate(uiShift->x, uiShift->y);
						ofNoFill();
						ofBeginShape();
						ofSetColor(255, 0, 0); // Rouge
						ofDrawCircle(formeCourante->getXC(), formeCourante->getYC(), formeCourante->getRayon());
						ofEndShape();
						ofPopMatrix();
					}
					else {
						// Dessiner le rectangle sans transformation
						ofSetColor(cercleColors[0]); // Couleur de contour normale
						ofDrawCircle(formeCourante->getXC(), formeCourante->getYC(), formeCourante->getRayon());
					}
				//}
			}

		}
	}
}


void Renderer::dessinerRectangle() {
	if (v_formes_ptr) {
		for (int i = 0; i < v_formes_ptr->size(); ++i) {
			const auto& formeCourante = (*v_formes_ptr)[i];
			if (formeCourante->getType() == Forme::RECTANGLE) {
				//if (rectangleFill) {
					//ofFill();
					ofSetLineWidth(rectangleStroke);
					ofSetColor(rectangleColors[1]);
					// Dessiner le rectangle avec transformation si l'index correspond et le mode de transformation est activ�
					if (i == inputIndex && modeTransformState) {
						ofPushMatrix();
						ofScale(uiSize->x, uiSize->y);
						ofTranslate(uiPosition->x, uiPosition->y); // translation 
						ofRotateXDeg(uiRotate->x); // pivoter sur x 
						ofRotateYDeg(uiRotate->y); // pivoter sur y 
						ofRotateZDeg(uiRotate->z); // pivoter sur z 
						//ofTranslate(uiStep->x, uiStep->y);
						//ofTranslate(uiShift->x, uiShift->y);
						ofNoFill();
						ofBeginShape(); 
						ofSetColor(255, 0, 0); // Rouge
						ofDrawRectangle(formeCourante->getXR(), formeCourante->getYR(),
							formeCourante->getWidth(), formeCourante->getHeight());
						ofEndShape(); 
						ofPopMatrix();
					}
					else {
						// Dessiner le rectangle sans transformation
						ofSetColor(rectangleColors[0]); // Couleur de contour normale
						ofDrawRectangle(formeCourante->getXR(), formeCourante->getYR(),
							formeCourante->getWidth(), formeCourante->getHeight());
					}
				//}
				
			}
		}
	}
}

void Renderer::dessinerLigne() {
	if (!vecteur_lignes.empty()) {
		for (auto& polyline : vecteur_lignes) {
			ofPushMatrix(); // Sauvegarde la matrice de transformation actuelle
			ofNoFill(); // Contour uniquement
			ofSetLineWidth(ligneStroke);
			ofSetColor(ligneColor);
			polyline.draw(); // Dessine la ligne
			ofPopMatrix(); // Restaure la matrice de transformation pr�c�dente
		}
	}
}
//void Renderer::dessinerLigne() {
//	int x = uiPosition->x;
//	int y = uiPosition->y;
//
//	if (!v_formes.empty()) {
//		for (auto& forme : v_formes) {
//			if (forme->getType() == Forme::LIGNE) { // V�rifiez si la forme est une ligne
//				ofPushMatrix(); // Sauvegarde la matrice de transformation actuelle
//				ofTranslate(x, y); // Applique la translation
//				ofNoFill(); // Contour uniquement
//				ofSetLineWidth(ligneStroke); // D�finir l'�paisseur du trait
//				ofSetColor(ligneColor); // D�finir la couleur
//				forme.draw(); // Dessine la ligne
//				ofPopMatrix(); // Restaure la matrice de transformation pr�c�dente
//			}
//		}
//	}
//}


void Renderer::dessinerEllipse() {
	if (v_formes_ptr) {
		for (int i = 0; i < v_formes_ptr->size(); ++i) {
			const auto& formeCourante = (*v_formes_ptr)[i];
			if (formeCourante->getType() == Forme::ELLIPSE) {
				//if (ellipseFill) {
					//ofFill();
					ofSetLineWidth(ellipseStroke);
					ofSetColor(ellipseColors[1]);
					// Dessiner l'ellipse avec transformation si l'index correspond et le mode de transformation est activ�
					if (i == inputIndex && modeTransformState) {
						ofPushMatrix();
						ofScale(uiSize->x, uiSize->y);
						ofTranslate(uiPosition->x, uiPosition->y); // translation 
						ofRotateXDeg(uiRotate->x); // pivoter sur x 
						ofRotateYDeg(uiRotate->y); // pivoter sur y 
						ofRotateZDeg(uiRotate->z); // pivoter sur z 
						//ofTranslate(uiStep->x, uiStep->y);
						//ofTranslate(uiShift->x, uiShift->y);
						ofNoFill();
						ofBeginShape();
						ofSetColor(255, 0, 0); // Rouge
						ofDrawEllipse(formeCourante->getXR(), formeCourante->getYR(),
							formeCourante->getWidth(), formeCourante->getHeight());
						ofEndShape(); 
						ofPopMatrix();
					}
					else {
						// Dessiner le ellipse sans transformation
						ofSetColor(ellipseColors[0]); // Couleur de contour normale
						ofDrawEllipse(formeCourante->getXR(), formeCourante->getYR(),
							formeCourante->getWidth(), formeCourante->getHeight());
					}
				//}
			}
		}
	}
}


void Renderer::dessinerBezier() {
	if (v_formes_ptr) {
		for (int i = 0; i < v_formes_ptr->size(); ++i) {
			const auto& formeCourante = (*v_formes_ptr)[i];
			if (formeCourante->getType() == Forme::BEZIER) {
				//if (bezierFill) {
					//ofFill();
					ofSetLineWidth(bezierStroke);
					ofSetColor(bezierColors[1]);
					// Dessiner la courbe de B�zier avec transformation si l'index correspond et le mode de transformation est activ�
					if (i == inputIndex && modeTransformState) {

						ofPushMatrix();
						ofScale(uiSize->x, uiSize->y);
						ofTranslate(uiPosition->x, uiPosition->y); // translation 
						ofRotateXDeg(uiRotate->x); // pivoter sur x 
						ofRotateYDeg(uiRotate->y); // pivoter sur y 
						ofRotateZDeg(uiRotate->z); // pivoter sur z 
						ofTranslate(uiStep->x, uiStep->y);
						ofTranslate(uiShift->x, uiShift->y);
						ofNoFill();
						ofBeginShape();
						ofSetColor(255, 0, 0); // Rouge
						ofDrawBezier(formeCourante->getX1(), formeCourante->getY1(),
							formeCourante->getXB1(), formeCourante->getYB1(),
							formeCourante->getXB2(), formeCourante->getYB2(),
							formeCourante->getX2(), formeCourante->getY2());
						ofEndShape();
						ofPopMatrix();
					}
					else {
						// Dessiner la courbe de B�zier sans transformation
						ofSetColor(bezierColors[0]); // Couleur de contour normale
						ofDrawBezier(formeCourante->getX1(), formeCourante->getY1(),
							formeCourante->getXB1(), formeCourante->getYB1(),
							formeCourante->getXB2(), formeCourante->getYB2(),
							formeCourante->getX2(), formeCourante->getY2());
					}
				//}
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
void Renderer::image_export(const string name, const string extension)const
{
	ofImage image;

	// extraire des donn�es temporelles format�es
	string time_stamp = ofGetTimestampString("-%y%m%d-%H%M%S-%i");

	// g�n�rer un nom de fichier unique et ordonn�
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