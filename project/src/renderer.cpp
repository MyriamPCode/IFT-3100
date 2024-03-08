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

	teapotMultiple.loadModel("models/teapot.obj");
	teapotMultiple.setPosition(0, 0, 0);

	teapotOrtho.loadModel("models/teapot.obj");
	teapotOrtho.setPosition(800, 700, 0);

	okDessiner = false; 

	//triangleColors= { interface.color_picker_stroke, interface.colorPickerFill };

	//Pour la capture dìmages
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

	
	model1.setPosition(1410,700, 0);
	model2.setPosition(-50, 1200, -400);
	//model3.setPosition(800, 1000, -600);

	//////////////////////////////////////////////////////////////////
	if (okDessiner)
	{
		dessinerTriangle();
		dessinerCercle(); 
		dessinerRectangle();
		dessinerLigne(); 
		dessinerEllipse(); 
		dessinerBezier(); 
		dessinerSphere(); 
		dessinerCube();
	}
	//////////////////////////////////////////////////////////////////
	if (interface.getShowModel()) {
		if (interface.getRenderType() == MeshRenderMode::wireframe) {
			teapotMultiple.draw(OF_MESH_WIREFRAME);
			teapotOrtho.draw(OF_MESH_WIREFRAME);
		}
		else if (interface.getRenderType() == MeshRenderMode::fill) {

			teapotMultiple.draw(OF_MESH_FILL);
			teapotOrtho.draw(OF_MESH_FILL);
		}
		else if (interface.getRenderType() == MeshRenderMode::vertex) {
			teapotMultiple.draw(OF_MESH_POINTS);
			teapotOrtho.draw(OF_MESH_POINTS);
		}
	}

	// Afficher un message si l'enregistrement est activé
	if (isRecording)
		ofDrawBitmapString("Enregistrement enmouse cours...", 20, 20);
}


void Renderer::dessinerSphere(){
	ofSetLineWidth(1);
	ofSetColor(255,0,0);
	if (v_formes_ptr) {
		for (const auto& formeCourante : *v_formes_ptr)
		{
			if(formeCourante->getType() == Forme::SPHERE){
				ofVec3f viktor = formeCourante->getVSphere();
				ofDrawSphere(viktor.x, viktor.y, 0, 150);
			}
		}
	}
}

void Renderer::dessinerCube() {
	ofSetLineWidth(1);
	ofSetColor(255, 255, 0);
	if (v_formes_ptr) {
		for (const auto& formeCourante : *v_formes_ptr)
		{
			if (formeCourante->getType() == Forme::CUBE) {
				ofVec3f viktor = formeCourante->getVSphere();
				ofDrawBox(viktor.x, viktor.y, 0, 150);
			}
		}
	}
}

void Renderer::dessinerTriangle() {
	if (v_formes_ptr) {
		for (int i = 0; i < v_formes_ptr->size(); ++i) {
			const auto& formeCourante = (*v_formes_ptr)[i];
			if (formeCourante->getType() == Forme::TRIANGLE) {
				//if (triangleFill) {
					//ofFill();
					ofSetLineWidth(triangleStroke);
					ofSetColor(triangleColors[1]);
					// Dessin du triangle avec translation si l'index correspond et le mode de transformation est activé
					if (i == inputIndex && modeTransformState) {
						ofPushMatrix();
						ofTranslate(uiPosition->x, uiPosition->y); // positionnement 
						ofTranslate(uiAmount * uiStep->x, uiAmount * uiStep->y);
	
						ofRotateXDeg(uiRotate->x); // pivoter sur x 
						ofRotateYDeg(uiRotate->y); // pivoter sur y 
						ofRotateZDeg(uiRotate->z); // pivoter sur z 
						ofScale(uiSize->x, uiSize->y);

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


void Renderer::dessinerCercle() {
	if (v_formes_ptr) {
		for (int i = 0; i < v_formes_ptr->size(); ++i) {
			const auto& formeCourante = (*v_formes_ptr)[i];
			if (formeCourante->getType() == Forme::CERCLE) {
				//if (cercleFill) {
					//ofFill();
					ofSetLineWidth(cercleStroke);
					ofSetColor(cercleColors[1]);
					// Dessiner le cercle avec transformation si l'index correspond et le mode de transformation est activé
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
					ofPushMatrix();
					ofTranslate(0, 0, 0); 
					// Dessiner le rectangle avec transformation si l'index correspond et le mode de transformation est activé
					if (i == inputIndex && modeTransformState) {
						ofPushMatrix();
						ofRotateXDeg(uiRotate->x); // pivoter sur x 
						ofRotateYDeg(uiRotate->y); // pivoter sur y 
						ofRotateZDeg(uiRotate->z); // pivoter sur z 
						ofTranslate(uiPosition->x, uiPosition->y); // translation 
						ofTranslate(uiStep->x, uiStep->y);
						ofTranslate(uiShift->x, uiShift->y);
						ofScale(uiSize->x, uiSize->y,1);
						
						ofNoFill();
						ofBeginShape(); 
						ofSetColor(255, 0, 0); // Rouge
						ofDrawRectangle(formeCourante->getXR(), formeCourante->getYR(),
							formeCourante->getWidth(), formeCourante->getHeight());
						ofTranslate(formeCourante->getXR(), formeCourante->getYR());
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
			ofPopMatrix(); // Restaure la matrice de transformation précédente
		}
	}
}


void Renderer::dessinerEllipse() {
	if (v_formes_ptr) {
		for (int i = 0; i < v_formes_ptr->size(); ++i) {
			const auto& formeCourante = (*v_formes_ptr)[i];
			if (formeCourante->getType() == Forme::ELLIPSE) {
				//if (ellipseFill) {
					//ofFill();
					ofSetLineWidth(ellipseStroke);
					ofSetColor(ellipseColors[1]);
					// Dessiner l'ellipse avec transformation si l'index correspond et le mode de transformation est activé
					if (i == inputIndex && modeTransformState) {
						ofPushMatrix();
						ofScale(uiSize->x, uiSize->y);
						ofTranslate(uiPosition->x, uiPosition->y); // translation 
						ofRotateXDeg(uiRotate->x); // pivoter sur x 
						ofRotateYDeg(uiRotate->y); // pivoter sur y 
						ofRotateZDeg(uiRotate->z); // pivoter sur z 
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
					// Dessiner la courbe de Bézier avec transformation si l'index correspond et le mode de transformation est activé
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
						// Dessiner la courbe de Bézier sans transformation
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
	image.grabScreen(0, INTERACTION_BAR_HEIGHT, ofGetWindowWidth() - INTERACTION_BAR_HEIGHT, ofGetWindowHeight() - INTERACTION_BAR_HEIGHT);

	// sauvegarder le fichier image
	image.save(file_name);

	ofLog() << "<export image: " << file_name << ">";
}

void Renderer::captureImage() {
	// Exporter l'image
	ofSaveScreen(ofToString(frameCounter) + ".png");
}