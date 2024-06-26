#include "Interface.h"
#include "Constants.h"
#include <vector>

using namespace std;

void Interface::setup() {
	//we only need to add the images of the buttons here to add it to the top bar
	imgImport.load("img/import.png");
	iconTopBar.push_back(imgImport);
	imgExport.load("img/export.png");
	iconTopBar.push_back(imgExport);
	imgAnimation.load("img/animation.png");
	iconTopBar.push_back(imgAnimation);
	imgMesh.load("img/mesh.png");
	iconTopBar.push_back(imgMesh);
	imgModele.load("img/model.png");
	iconTopBar.push_back(imgModele);
	imgCamera.load("img/camera.png");
	iconTopBar.push_back(imgCamera);
	imgCurve.load("img/curve.png");
	iconTopBar.push_back(imgCurve);
	imgLight.load("img/Light.png");
	iconTopBar.push_back(imgLight);

	//same here but for the side bar
	imgColorWheel.load("img/color-wheel.png");
	iconSideBar.push_back(imgColorWheel);
	imgPen.load("img/pen.png");
	iconSideBar.push_back(imgPen);
	imgCircle.load("img/circle.png");
	iconSideBar.push_back(imgCircle);
	imgRectangle.load("img/rectangle.png");
	iconSideBar.push_back(imgRectangle);
	imgTriangle.load("img/triangle.png");
	iconSideBar.push_back(imgTriangle);
	imgEllipse.load("img/ellipse.png");
	iconSideBar.push_back(imgEllipse);

	setupOutilsGuiOptions();
	setupCameraOptions();
	setupModelOptions();
	setupLightOptions();
	setupMeshOptions();

	sphereAmbientColorPicker.set("Couleur ambiante", ofColor(36, 36, 36));
	sphereDiffuseColorPicker.set("Couleur diffuse", ofColor(36, 0, 36));
	sphereEmissiveColorPicker.set("Couleur emissive", ofColor(0, 0, 86));
	sphereSpecularColorPicker.set("Couleur specular", ofColor(84, 84, 84));

	sphereMaterials.setup("Sphere materials");
	sphereMaterials.add(showSphereMaterials.setup("Add materials", false));
	sphereMaterials.add(sphereAmbientColorPicker);
	sphereMaterials.add(sphereDiffuseColorPicker);
	sphereMaterials.add(sphereEmissiveColorPicker);
	sphereMaterials.add(sphereSpecularColorPicker);
	sphereMaterials.add(sphereShininess.set("Shininess", 5, 0, 10));

	cubeAmbientColorPicker.set("Couleur ambiante", ofColor(125, 63, 84));
	cubeDiffuseColorPicker.set("Couleur diffuse", ofColor(13, 10, 13));
	cubeEmissiveColorPicker.set("Couleur emissive", ofColor(75, 75, 78));
	cubeSpecularColorPicker.set("Couleur specular", ofColor(131, 131, 131));

	cubeMaterials.setup("Cube materials");
	cubeMaterials.add(showCubeMaterials.setup("Add materials", false));
	cubeMaterials.add(cubeAmbientColorPicker);
	cubeMaterials.add(cubeDiffuseColorPicker);
	cubeMaterials.add(cubeEmissiveColorPicker);
	cubeMaterials.add(cubeSpecularColorPicker);
	cubeMaterials.add(cubeShininess.set("Shininess", 5, 0, 10));

	is_mouse_button_pressed = false;
	is_mouse_button_dragged = false;
	import_activate = false; 
	export_activate = false;
	curve_activate = false;

	typeRender = MeshRenderMode::wireframe;
	mouse_current_x = mouse_current_y = mouse_press_x = mouse_press_y = mouse_drag_x = mouse_drag_y = 0;
}

void Interface::draw() {
	backgroundInteraction();
	topButtons();
	sideButtons();
	draw_cursor(mouse_current_x, mouse_current_y);

	if (outilsPressed) {
		outilsGui.draw();
	}
	if (modelsPressed) {
		modelsGui.draw();
	}
	if (camPressed) {
		cameraGui.draw();
	}
	if (lightPressed) {
		lightGui.draw();
	}
	if (mailleVisible) {
		mailleGui.draw();
		if (meshButton) {
			mesh_activate = true;
		}
		else mesh_activate = false;
		if (meshAnimationButton) {
			noise_activate = true;
		}
		else noise_activate = false;
	}
	if (curvePressed) {
		curve_activate = true;
	}

	if (curvePressed) {
		curve_activate = true;
	}

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
	ofDrawBitmapString("Current camera: " + camera_name, 1630, 105);

}

void Interface::draw_cursor(float x, float y) const {
	float length = 10.0f;
	float offset = 5.0f;

	ofSetLineWidth(2);

	if (is_mouse_button_dragged) {
		ofSetColor(254, 142, 118); // Couleur rouge
		ofDrawCircle(x, y, 20);
	}

	else if (is_mouse_button_pressed) {
		ofSetColor(249, 220, 40); // Couleur jaune
		ofDrawRectangle(x - 20, y - 20, 40, 40);
	}
	else if (import_activate) {
		ofSetColor(135, 210, 88); // Couleur verte
		float tailleTriangle = 20;
		ofDrawTriangle(x, y - tailleTriangle, x - tailleTriangle, y + tailleTriangle, x + tailleTriangle, y + tailleTriangle);
	}
	else if (export_activate) {
		ofSetColor(0, 0, 255); // Couleur bleue
		float hexagoneSize = 20.0f; // Taille de l'hexagone
		ofSetColor(0, 0, 255); // Couleur bleue
		ofPushMatrix();
		ofTranslate(x, y); // Translation au position du curseur
		ofBeginShape();
		for (int i = 0; i < 6; ++i) {
			float angle = i * TWO_PI / 6;
			float px = hexagoneSize * cos(angle);
			float py = hexagoneSize * sin(angle);
			ofVertex(px, py);
		}
		ofEndShape(true);
		ofPopMatrix();

	}
	else {
		ofSetColor(255);
	}
	 // Couleur blanche

	ofDrawLine(x + offset, y, x + offset + length, y);
	ofDrawLine(x - offset, y, x - offset - length, y);
	ofDrawLine(x, y + offset, x, y + offset + length);
	ofDrawLine(x, y - offset, x, y - offset - length);
}

void Interface::topButtons() {
	for (int i = 0; i < iconTopBar.size(); i++) {
		if (i == 0) {
			iconTopBar[i].draw(0, 0, iconWidth, iconWidth);
		}
		else {
			iconTopBar[i].draw(10 + iconWidth * i, 0, iconWidth, iconWidth);
		}
	}
}

void Interface::sideButtons() {
	for (int i = 0; i < iconSideBar.size(); i++) {
		if (i == 0) {
			iconSideBar[i].draw(WIDTH - iconWidth, INTERACTION_BAR_HEIGHT, iconWidth, iconWidth);
		}
		else {
			iconSideBar[i].draw(WIDTH - iconWidth, INTERACTION_BAR_HEIGHT + iconWidth * i, iconWidth, iconWidth);
		}
	}
}

void Interface::showOutils(bool& value) {
	outilsPressed = !outilsPressed;
}

void Interface::toggleColorWheel() {
	outilsPressed = !outilsPressed;
}

void Interface::toggleModelOptions() {
	modelsPressed = !modelsPressed;
}

void Interface::modelFill(bool& value) {
	if (value) {
		setRenderType(MeshRenderMode::fill);
		wireframeRender = pointRender = false;
	}
}

void Interface::modelWireframe(bool& value) {
	if (value) {
		setRenderType(MeshRenderMode::wireframe);
		fillRender = pointRender = false;
	}
}

void Interface::modelPoints(bool& value) {
	if (value) {
		setRenderType(MeshRenderMode::vertex);
		fillRender = wireframeRender = false;
	}
}

void Interface::enableFill(bool& value) {
	fillEnabled = !fillEnabled;
}

void Interface::enableTextureFill(bool& value) {
	textureFill = !textureFill;
}

void Interface::toggleCamOptions() {
	camPressed = !camPressed;
}

void Interface::toggleLightOptions() {
	lightPressed = !lightPressed;
}

void Interface::toggleMailleGUI() {
	showMaille();
}

void Interface::showMaille() {
	mailleVisible = !mailleVisible;
}

void Interface::toggleCurveOptions() {
	curvePressed = !curvePressed;
}

void Interface::enableOrtho(bool& value) {
	if (value) {
		orthoIsActive = true;
		angleIsActive = false;
		angleActivate = false;
	}
}

void Interface::enableAngle(bool& value) {
	if (value) {
		angleIsActive = true;
		orthoIsActive = false;
		orthoActivate = false;
	}
}

void Interface::orthoSelect(bool& value) {
	if (value) {
		orthoRendering = true;
		perspRendering = false;
		renderAsPersp = false;
	}
}

void Interface::perspSelect(bool& value) {
	if (value) {
		perspRendering = true;
		orthoRendering = false;
		renderAsOrtho = false;
	}
}

void Interface::frontCamSelect(bool& value) {
	if (value) {
		frontCamRendering = true;
		backCamRendering = leftCamRendering = rightCamRendering = topCamRendering = bottomCamRendering = false;
		backCam = leftCam = rightCam = topCam = bottomCam = false;
	}
}

void Interface::backCamSelect(bool& value) {
	if (value) {
		backCamRendering = true;
		frontCamRendering = leftCamRendering = rightCamRendering = topCamRendering = bottomCamRendering = false;
		frontCam = leftCam = rightCam = topCam = bottomCam = false;
	}
}

void Interface::leftCamSelect(bool& value) {
	if (value) {
		leftCamRendering = true;
		backCamRendering = topCamRendering = rightCamRendering = topCamRendering = bottomCamRendering = false;
		backCam = topCam = rightCam = topCam = bottomCam = false;
	}
}

void Interface::rightCamSelect(bool& value) {
	if (value) {
		rightCamRendering = true;
		backCamRendering = leftCamRendering = frontCamRendering = topCamRendering = bottomCamRendering = false;
		backCam = leftCam = frontCam = topCam = bottomCam = false;
	}
}

void Interface::topCamSelect(bool& value) {
	if (value) {
		topCamRendering = true;
		backCamRendering = leftCamRendering = rightCamRendering = frontCamRendering = bottomCamRendering = false;
		backCam = leftCam = rightCam = frontCam = bottomCam = false;
	}
}

void Interface::bottomCamSelect(bool& value) {
	if (value) {
		bottomCamRendering = true;
		backCamRendering = leftCamRendering = rightCamRendering = topCamRendering = frontCamRendering = false;
		backCam = leftCam = rightCam = topCam = frontCam = false;
	}
}

void Interface::enableModels(bool& value) {
	showModels = !showModels;
}

void Interface::colorFillSelect(bool& value) {
	if (value) {
		illuminationType = 0;
		lambert = gouraud = phong = blinnPhong = false;
	}
}

void Interface::lambertSelect(bool& value) {
	if (value) {
		illuminationType = 1;
		colorFill = gouraud = phong = blinnPhong = false;
	}
}

void Interface::gouraudSelect(bool& value) {
	if (value) {
		illuminationType = 2;
		lambert = colorFill = phong = blinnPhong = false;
	}
}

void Interface::phongSelect(bool& value) {
	if (value) {
		illuminationType = 3;
		lambert = gouraud = colorFill = blinnPhong = false;
	}
}

void Interface::blinnPhongSelect(bool& value) {
	if (value) {
		illuminationType = 4;
		lambert = gouraud = phong = colorFill = false;
	}
}

void Interface::backgroundInteraction() {
	//Right rectangle
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
	//int numSegments = 8; // Nombre de segments � dessiner
	//float segmentHeight = (HEIGHT - 40) / numSegments; // Hauteur de chaque segment

	//for (int i = 1; i < numSegments; ++i) {
	//	// Dessiner une ligne horizontale pour segmenter la zone
	//	ofDrawLine(0, 41 + i * segmentHeight, 255, 41 + i * segmentHeight);
	//}
}

void Interface::setupOutilsGuiOptions() {
	outilsGui.setup();
	outilsGui.loadFont("roboto/Roboto-Regular.ttf", 10);
	outilsGui.setPosition(WIDTH - outilsGui.getWidth() - INTERACTION_BAR_HEIGHT, INTERACTION_BAR_HEIGHT);
	outilsGui.setSize(200, 1080);

	color_picker_background.set("Couleur du canevas", ofColor(31), ofColor(0, 0), ofColor(255, 255));
	color_picker_stroke.set("Couleur de la ligne de contour", ofColor(255), ofColor(0, 0), ofColor(255, 255));
	colorPickerFill.set("Couleur de remplissage", ofColor(255), ofColor(0, 0), ofColor(255, 255));
	slider_stroke_weight.set("Epaisseur de la ligne de contour", 4.0f, 0.0f, 10.0f);

	outilsGui.add(color_picker_background);
	outilsGui.add(color_picker_stroke);
	outilsGui.add(fillButton.setup("Remplissage de forme", false));
	outilsGui.add(textureFillButton.setup("Ajout d'une texture", false));
	outilsGui.add(colorPickerFill);
	outilsGui.add(slider_stroke_weight);

	fillButton.addListener(this, &Interface::enableFill);
	textureFillButton.addListener(this, &Interface::enableFill);

	outilsGui.setFillColor(backgroundInteractionColor);
	outilsGui.setTextColor(backgroundInteractionColor);
	outilsGui.maximizeAll();
	outilsGui.disableHeader();

}

void Interface::setupModelOptions() {
	modelsGui.setup("Model options");
	modelsGui.loadFont("roboto/Roboto-Regular.ttf", 10);
	modelsGui.setPosition(200, 40);
	modelsGui.setSize(200, 1080);

	texturedModels.setup("Textured Model Options");
	texturedModels.add(texturedPanel.setup("Panel", false));
	texturedModels.add(texturedBox.setup("Box", false));
	texturedModels.add(texturedSphere.setup("Sphere", false));

	modelsGui.add(modelToggle.setup("Show models", false));
	modelsGui.add(fillRender.setup("Fill render", false));
	modelsGui.add(wireframeRender.setup("Wireframe render", true));
	modelsGui.add(pointRender.setup("Vertex render", false));
	modelsGui.add(&texturedModels);
	
	modelToggle.addListener(this, &Interface::enableModels);
	fillRender.addListener(this, &Interface::modelFill);
	wireframeRender.addListener(this, &Interface::modelWireframe);
	pointRender.addListener(this, &Interface::modelPoints);

	teapotAmbientColorPicker.set("Couleur ambiante", ofColor(63, 63, 63));
	teapotDiffuseColorPicker.set("Couleur diffuse", ofColor(63, 0, 63));
	teapotEmissiveColorPicker.set("Couleur emissive", ofColor(0, 0, 31));
	teapotSpecularColorPicker.set("Couleur specular", ofColor(191, 191, 191));

	teapotMaterials.setup("Teapot materials");
	teapotMaterials.add(showTeapotMaterials.setup("Add materials", false));
	teapotMaterials.add(teapotAmbientColorPicker);
	teapotMaterials.add(teapotEmissiveColorPicker);
	teapotMaterials.add(teapotDiffuseColorPicker);
	teapotMaterials.add(teapotSpecularColorPicker);
	teapotMaterials.add(teapotShininess.set("Shininess", 5, 0, 10));

	modelsGui.add(&teapotMaterials);

	modelsGui.maximizeAll();
}

void Interface::setupCameraOptions() {
	cameraGui.setup();
	cameraGui.loadFont("roboto/Roboto-Regular.ttf", 10);
	cameraGui.setPosition(WIDTH / 2 - 250, INTERACTION_BAR_HEIGHT);
	cameraGui.setSize(250, 1080);

	cameraGui.add(orthoActivate.setup("Activate Perspective Camera", true));
	cameraGui.add(angleActivate.setup("Activate Angle Cameras", false));
	
	orthoOptions.setup("Perspective Camera Options");
	orthoOptions.add(renderAsOrtho.setup("Render orthographically", true));
	orthoOptions.add(renderAsPersp.setup("Render as perspective", false));

	angleOptions.setup("Multiple Camera Selection");
	angleOptions.add(frontCam.setup("See with Front Camera", true));
	angleOptions.add(backCam.setup("See with Back Camera", false));
	angleOptions.add(leftCam.setup("See with Left Camera", false));
	angleOptions.add(rightCam.setup("See with Right Camera", false));
	angleOptions.add(topCam.setup("See with Top Camera", false));
	angleOptions.add(bottomCam.setup("See with Bottom Camera", false));

	renderAsOrtho.addListener(this, &Interface::orthoSelect);
	renderAsPersp.addListener(this, &Interface::perspSelect);
	frontCam.addListener(this, &Interface::frontCamSelect);
	backCam.addListener(this, &Interface::backCamSelect);
	leftCam.addListener(this, &Interface::leftCamSelect);
	rightCam.addListener(this, &Interface::rightCamSelect);
	topCam.addListener(this, &Interface::topCamSelect);
	bottomCam.addListener(this, &Interface::bottomCamSelect);

	cameraGui.add(&orthoOptions);
	cameraGui.add(&angleOptions);

	orthoActivate.addListener(this, &Interface::enableOrtho);
	angleActivate.addListener(this, &Interface::enableAngle);

	cameraGui.setFillColor(backgroundInteractionColor);
	cameraGui.setTextColor(backgroundInteractionColor);
	cameraGui.maximizeAll();
	cameraGui.disableHeader();
}

void Interface::setupLightOptions() {
	lightGui.setup();
	lightGui.loadFont("roboto/Roboto-Regular.ttf", 10);
	lightGui.setPosition(WIDTH / 2 - 250, INTERACTION_BAR_HEIGHT);
	lightGui.setSize(200, 1080);

	ambientLightOptions.setup("Ambient Light Options");
	ambientLightOptions.add(showAmbientLight.setup("Show Ambient Light", false));
	ambientLightOptions.add(ambientLightColor);
	areaLightOptions.setup("Area Light Options");
	areaLightOptions.add(showAreaLight.setup("Show Area Light", false));
	areaLightOptions.add(areaLightWidth);
	areaLightOptions.add(areaLightHeight);
	areaLightOptions.add(&areaLightPosition);
	areaLightPosition.setup("Position of Area Light");
	areaLightPosition.add(areaLightPositionX);
	areaLightPosition.add(areaLightPositionY);
	areaLightPosition.add(areaLightPositionZ); //Determiner comment ajouter l'orientation
	areaLightOptions.add(&areaLightOrientation);
	areaLightOrientation.setup("Orientation of Area Light");
	areaLightOrientation.add(areaLightOrientationX);
	areaLightOrientation.add(areaLightOrientationY);
	areaLightOrientation.add(areaLightOrientationZ);
	areaLightOptions.add(areaLightColor);
	directionnalLightOptions.setup("Directionnal Light Options");
	directionnalLightOptions.add(showDirectionnalLight.setup("Show Directionnal Light", false));
	directionnalLightOptions.add(&directionnalLightPosition);
	directionnalLightPosition.setup("Position of Directionnal Light");
	directionnalLightPosition.add(directionnalLightPositionX);
	directionnalLightPosition.add(directionnalLightPositionY);
	directionnalLightPosition.add(directionnalLightPositionZ); //Determiner comment ajouter l'orientation
	directionnalLightOptions.add(&directionnalLightOrientation);
	directionnalLightOrientation.setup("Orientation of Directionnal Light");
	directionnalLightOrientation.add(directionnalLightOrientationX);
	directionnalLightOrientation.add(directionnalLightOrientationY);
	directionnalLightOrientation.add(directionnalLightOrientationZ);
	directionnalLightOptions.add(directionnalLightColor);
	spotLightOptions.setup("Spot Light Options");
	spotLightOptions.add(showSpotLight.setup("Show Spot Light", false));
	spotLightOptions.add(&spotLightPosition);
	spotLightPosition.setup("Position of Spot Light");
	spotLightPosition.add(spotLightPositionX);
	spotLightPosition.add(spotLightPositionY);
	spotLightPosition.add(spotLightPositionZ);
	spotLightOptions.add(&spotLightOrientation);
	spotLightOrientation.setup("Orientation of Spot Light");
	spotLightOrientation.add(spotLightOrientationX);
	spotLightOrientation.add(spotLightOrientationY);
	spotLightOrientation.add(spotLightOrientationZ);
	spotLightOptions.add(spotLightColor);
	pointLightOptions.setup("Point Light Options");
	pointLightOptions.add(showPointLight.setup("Show Point Light", false));
	pointLightOptions.add(&pointLightPosition);
	pointLightPosition.setup("Position of Point Light");
	pointLightPosition.add(pointLightPositionX);
	pointLightPosition.add(pointLightPositionY);
	pointLightPosition.add(pointLightPositionZ);
	pointLightOptions.add(pointLightColor);

	ambientLightColor.set("Ambient Light Color", ofColor(255, 0), ofColor(0, 0), ofColor(255, 0));
	areaLightColor.set("Area Light Color", ofColor(255), ofColor(0, 0), ofColor(255, 255));
	areaLightWidth.set("Area Light Width", 200.0f, 1.0f, 1000.0f);
	areaLightHeight.set("Area Light Height", 200.0f, 1.0f, 1000.0f);
	areaLightPositionX.set("X", 0.0f, -5000.0f, 5000.0f); //A verifier
	areaLightPositionY.set("Y", 0.0f, -5000.0f, 5000.0f);
	areaLightPositionZ.set("Z", 0.0f, -5000.0f, 5000.0f);
	areaLightOrientationX.set("X", 0.0f, 0.0f, 360.0f);
	areaLightOrientationY.set("Y", 0.0f, 0.0f, 360.0f);
	areaLightOrientationZ.set("Z", 0.0f, 0.0f, 360.0f);
	directionnalLightColor.set("Directionnal Light Color", ofColor(255), ofColor(0, 0), ofColor(255, 255));
	directionnalLightPositionX.set("X", 0.0f, -5000.0f, 5000.0f); //A verifier
	directionnalLightPositionY.set("Y", 0.0f, -5000.0f, 5000.0f);
	directionnalLightPositionZ.set("Z", 0.0f, -5000.0f, 5000.0f);
	directionnalLightOrientationX.set("X", 0.0f, 0.0f, 360.0f);
	directionnalLightOrientationY.set("Y", 0.0f, 0.0f, 360.0f);
	directionnalLightOrientationZ.set("Z", 0.0f, 0.0f, 360.0f);
	spotLightColor.set("Spot Light Color", ofColor(255), ofColor(0, 0), ofColor(255, 255));
	spotLightPositionX.set("X", 0.0f, -5000.0f, 5000.0f);
	spotLightPositionY.set("Y", 0.0f, -5000.0f, 5000.0f);
	spotLightPositionZ.set("Z", 0.0f, -5000.0f, 5000.0f);
	spotLightOrientationX.set("X", 0.0f, 0.0f, 360.0f);
	spotLightOrientationY.set("Y", 0.0f, 0.0f, 360.0f);
	spotLightOrientationZ.set("Z", 0.0f, 0.0f, 360.0f);
	pointLightColor.set("Point Light Color", ofColor(255), ofColor(0, 0), ofColor(255, 255));
	pointLightPositionX.set("X Position of point Light", 0.0f, -5000.0f, 5000.0f);
	pointLightPositionY.set("Y Position of point Light", 0.0f, -5000.0f, 5000.0f);
	pointLightPositionZ.set("Z Position of point Light", 0.0f, -5000.0f, 5000.0f);

	modelesIllumination.setup("Illumination Models");
	modelesIllumination.add(activateModelesIllumination.setup("Activate Illumination Models", false));
	modelesIllumination.add(colorFill.setup("Color Fill", false));
	modelesIllumination.add(lambert.setup("Lambert", false));
	modelesIllumination.add(gouraud.setup("Gouraud", false));
	modelesIllumination.add(phong.setup("Phong", false));
	modelesIllumination.add(blinnPhong.setup("Blinn-Phong", true));


	colorFill.addListener(this, &Interface::colorFillSelect);
	lambert.addListener(this, &Interface::lambertSelect);
	gouraud.addListener(this, &Interface::gouraudSelect);
	phong.addListener(this, &Interface::phongSelect);
	blinnPhong.addListener(this, &Interface::blinnPhongSelect);

	lightGui.add(&ambientLightOptions);
	lightGui.add(&areaLightOptions);
	lightGui.add(&directionnalLightOptions);
	lightGui.add(&spotLightOptions);
	lightGui.add(&pointLightOptions);
	lightGui.add(activateMultiShader.setup("Activate Multilighting",false));
	lightGui.add(&modelesIllumination);
	ambientLightOptions.minimizeAll();
	areaLightOptions.minimizeAll();
	directionnalLightOptions.minimizeAll();
	spotLightOptions.minimizeAll();
	pointLightOptions.minimizeAll();
	lightGui.minimizeAll();
	lightGui.disableHeader();
}

void Interface::setupMeshOptions() {
	mailleGui.setup("Mesh");
	mailleGui.loadFont("roboto/Roboto-Regular.ttf", 10);
	mailleGui.setPosition(200, 40);
	meshButton.setName("Mesh");
	mailleGui.add(meshButton);
	meshAnimationButton.setName("Mesh animation");
	mailleGui.add(meshAnimationButton);
	mailleGui.add(delaunay.setup("Delaunay Triangulation", false));
}
