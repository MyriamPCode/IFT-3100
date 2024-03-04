#include "Interface.h"
#include "Constants.h"

using namespace std;

void Interface::setup() {
	buttonFont.load("roboto/Roboto-Regular.ttf", 24);
}

void Interface::draw() {
	backgroundLine();
	backgroundInteraction();
	importButton();
	//panelScene();
}

//Interface::Interface(Renderer renderer) : renderer(renderer) {}


void Interface::importButton() {
	string buttonText = "Import";
	ofRectangle textbox = buttonFont.getStringBoundingBox(buttonText, 0, 0);
	if (IMPORT_BUTTON_WIDTH != textbox.getWidth() + 9) {
		IMPORT_BUTTON_WIDTH = textbox.getWidth() + 9;
	}

	ofSetColor(backgroundInteractionBorderColor);
	ofDrawRectangle(0, 0, IMPORT_BUTTON_WIDTH, 40);
	ofSetColor(textColor);

	buttonFont.drawString(buttonText, 3, 30);
}

void Interface::backgroundLine() {
	ofSetLineWidth(1);
	ofSetColor(backgroundLineColor);
	for (int i = 0; i < WIDTH; i += WIDTH / 40) {
		ofDrawLine(i, 0, i, HEIGHT);
		ofDrawLine(0, i, WIDTH, i);
	};

	for (int i = 0; i < HEIGHT; i += WIDTH / 40) {
		ofDrawLine(0, i, WIDTH, i);
	};
}

void Interface::backgroundInteraction() {
	// Right rectangle
	ofSetColor(backgroundInteractionColor);
	ofFill();
	ofDrawRectangle(WIDTH - 40, 0, 40, HEIGHT);

	ofSetColor(backgroundInteractionBorderColor);
	ofNoFill();
	ofDrawRectangle(WIDTH - 40, 0, 40, HEIGHT);

	//Top rectangle
	ofSetColor(backgroundInteractionColor);
	ofFill();
	ofDrawRectangle(0, 0, WIDTH, 40);

	ofSetColor(backgroundInteractionBorderColor);
	ofNoFill();
	ofDrawRectangle(0, 0, WIDTH, 40);
}

void Interface::panelScene()
{
	ofSetColor(backgroundPanelSceneColor);
	ofFill();
	ofDrawRectangle(0, 41, 255, HEIGHT - 40);

	ofSetColor(backgroundInteractionBorderColor);
	ofNoFill();

	// Dessiner les lignes de segmentation dans le panneau
	int numSegments = 8; // Nombre de segments � dessiner
	float segmentHeight = (HEIGHT - 40) / numSegments; // Hauteur de chaque segment

	for (int i = 1; i < numSegments; ++i) {
		// Dessiner une ligne horizontale pour segmenter la zone
		ofDrawLine(0, 41 + i * segmentHeight, 255, 41 + i * segmentHeight);
	}

	//// Dessiner les formes dans le panneau
	//int lineHeight = 20; // Hauteur de ligne pour chaque forme
	//int startY = 41 + lineHeight; // Position verticale de d�part pour afficher les formes

	//vector<unique_ptr<Forme>>* ptrFormes = &renderer.getVecteurFormesPtr();
	//for (const auto& forme : *ptrFormes)
	//{
	//	int xPos = 41 + 10; // Position horizontale de d�part pour afficher les informations de la forme
	//	int yPos = startY + i * lineHeight; // Position verticale pour afficher les informations de la forme

	//	// Dessiner les informations de la forme
	//	ofSetColor(textColor);
	//	ofDrawBitmapString("Forme " + ofToString(i) + " : ", xPos, yPos);
	//	// Dessiner d'autres informations de forme selon vos besoins
	//}
	// Parcourir le vecteur de formes et afficher les informations de chaque forme
	//for (size_t i = 0; i < renderer.getVecteurFormesPtr()->size(); ++i) {
	//	int xPos = 41 + 10; // Position horizontale de d�part pour afficher les informations de la forme
	//	int yPos = startY + i * lineHeight; // Position verticale pour afficher les informations de la forme

	//	// Dessiner les informations de la forme
	//	ofSetColor(textColor);
	//	ofDrawBitmapString("Forme " + ofToString(i) + " : ", xPos, yPos);
	//	// Dessiner d'autres informations de forme selon vos besoins
	//}
}