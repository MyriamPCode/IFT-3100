#pragma once

#include "ofMain.h"
#include <vector>
#include "ofxGui.h"
#include "ofxButton.h"
//#include "Renderer.h"
#include <memory>
// D�claration anticip�e de la classe Renderer

using namespace std;

enum class MeshRenderMode { fill, wireframe, vertex };

class Interface {
	public:
		vector<ofImage> iconTopBar;
		vector<ofImage> iconSideBar;
		
		ofParameter<ofColor> color_picker_background;
		ofParameter<ofColor> color_picker_stroke;
		ofxToggle fillButton;
		ofParameter<ofColor> colorPickerFill;
		ofParameter<float> slider_stroke_weight;
		bool fillEnabled = false;

		ofxToggle modelToggle;
		ofxToggle fillRender;
		ofxToggle wireframeRender;
		ofxToggle pointRender;

		void setup();
		void draw();
		void toggleColorWheel();
		void toggleModelOptions();
		void setRenderType(MeshRenderMode renderMode) { typeRender = renderMode; }
		MeshRenderMode getRenderType() { return typeRender; }
		bool getShowModel() { return showModels; }

	private:
		bool outilsPressed = false;
		bool modelsPressed = false;
		bool showModels = false;
		MeshRenderMode typeRender;

		ofTrueTypeFont buttonFont;

		ofImage imgImport;
		ofImage imgExport;
		ofImage imgColorWheel;
		ofImage imgPen;
		ofImage imgCircle;
		ofImage imgRectangle;
		ofImage imgTriangle;
		ofImage imgAnimation;
		ofImage imgModele;

		ofColor backgroundLineColor = ofColor(217, 217, 217);
		ofColor backgroundInteractionColor = ofColor(4, 3, 77);
		ofColor backgroundInteractionBorderColor = ofColor(215, 215, 224);

		ofColor textColor = backgroundLineColor;

		ofxPanel outilsGui;
		ofxPanel modelsGui;

		void backgroundLine();
		void backgroundInteraction();
		void topButtons();
		void sideButtons();

		void panelScene(); 
		ofColor backgroundPanelSceneColor = ofColor(125);
		//Renderer& renderer;
		void showOutils(bool& value);
		void enableFill(bool& value);
		void Interface::enableModels(bool& value);
		void Interface::modelFill(bool& value);
		void Interface::modelWireframe(bool& value);
		void Interface::modelPoints(bool& value);
};


