#pragma once

#include "ofMain.h"
//#include "Renderer.h"
#include <memory>
// Déclaration anticipée de la classe Renderer

class Interface {
	public:
		void setup();
		void draw();
		// Constructeur prenant un objet Renderer en paramètre
		//Interface(Renderer& r) : renderer(r) {}
		//Renderer renderer;
	private:
		ofTrueTypeFont buttonFont;

		ofColor backgroundLineColor = ofColor(217, 217, 217);
		ofColor backgroundInteractionColor = ofColor(4, 3, 77);
		ofColor backgroundInteractionBorderColor = ofColor(215, 215, 224);

		ofColor textColor = backgroundLineColor;

		void backgroundLine();
		void backgroundInteraction();
		void importButton();

		void panelScene(); 
		ofColor backgroundPanelSceneColor = ofColor(125);
		//Renderer& renderer;
};

//class Renderer;

