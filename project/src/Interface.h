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
		ofxToggle textureFillButton;
		ofParameter<ofColor> colorPickerFill;
		ofParameter<float> slider_stroke_weight;
		bool fillEnabled = false;
		bool textureFill = false;

		string camera_name;

		ofxToggle orthoActivate;
		ofxToggle angleActivate;
		bool orthoIsActive = true;
		bool angleIsActive = false;
		ofxGuiGroup orthoOptions;
		ofxToggle renderAsOrtho;
		ofxToggle renderAsPersp;
		bool orthoRendering = true;
		bool perspRendering = false;
		ofxGuiGroup angleOptions;
		ofxToggle frontCam;
		ofxToggle backCam;
		ofxToggle leftCam;
		ofxToggle rightCam;
		ofxToggle topCam;
		ofxToggle bottomCam;
		bool frontCamRendering = true;
		bool backCamRendering = false;
		bool leftCamRendering = false;
		bool rightCamRendering = false;
		bool topCamRendering = false;
		bool bottomCamRendering = false;

		ofxGuiGroup ambientLightOptions; // Options pour lumiere ambiente
		ofxToggle showAmbientLight;
		ofParameter<ofColor> ambientLightColor;
		ofxGuiGroup areaLightOptions;
		ofxToggle showAreaLight;
		ofParameter<float> areaLightWidth;
		ofParameter<float> areaLightHeight;
		ofxGuiGroup areaLightPosition;
		ofParameter<float> areaLightPositionX;
		ofParameter<float> areaLightPositionY;
		ofParameter<float> areaLightPositionZ;
		ofxGuiGroup areaLightOrientation;
		ofParameter<float> areaLightOrientationX;
		ofParameter<float> areaLightOrientationY;
		ofParameter<float> areaLightOrientationZ;
		ofParameter<ofColor> areaLightColor;
		ofxGuiGroup directionnalLightOptions; // Options pour lumiere directionnelle
		ofxToggle showDirectionnalLight;
		ofxGuiGroup directionnalLightPosition;
		ofParameter<float> directionnalLightPositionX;
		ofParameter<float> directionnalLightPositionY;
		ofParameter<float> directionnalLightPositionZ;
		ofxGuiGroup directionnalLightOrientation;
		ofParameter<float> directionnalLightOrientationX;
		ofParameter<float> directionnalLightOrientationY;
		ofParameter<float> directionnalLightOrientationZ;
		ofParameter<ofColor> directionnalLightColor;
		ofxGuiGroup spotLightOptions; // Options pour lumiere "spot"
		ofxToggle showSpotLight;
		ofxGuiGroup spotLightPosition;
		ofParameter<float> spotLightPositionX;
		ofParameter<float> spotLightPositionY;
		ofParameter<float> spotLightPositionZ;
		ofxGuiGroup spotLightOrientation;
		ofParameter<float> spotLightOrientationX;
		ofParameter<float> spotLightOrientationY;
		ofParameter<float> spotLightOrientationZ;
		ofParameter<ofColor> spotLightColor;
		ofxGuiGroup pointLightOptions; // Options pour lumiere de point
		ofxToggle showPointLight;
		ofxGuiGroup pointLightPosition;
		ofParameter<float> pointLightPositionX;
		ofParameter<float> pointLightPositionY;
		ofParameter<float> pointLightPositionZ;
		ofParameter<ofColor> pointLightColor;
		ofxToggle activateMultiShader;
		ofxGuiGroup modelesIllumination;
		ofxToggle activateModelesIllumination;
		ofxToggle colorFill;
		ofxToggle lambert;
		ofxToggle gouraud;
		ofxToggle phong;
		ofxToggle blinnPhong;
		int illuminationType = 4;


		int mouse_press_x;
		int mouse_press_y;

		int mouse_current_x;
		int mouse_current_y;

		int mouse_drag_x;
		int mouse_drag_y;

		bool is_mouse_button_pressed;
		bool is_mouse_button_dragged;
		bool import_activate;
		bool export_activate;

		ofxToggle modelToggle;
		ofxToggle fillRender;
		ofxToggle wireframeRender;
		ofxToggle pointRender;
		ofxGuiGroup texturedModels;
		ofxToggle texturedPanel;
		ofxToggle texturedSphere;
		ofxToggle texturedBox;

		ofxGuiGroup teapotMaterials;
		ofxToggle showTeapotMaterials;
		ofParameter<ofColor> teapotAmbientColorPicker;
		ofParameter<ofColor> teapotDiffuseColorPicker;
		ofParameter<ofColor> teapotEmissiveColorPicker;
		ofParameter<ofColor> teapotSpecularColorPicker;
		ofParameter <float> teapotShininess;

		ofxGuiGroup sphereMaterials;
		ofxToggle showSphereMaterials;
		ofParameter<ofColor> sphereAmbientColorPicker;
		ofParameter<ofColor> sphereDiffuseColorPicker;
		ofParameter<ofColor> sphereEmissiveColorPicker;
		ofParameter<ofColor> sphereSpecularColorPicker;
		ofParameter <float> sphereShininess;

		ofxGuiGroup cubeMaterials;
		ofxToggle showCubeMaterials;
		ofParameter<ofColor> cubeAmbientColorPicker;
		ofParameter<ofColor> cubeDiffuseColorPicker;
		ofParameter<ofColor> cubeEmissiveColorPicker;
		ofParameter<ofColor> cubeSpecularColorPicker;
		ofParameter <float> cubeShininess;

		bool mesh_activate;
		bool noise_activate;
		bool curve_activate;
		ofxToggle delaunay;

		void setup();
		void draw();
		void toggleColorWheel();
		void toggleModelOptions();
		void setRenderType(MeshRenderMode renderMode) { typeRender = renderMode; }
		MeshRenderMode getRenderType() { return typeRender; }
		bool getShowModel() { return showModels; }
		void toggleCamOptions();
		void toggleLightOptions();
		void toggleMailleGUI();
		void toggleCurveOptions();

	private:
		bool outilsPressed = false;
		bool camPressed = false;
		bool curvePressed = false;
		
		bool modelsPressed = false;
		bool lightPressed = false;
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
		ofImage imgEllipse;
		ofImage imgMesh;
		ofImage imgCamera;
		ofImage imgLight;
		ofImage imgCurve;

		ofColor backgroundInteractionColor = ofColor(4, 3, 77);
		ofColor backgroundInteractionBorderColor = ofColor(215, 215, 224);

		ofColor textColor = ofColor(217, 217, 217);;

		ofxPanel outilsGui;
		ofxPanel modelsGui;
		ofxPanel cameraGui;
		ofxPanel lightGui;
		ofxPanel mailleGui;

		bool mailleVisible = false;
		ofParameter<bool> meshButton = false;
		ofParameter<bool> meshAnimationButton = false;

		void backgroundInteraction();
		void topButtons();
		void sideButtons();
		void draw_cursor(float x, float y) const;

		void panelScene(); 
		ofColor backgroundPanelSceneColor = ofColor(125);
		void setupCameraOptions();
		void setupOutilsGuiOptions();
		void setupModelOptions();
		void setupLightOptions();
		void setupMeshOptions();
		//Renderer& renderer;
		void showOutils(bool& value);
		void showMaille();
		void enableFill(bool& value);
		void enableTextureFill(bool& value);
		void enableOrtho(bool& value);
		void enableAngle(bool& value);
		void orthoSelect(bool& value);
		void perspSelect(bool& value);
		void frontCamSelect(bool& value);
		void backCamSelect(bool& value);
		void leftCamSelect(bool& value);
		void rightCamSelect(bool& value);
		void topCamSelect(bool& value);
		void bottomCamSelect(bool& value);
		void enableModels(bool& value);
		void modelFill(bool& value);
		void modelWireframe(bool& value);
		void modelPoints(bool& value);
		void colorFillSelect(bool& value);
		void lambertSelect(bool& value);
		void gouraudSelect(bool& value);
		void phongSelect(bool& value);
		void blinnPhongSelect(bool& value);
};


