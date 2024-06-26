﻿#include "Application.h"
#include "Constants.h"
#include <cmath>
#include <iostream>

using namespace std;

void Application::setup(){
	ofSetWindowTitle("BIBO");
	ofBackground(backgroundColor);
	renderer.setup();

	//toggleDraw, toggleTransform
	drawingGUI.setup();
	drawingGUI.setPosition(0, 40);//(300, 40)
	primitivesMode.setup("Mode");
	primitivesMode.add(toggleDraw.setup("Drawing", false));
	primitivesMode.add(toggleTransform.setup("Transformation", false));
	renderer.inputIndex.setup("Index de la liste: ", 0);
	drawingGUI.add(&renderer.inputIndex);
	//gui.add(renderer.uiIndex.set("Index de la liste: ", int(0))); 
	toggleDraw.addListener(this, &Application::button_modeDraw);
	toggleTransform.addListener(this, &Application::button_modeTransform);
	drawingGUI.add(&primitivesMode);

	primitivesGroupe.setup("Primitives");

	// Ajout des boutons pour les primitives
	primitivesGroupe.add(toggleDrawTriangle.setup("Triangle",false));
	primitivesGroupe.add(toggleDrawCircle.setup("Circle",false));
	primitivesGroupe.add(toggleDrawRectangle.setup("Rectangle",false));
	primitivesGroupe.add(toggleDrawLine.setup("Line", false));
	primitivesGroupe.add(toggleDrawEllipse.setup("Ellipse", false));
	primitivesGroupe.add(toggleDrawBezier.setup("Bezier", false));
	primitivesGroupe.add(drawSphere.setup("Sphere", false));
	primitivesGroupe.add(&renderer.interface.sphereMaterials);
	primitivesGroupe.add(drawCube.setup("Cube", false));
	primitivesGroupe.add(&renderer.interface.cubeMaterials);
	
	// Associer des fonctions de rappel aux boutons
	toggleDrawTriangle.addListener(this, &Application::button_triangle);
	toggleDrawCircle.addListener(this, &Application::button_circle);
	toggleDrawRectangle.addListener(this, &Application::button_rectangle);
	toggleDrawLine.addListener(this, &Application::button_line);
	toggleDrawEllipse.addListener(this, &Application::button_ellipse);
	toggleDrawBezier.addListener(this, &Application::button_bezier);
	drawSphere.addListener(this, &Application::button_sphere); 
	drawCube.addListener(this, &Application::button_cube);


	drawingGUI.add(&primitivesGroupe);
	drawingGUI.add(renderer.uiPosition.set("position", ofVec2f(0), ofVec2f(0), ofVec2f(ofGetWidth(), ofGetHeight()))); // La position des primitives
	drawingGUI.add(renderer.uiAmount.set("amount", 1, 0, 5));
	drawingGUI.add(renderer.uiStep.set("step", ofVec2f(0), ofVec2f(0), ofVec2f(300)));
	drawingGUI.add(renderer.uiRotate.set("rotate", ofVec3f(0), ofVec3f(-180), ofVec3f(180))); // La rotation des primitives
	drawingGUI.add(renderer.uiShift.set("shift", ofVec2f(0), ofVec2f(0), ofVec2f(300)));
	drawingGUI.add(renderer.uiSize.set("size", ofVec2f(1), ofVec2f(1), ofVec2f(10)));
	drawingGUI.minimizeAll();

	camera_setup_perspective(WIDTH, HEIGHT, 60.0f, 0.0f, 0.0f);
	cam.enableOrtho();
	orthoEnabled = true;
	reset_cam();
	setupCamera();
	is_visible_camera = true;
	
	draw_triangle = false;
	draw_circle = false;
	draw_rectangle = false;
	rotation_activate = false;
	mesh_activate = false;
	noise_activate = false;
	catmullRom_activate = false;
	catmullRom6_activate = false;
	hermite_activate = false;

	gray_activate = false;
	sharpen_activate = false;
	emboss_activate = false;

	reinitialisationGroupe.setup("Reinitialisation");
	reinitialisationGroupe.add(resetButton.setup("Reset", false));
	resetButton.addListener(this, &Application::reset);
	drawingGUI.add(&reinitialisationGroupe);

	animationGroupe.setup("Animations");
	animationGroupe.add(rotationButton.setup("Rotation", false));
	rotationButton.addListener(this, &Application::button_rotation);
	animationGroupe.minimize(); 
	drawingGUI.add(&animationGroupe);

	curveGui.setup("Curve");
	curveGui.loadFont("roboto/Roboto-Regular.ttf", 10);
	curveGui.setPosition(600, 50);
	curveGui.add(segments.set("Segments", 150, 150, 400));
	catmullRomGroupe.setup("Catmull-Rom's curve"); 
	catmullRomButton.setName("5 points");
	catmullRomGroupe.add(catmullRomButton);
	catmullRomButton.addListener(this, &Application::button_catmullRom);
	catmullRomButton6.setName("6 points");
	catmullRomGroupe.add(catmullRomButton6);
	catmullRomButton6.addListener(this, &Application::button_catmullRom6);
	curveGui.add(&catmullRomGroupe);
	hermiteGroupe.setup("Hermite's curve");
	hermiteButton.setName("3 curves");
	hermiteGroupe.add(hermiteButton);
	hermiteButton.addListener(this, &Application::button_hermite);
	curveGui.add(&hermiteGroupe);
	surfaceParametriqueGroup.setup("Surface parametrique");
	coonsButton.setName("Coons");
	surfaceParametriqueGroup.add(coonsButton);
	coonsButton.addListener(this, &Application::button_coons);
	curveGui.add(&surfaceParametriqueGroup);

	filterGUI.setup();
	filterGUI.setPosition(700, 70);
	textureGroupe.setup("Filtres (Textures)");
	textureGroupe.add(sphereTextureButton.setup("Sphere", false));
	sphereTextureButton.addListener(this, &Application::button_sphereTexture);
	filterGUI.add(&textureGroupe);
	filterGroupe.setup("Filtres");
	filterGUI.add(color_picker.set("teinte", renderer.tint, ofColor(0, 0), ofColor(255, 255)));
	filterGUI.add(slider.set("mix", renderer.mix_factor, 0.0f, 1.0f));
	filterGroupe.add(grayButton);
	grayButton.setName("Black and white");
	grayButton.addListener(this, &Application::button_blackAndWhite);
	filterGroupe.add(sharpenButton);
	sharpenButton.setName("Sharpen");
	sharpenButton.addListener(this, &Application::button_sharpen);
	filterGroupe.add(embossButton);
	embossButton.setName("Emboss");
	embossButton.addListener(this, &Application::button_emboss);
	filterGUI.add(&filterGroupe);

	// CrÃƒÂ©ation de la maille
	for (int x = 0; x < size; x++) {
		for (int y = 0; y < size; y++) {
			mesh.addVertex(ofPoint(x - size / 2, y - size / 2));
		}
	}

	for (int y = 0; y < size - 1; y++) {
		for (int x = 0; x < size - 1; x++) {
			mesh.addIndex(x + y * size);
			mesh.addIndex((x + 1) + y * size);
			mesh.addIndex(x + (y + 1) * size);
			mesh.addIndex((x + 1) + y * size);
			mesh.addIndex((x + 1) + (y + 1) * size);
			mesh.addIndex(x + (y + 1) * size);
		}
	}

	renderer.setup(renderer.v_formes);
	forme.setup();
	diffX = (abs(forme.getX2() - forme.getX3())) / 2;
	diffY = abs(forme.getY1() - forme.getY2());
	newX2 = 0;
	newY2 = 0;
	newX3 = 0;
	newY3 = 0;

	// état par défaut des bool
	draw_triangle = draw_circle = draw_rectangle =
		draw_line = draw_ellipse = draw_bezier = draw_sphere = false;
	

	shapeBool = false; 
	v_buttons_ptr = &v_buttons;
	//guiScene.setup();
	guiScene.setPosition(0, 40);

	addAction([this]() { undo(); }, [this]() { redo(); });

	controlPoints.push_back(ofVec2f(100, 300));
	controlPoints.push_back(ofVec2f(200, 100));
	controlPoints.push_back(ofVec2f(300, 500));
	controlPoints.push_back(ofVec2f(400, 200));
	controlPoints.push_back(ofVec2f(500, 400));
	controlPoints.push_back(ofVec2f(600, 100));
	controlPoints.push_back(ofVec2f(700, 300));
	controlPoints.push_back(ofVec2f(800, 200));
	controlPoints.push_back(ofVec2f(900, 200));
	controlPoints.push_back(ofVec2f(1000, 100));
	controlPoints.push_back(ofVec2f(1100, 100));
	controlPoints.push_back(ofVec2f(1200, 200));

	// Aucun point de contrôle n'est sélectionné au début
	selectedPointIndex = -1;

	// Modele Illumination
	is_key_press_up = is_key_press_down = is_key_press_left = is_key_press_right = false;

	// Coons
	renderer.selected_ctrl_point = &renderer.controlPoint0;
	controlPtZ = 0.0f; //pour le mousewheel avec Coons sur axe des Z

	resetTexture();

	// Texture
	groupe_activer_texture.setup("Texture");
	groupe_activer_texture.add(toggleTexture.setup("Add texture", false));
	toggleTexture.addListener(this, &Application::selectTexture);
	drawingGUI.add(&groupe_activer_texture);

	group_material_color.setup("color");
	group_material_color.add(color_picker_ambient);
	group_material_color.add(color_picker_diffuse);
	group_material_color.add(color_picker_specular);
	drawingGUI.add(&group_material_color);
	group_material_factor.setup("factor");
	group_material_factor.add(slider_metallic);
	group_material_factor.add(slider_roughness);
	group_material_factor.add(slider_occlusion);
	group_material_factor.add(slider_brightness);
	group_material_factor.add(slider_fresnel_ior);
	drawingGUI.add(&group_material_factor);
	group_light.setup("light");
	group_light.add(color_picker_light_color);
	group_light.add(slider_light_intensity);
	group_light.add(toggle_light_motion);
	drawingGUI.add(&group_light);
	group_tone_mapping.setup("tone mapping");
	group_tone_mapping.add(slider_exposure);
	group_tone_mapping.add(slider_gamma);
	group_tone_mapping.add(toggle_tone_mapping);
	drawingGUI.add(&group_tone_mapping);

	color_picker_ambient.set("ambient", renderer.material_color_ambient, ofColor(0, 0), ofColor(255, 255));
	color_picker_diffuse.set("diffuse", renderer.material_color_diffuse, ofColor(0, 0), ofColor(255, 255));
	color_picker_specular.set("specular", renderer.material_color_specular, ofColor(0, 0), ofColor(255, 255));

	slider_metallic.set("metallic", renderer.material_metallic, 0.0f, 1.0f);
	slider_roughness.set("roughness", renderer.material_roughness, 0.0f, 1.0f);
	slider_occlusion.set("occlusion", renderer.material_occlusion, 0.0f, 5.0f);
	slider_brightness.set("brightness", renderer.material_brightness, 0.0f, 5.0f);

	slider_fresnel_ior.set("fresnel ior", renderer.material_fresnel_ior, glm::vec3(0.0f), glm::vec3(1.0f));

	color_picker_light_color.set("color", renderer.light_color, ofColor(0, 0), ofColor(255, 255));
	slider_light_intensity.set("intensity", renderer.light_intensity, 0.0f, 10.0f);

	toggle_light_motion.set("motion", renderer.light_motion);

	slider_exposure.set("exposure", 1.0f, 0.0f, 5.0f);
	slider_gamma.set("gamma", 2.2f, 0.0f, 5.0f);

	if (renderer.tone_mapping_toggle)
		toggle_tone_mapping.set("aces filmic", true);
	else
		toggle_tone_mapping.set("reinhard", false);
}

void Application::resetTexture(){
	color_picker_ambient.set(ofColor(63, 63, 63));
	color_picker_diffuse.set(ofColor(255, 255, 255));
	color_picker_specular.set(ofColor(255, 255, 255));
	slider_metallic.set(0.5f);
	slider_roughness.set(0.5f);
	slider_occlusion.set(1.0f);
	slider_brightness.set(1.0f);
	slider_fresnel_ior.set(glm::vec3(0.04f, 0.04f, 0.04f));
	color_picker_light_color.set(ofColor(255, 255, 255));
	slider_light_intensity.set(1.0f);
	slider_exposure.set(1.0f);
	slider_gamma.set(2.2f);
	toggle_light_motion.set(true);
	toggle_tone_mapping.set("aces filmic", true);
}
void Application::selectTexture(bool &value) {
	renderer.isTexture = !renderer.isTexture; 
}

void Application::update()
{
	// Rotations des primitives vectorielles
	rotate++;

	renderer.update();
	renderer.tint = color_picker;
	renderer.mix_factor = slider;
	
	if (renderer.isRecording) {
		// Mettez Ã¯Â¿Â½ jour et capturez l'image Ã¯Â¿Â½ intervalles rÃ¯Â¿Â½guliers
		renderer.captureImage();
	}


	if (moveCameraLeft) {
		cam.move(-1, 0, 0); // DÃ©placer la camÃ©ra vers la gauche
	}

	if (moveCameraRight) {
		cam.move(1, 0, 0); // DÃ©placer la camÃ©ra vers la droite
	}

	if (moveCameraUp) {
		cam.move(0, 1, 0); // DÃ©placer la camÃ©ra vers le haut
	}

	if (moveCameraDown) {
		cam.move(0, -1, 0); // DÃ©placer la camÃ©ra vers le bas
	}
	if (moveCameraNear) {
		cam.move(0, 0, 1); // DÃ©placer la camÃ©ra en s'approchant
	}

	if (moveCameraFar) {
		cam.move(0, 0, -1); // DÃ©placer la camÃ©ra en s'eloignant
	}

	/// Modele coons
	time_current = ofGetElapsedTimef();
	time_elapsed = time_current - time_last;
	time_last = time_current;
	if (renderer.isCoons && !renderer.isModeIllumination)
	{
		if (is_key_press_up) {
			renderer.selected_ctrl_point->y -= renderer.delta_y * time_elapsed;
		}
		if (is_key_press_down) {
			renderer.selected_ctrl_point->y += renderer.delta_y * time_elapsed;
		}
		if (is_key_press_left) {
			renderer.selected_ctrl_point->x -= renderer.delta_x * time_elapsed;
		}
		if (is_key_press_right) {
			renderer.selected_ctrl_point->x += renderer.delta_x * time_elapsed;
		}
		if (renderer.isCoons) {
			renderer.selected_ctrl_point->z += controlPtZ * 5;
			controlPtZ = 0.0f;
		}
	}
	if (!renderer.isCoons && renderer.isModeIllumination)
	{
		if (is_key_press_up){renderer.offset_z += renderer.delta_z * time_elapsed;}
			
		if (is_key_press_down){renderer.offset_z -= renderer.delta_z * time_elapsed;}
			
		if (is_key_press_left){renderer.offset_x += renderer.delta_x * time_elapsed;}
			
		if (is_key_press_right){}renderer.offset_x -= renderer.delta_x * time_elapsed;
			
	}
	////////////////////////////
	// Texture
	if(renderer.isTexture)
	{

		//////////////////////////////
		time_current = ofGetElapsedTimef();
		time_elapsed = time_current - time_last;
		time_last = time_current;

		if (is_key_press_up)
			renderer.offset_z += renderer.delta_z * time_elapsed;
		if (is_key_press_down)
			renderer.offset_z -= renderer.delta_z * time_elapsed;
		if (is_key_press_left)
			renderer.offset_x += renderer.delta_x * time_elapsed;
		if (is_key_press_right)
			renderer.offset_x -= renderer.delta_x * time_elapsed;
		//if (is_key_press_q)
		//	renderer.rotation_y += renderer.delta_y * time_elapsed;
		//if (is_key_press_e)
		//	renderer.rotation_y -= renderer.delta_y * time_elapsed;

		renderer.material_color_ambient = color_picker_ambient;
		renderer.material_color_diffuse = color_picker_diffuse;
		renderer.material_color_specular = color_picker_specular;

		renderer.material_metallic = slider_metallic;
		renderer.material_roughness = slider_roughness;
		renderer.material_occlusion = slider_occlusion;
		renderer.material_brightness = slider_brightness;

		renderer.material_fresnel_ior = slider_fresnel_ior;

		renderer.light_color = color_picker_light_color;
		renderer.light_intensity = slider_light_intensity;
		renderer.light_motion = toggle_light_motion;

		renderer.tone_mapping_exposure = slider_exposure;
		renderer.tone_mapping_gamma = slider_gamma;
		renderer.tone_mapping_toggle = toggle_tone_mapping;

		if (renderer.tone_mapping_toggle)
			toggle_tone_mapping.set("aces filmic", true);
		else
			toggle_tone_mapping.set("reinhard", false);

		renderer.update();
	}
}

void Application::mouseScrolled(int x, int y, float scrollX, float scrollY) {
	controlPtZ += scrollY;
}

void Application::draw(){
	if (isImportable) {
		renderer.interface.import_activate = true;
		ofDrawBitmapString("Please drag an image to import it.", 30, 70);
		ofSetColor(renderer.tint);
		//ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);
	}
	//ofDisableBlendMode();

	if (renderer.interface.orthoIsActive) {
		if (renderer.interface.orthoRendering) {
			cam.enableOrtho();
		}
		else if (renderer.interface.perspRendering) {
			cam.disableOrtho();
		}
		cam.begin();
	}
	else if (renderer.interface.angleIsActive) {
		if (renderer.interface.frontCamRendering) {
			camera_active = CameraPosition::front;
			setupCamera();
		}
		else if (renderer.interface.backCamRendering) {
			camera_active = CameraPosition::back;
			setupCamera();
		}
		else if (renderer.interface.leftCamRendering) {
			camera_active = CameraPosition::left;
			setupCamera();
		}
		else if (renderer.interface.rightCamRendering) {
			camera_active = CameraPosition::right;
			setupCamera();
		}
		else if (renderer.interface.topCamRendering) {
			camera_active = CameraPosition::top;
			setupCamera();
		}
		else if (renderer.interface.bottomCamRendering) {
			camera_active = CameraPosition::down;
			setupCamera();
		}

		camera->begin();

		if (is_visible_camera)
		{
			if (camera_active != CameraPosition::front)
				camFront.draw();
			if (camera_active != CameraPosition::back)
				camBack.draw();
			if (camera_active != CameraPosition::left)
				camLeft.draw();
			if (camera_active != CameraPosition::right)
				camRight.draw();
			if (camera_active != CameraPosition::top)
				camTop.draw();
			if (camera_active != CameraPosition::down)
				camBottom.draw();
		}
	}

	if (renderer.interface.import_activate) {
		filterGUI.draw();
	}

	renderer.draw();

	//ofPopMatrix();
	if (renderer.interface.orthoIsActive) {
		cam.end();
	}
	else if (renderer.interface.angleIsActive) {
		camera->end();
	}

	renderer.interface.draw();

	if (drawingGUIPressed) {
		drawingGUI.draw();
	}

	if (renderer.interface.curve_activate) {
		curveGui.draw();
		if (catmullRom_activate) {
			ofSetColor(255);
			for (auto& p : controlPoints) {
				ofDrawCircle(p, 5);
			}

			ofSetColor(255, 0, 0);
			for (auto& p : controlPoints) {
				ofDrawCircle(p, 2);
			}

			ofSetColor(255, 0, 0);

			for (unsigned int i = 0; i < controlPoints.size() - 4; i ++) { 
				for (int j = 0; j <= segments; j++) {
					float t = (float)j / segments;
					ofVec2f p0 = controlPoints[i];
					ofVec2f p1 = controlPoints[i + 1];
					ofVec2f p2 = controlPoints[i + 2];
					ofVec2f p3 = controlPoints[i + 3];
					ofVec2f p4 = controlPoints[i + 4];
					ofVec2f p = catmullRom(t, p0, p1, p2, p3, p4);
					ofDrawCircle(p, 2);
				}
			}
		}
		if (catmullRom6_activate) {
			ofSetColor(255);
			for (auto& p : controlPoints) {
				ofDrawCircle(p, 5);
			}

			ofSetColor(255, 0, 0);
			for (auto& p : controlPoints) {
				ofDrawCircle(p, 2);
			}

			ofSetColor(255, 0, 0);
      
			for (unsigned int i = 1; i < controlPoints.size() - 4; i ++) {
				for (int j = 0; j <= segments; j++) {
					float t = (float)j / segments;
					ofVec2f p0 = controlPoints[i - 1];
					ofVec2f p1 = controlPoints[i];
					ofVec2f p2 = controlPoints[i + 1];
					ofVec2f p3 = controlPoints[i + 2];
					ofVec2f p4 = controlPoints[i + 3];
					ofVec2f p5 = controlPoints[i + 4];
					ofVec2f p = catmullRom6(t, p0, p1, p2, p3, p4, p5);
					ofDrawCircle(p, 2);
				}
			}
		}
		if (hermite_activate) {
			ofSetColor(255);
			for (auto& p : controlPoints) {
				ofDrawCircle(p, 5);
			}

			ofSetColor(255, 0, 0);
			for (auto& p : controlPoints) {
				ofDrawCircle(p, 2);
			}

			ofSetColor(255, 0, 0);

			for (int i = 0; i < controlPoints.size() - 4; i++) { 
				for (int j = 0; j <= segments; j++) {
					float t = (float)j / segments;
					ofVec2f p0 = controlPoints[i];
					ofVec2f p1 = controlPoints[i + 1];
					ofVec2f p2 = controlPoints[i + 2];
					ofVec2f p3 = controlPoints[i + 3];
					ofVec2f p4 = controlPoints[i + 4];
					float x, y;
					hermite(t, p0, p1, p2, p3, p4, x, y);
					ofDrawCircle(x, y, 2);
				}
			}
		}
	}
	guiScene.draw();
}

void Application::toggleDrawingGUI(Forme::TypeForme drawingShape) {
	if (lastShape == drawingShape) {
		drawingGUIPressed = !drawingGUIPressed;
	}
	else {
		lastShape = drawingShape;
		drawingGUIPressed = true;
	}
}


void Application::keyPressed(int key) 
{
	// DÃ¯Â¿Â½marrer/arrÃ¯Â¿Â½ter l'enregistrement lors de l'appui sur la touche 'r'
	if (key == 'r') 
	{
		renderer.isRecording = !renderer.isRecording;
		if (renderer.isRecording) 
		{
			renderer.frameCounter = 0; // RÃ¯Â¿Â½initialiser le compteur de frames lors du dÃ¯Â¿Â½marrage de l'enregistrement
			std::cout << "Enregistrement demarrer." << endl;
			//renderer.update();
			//renderer.captureImage();
		}
		else {
			std::cout << "Enregistrement arreter." << endl;
		}
	}
	/// Ajout du false pour retirer le modeIllumination 
	if (renderer.isModeIllumination == false && renderer.isCoons == false)
	{
		if (key == OF_KEY_LEFT) {
			moveCameraLeft = true;
		}
		if (key == OF_KEY_RIGHT) {
			moveCameraRight = true;
		}
		if (key == OF_KEY_UP) {
			moveCameraUp = true;
		}
		if (key == OF_KEY_DOWN) {
			moveCameraDown = true;
		}
		if (key == 49) {
			moveCameraNear = true;
		}
		if (key == 50) {
			moveCameraFar = true;
		}
	}
	// Coons
	if (renderer.isCoons)
	{
		switch (key)
		{
		case OF_KEY_LEFT: // touche ?
			is_key_press_left = true;
			break;

		case OF_KEY_UP: // touche ?
			is_key_press_up = true;
			break;

		case OF_KEY_RIGHT: // touche ?
			is_key_press_right = true;
			break;

		case OF_KEY_DOWN: // touche ?
			is_key_press_down = true;
			break;

		default:
			break;
		}
	}
	// Modele illumination
	if (renderer.isModeIllumination)
	{
		switch (key)
		{
		case OF_KEY_LEFT: // touche ?
			is_key_press_left = true;
			break;

		case OF_KEY_UP: // touche ?
			is_key_press_up = true;
			break;

		case OF_KEY_RIGHT: // touche ?
			is_key_press_right = true;
			break;

		case OF_KEY_DOWN: // touche ?
			is_key_press_down = true;
			break;

		default:
			break;
		}
	}
}

void Application::keyReleased(int key){
	/// Ajout boolean mode non-Illumination 
	if (!renderer.isModeIllumination && !renderer.isCoons)
	{
		if (key == 105) { // 105 = key "i"
			isImportable = !isImportable;
			renderer.interface.import_activate = !renderer.interface.import_activate;
		}
		if (key == OF_KEY_LEFT) {
			moveCameraLeft = false;
		}
		if (key == OF_KEY_RIGHT) {
			moveCameraRight = false;
		}
		if (key == OF_KEY_UP) {
			moveCameraUp = false;
		}
		if (key == OF_KEY_DOWN) {
			moveCameraDown = false;
		}
		if (key == 49) { // 49 = touche 1
			moveCameraNear = false;
		}
		if (key == 50) { // 50 = touche 2
			moveCameraFar = false;
		}
	}
	// Coons
	if (renderer.isCoons)
	{
		switch (key)
		{
		case 49: // touche 1
			renderer.selected_ctrl_point = &renderer.controlPoint0;
			ofLog() << "<select control point 0>";
			break;

		case 50: // touche 2
			renderer.selected_ctrl_point = &renderer.controlPoint1;
			ofLog() << "<select control point 1>";
			break;

		case 51: // touche 3
			renderer.selected_ctrl_point = &renderer.controlPoint2;
			ofLog() << "<select control point 2>";
			break;

		case 52: // touche 4
			renderer.selected_ctrl_point = &renderer.controlPoint3;
			ofLog() << "<select control point 3>";
			break;
		case 114: // touche r
			renderer.reset();
			break;
		case OF_KEY_LEFT: // touche ?
			is_key_press_left = false;
			break;

		case OF_KEY_UP: // touche ?
			is_key_press_up = false;
			break;

		case OF_KEY_RIGHT: // touche ?
			is_key_press_right = false;
			break;

		case OF_KEY_DOWN: // touche ?
			is_key_press_down = false;
			break;
		default:
			break;
		}
	}
	/// Modele illumination 
	if (renderer.isModeIllumination)
	{
		switch (key)
		{
		case 49: // touche 1
			renderer.shader_active = ShaderType::color_fill;
			ofLog() << "<shader: color fill>";
			break;

		case 50: // touche 2
			renderer.shader_active = ShaderType::lambert;
			ofLog() << "<shader: lambert>";
			break;

		case 51: // touche 3
			renderer.shader_active = ShaderType::gouraud;
			ofLog() << "<shader: gouraud>";
			break;

		case 52: // touche 4
			renderer.shader_active = ShaderType::phong;
			ofLog() << "<shader: phong>";
			break;

		case 53: // touche 5
			renderer.shader_active = ShaderType::blinn_phong;
			ofLog() << "<shader: blinn-phong>";
			break;

		case 114: // touche r
			renderer.reset();
			break;

		case OF_KEY_LEFT: // touche ?
			is_key_press_left = false;
			break;

		case OF_KEY_UP: // touche ?
			is_key_press_up = false;
			break;

		case OF_KEY_RIGHT: // touche ?
			is_key_press_right = false;
			break;

		case OF_KEY_DOWN: // touche ?
			is_key_press_down = false;
			break;

		default:
			break;
		}
	}

	if (key == 54) { // 50 = touche 6
		image_width = 256;
		image_height = 256;
		ray_per_pixel = 16;
		rayons.draw();
	}

}

void Application::mouseMoved(int x, int y ){
	renderer.interface.mouse_current_x = x;
	renderer.interface.mouse_current_y = y;
}

void Application::mouseDragged(int x, int y, int button){
	renderer.interface.mouse_current_x = x;
	renderer.interface.mouse_current_y = y;

	renderer.interface.mouse_drag_x = x;
	renderer.interface.mouse_drag_y = y;

	renderer.interface.is_mouse_button_dragged = true;
	renderer.interface.is_mouse_button_pressed = false;

	if (draw_line)
	{
		renderer.ligne.addVertex(renderer.interface.mouse_drag_x, renderer.interface.mouse_drag_y);
	}
	if (selectedPointIndex != -1) {
		controlPoints[selectedPointIndex].set(x, y);
	}
}

void Application::showButtonsList(){
	if(v_buttons_ptr)
	{ 
	// Parcourir la liste des boutons
		for (size_t i = 0; i < v_buttons.size(); ++i) {
			auto& button = v_buttons[i];
			auto& forme = *renderer.v_formes[i];

			// Déterminer le type de forme
			string formeType;
			switch (forme.getType()) {
			case Forme::TRIANGLE:
				formeType = "TRIANGLE";
				break;
			case Forme::CERCLE:
				formeType = "CERCLE";
				break;
			case Forme::RECTANGLE:
				formeType = "RECTANGLE";
				break;
				//case Forme::LIGNE:
				//	formeType = "LIGNE";
				//	break;
			case Forme::ELLIPSE:
				formeType = "ELLIPSE";
				break;
			case Forme::BEZIER:
				formeType = "BEZIER";
				break;
				// Ajoutez d'autres cas pour les autres types de forme
			default:
				formeType = "INCONNU";
				break;
			}
		
			// Créer l'étiquette en fonction du type et de l'index de la forme
			string label = to_string(i) + " " + formeType;

			// Ajouter le bouton avec l'étiquette dynamique
			guiScene.add(button->setup(label, false));
		}
	}
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

	for (int i = 0; i < controlPoints.size(); i++) {
		float distance = ofDist(x, y, controlPoints[i].x, controlPoints[i].y);
		if (distance < 5) {
			selectedPointIndex = i;
			break;
		}
	}

	renderer.interface.is_mouse_button_pressed = true;
	renderer.interface.is_mouse_button_dragged = false;
	renderer.interface.mouse_current_x = x;
	renderer.interface.mouse_current_y = y;
	renderer.interface.mouse_press_x = x;
	renderer.interface.mouse_press_y = y;

	// Pour les spheres et cubes, peuvent etre ameliore plus tard
	renderer.interface.mouse_press_x = x;
	renderer.interface.mouse_press_y = y;

	if(draw_triangle) {
		// A partir du mouse click, calcul des 2 autres sommets 
		newX2 = renderer.interface.mouse_press_x - diffX;
		newY2 = renderer.interface.mouse_press_y + diffY;
		newX3 = renderer.interface.mouse_press_x + diffX;
		newY3 = renderer.interface.mouse_press_y + diffY;
		forme.setX1(x);
		forme.setY1(y);
		forme.setX2(newX2);
		forme.setY2(newY2);
		forme.setX3(newX3);
		forme.setY3(newY3);
		forme.setColors(renderer.interface.color_picker_stroke, renderer.interface.colorPickerFill);
		forme.setIsFilled(renderer.interface.fillEnabled);
		forme.setOutlineWeight(renderer.interface.slider_stroke_weight);

		renderer.v_formes.push_back(make_unique<Forme>(Forme::TRIANGLE, forme.getX1(), forme.getY1(),
			forme.getX2(), forme.getY2(),
			forme.getX3(), forme.getY3(), forme.getColors(), forme.getIsFilled(), forme.getOutlineWeight()));

		renderer.okDessiner = true;
		renderer.triangleColors = { renderer.interface.color_picker_stroke, renderer.interface.colorPickerFill }; // Ajuste les parametres
		renderer.triangleFill = renderer.interface.fillEnabled;
		renderer.triangleStroke = renderer.interface.slider_stroke_weight;
		//ofxToggle button;
		auto button = make_unique<ofxToggle>();
		//guiScene.add(button->setup("TRIANGLE", shapeBool)); // Nom du bouton
		v_buttons.push_back(move(button)); // Ajoutez le bouton ï¿½ la liste des boutons
		guiScene.add(triangle);
	}

	if(draw_circle)
	{ 
		forme.setXC(renderer.interface.mouse_press_x);
		forme.setYC(renderer.interface.mouse_press_y);
		forme.setColors(renderer.interface.color_picker_stroke, renderer.interface.colorPickerFill);
		forme.setIsFilled(renderer.interface.fillEnabled);
		forme.setOutlineWeight(renderer.interface.slider_stroke_weight);
		renderer.v_formes.push_back(make_unique<Forme>(Forme::CERCLE, forme.getXC(), forme.getYC(), forme.getRayon(), 
			forme.getColors(), forme.getIsFilled(), forme.getOutlineWeight()));
		//ofSetCircleResolution(55);
		renderer.okDessiner = true; 
		renderer.cercleColors = { renderer.interface.color_picker_stroke, renderer.interface.colorPickerFill }; // Ajuste les parametres
		renderer.cercleFill = renderer.interface.fillEnabled;
		renderer.cercleStroke = renderer.interface.slider_stroke_weight;
		//ofxToggle button;
		auto button = make_unique<ofxToggle>();
		//guiScene.add(button->setup("CERCLE", false)); // Nom du bouton
		v_buttons.push_back(move(button)); // Ajoutez le bouton ï¿½ la liste des boutons

		guiScene.add(circle);
	}

	if(draw_rectangle)
	{ 
		forme.setXR(renderer.interface.mouse_current_x);
		forme.setYR(renderer.interface.mouse_current_y);
		forme.setColors(renderer.interface.color_picker_stroke, renderer.interface.colorPickerFill);
		forme.setIsFilled(renderer.interface.fillEnabled);
		forme.setOutlineWeight(renderer.interface.slider_stroke_weight);
		renderer.v_formes.push_back(make_unique<Forme>(Forme::RECTANGLE, forme.getXR(), forme.getYR(), forme.getWidth(), forme.getHeight(),
			forme.getColors(), forme.getIsFilled(), forme.getOutlineWeight()));
		renderer.okDessiner = true;
		renderer.rectangleColors = { renderer.interface.color_picker_stroke, renderer.interface.colorPickerFill }; // Ajuste les parametres
		renderer.rectangleFill = renderer.interface.fillEnabled;
		renderer.rectangleStroke = renderer.interface.slider_stroke_weight;
		//ofxToggle button;
		auto button = make_unique<ofxToggle>();
		//guiScene.add(button->setup("RECTANGLE", false)); // Nom du bouton
		v_buttons.push_back(move(button)); // Ajoutez le bouton ï¿½ la liste des boutons
		
		guiScene.add(rectangle);
	}

	if (draw_line)
	{
		//renderer.vecteur_lignes_ptr->emplace_back(make_unique<ofPolyline>());
		//auto& polyline = renderer.vecteur_lignes_ptr->back();
		//polyline->addVertex(renderer.mouse_press_x, renderer.mouse_press_y);

		renderer.ligne.addVertex(renderer.interface.mouse_press_x, renderer.interface.mouse_press_y);
		renderer.okDessiner = true;
		renderer.ligneColor = renderer.interface.color_picker_stroke; // Ajuste les parametres
		renderer.ligneStroke = renderer.interface.slider_stroke_weight;
		auto button = make_unique<ofxToggle>();
		//guiScene.add(button->setup("LIGNE", false)); // Nom du bouton
		v_buttons.push_back(move(button)); // Ajoutez le bouton ï¿½ la liste des boutons
		
		guiScene.add(line);
	}

	if (draw_ellipse)
	{
		forme.setXR(renderer.interface.mouse_press_x);
		forme.setYR(renderer.interface.mouse_press_y);
		forme.setColors(renderer.interface.color_picker_stroke, renderer.interface.colorPickerFill);
		forme.setIsFilled(renderer.interface.fillEnabled);
		forme.setOutlineWeight(renderer.interface.slider_stroke_weight);
		renderer.v_formes.push_back(make_unique<Forme>(Forme::ELLIPSE, forme.getXR(), forme.getYR(), forme.getWidth(), forme.getHeight(),
			forme.getColors(), forme.getIsFilled(), forme.getOutlineWeight()));
		renderer.okDessiner = true;
		renderer.ellipseColors = { renderer.interface.color_picker_stroke, renderer.interface.colorPickerFill }; // Ajuste les parametres
		renderer.ellipseFill = renderer.interface.fillEnabled;
		renderer.ellipseStroke = renderer.interface.slider_stroke_weight;
		auto button = make_unique<ofxToggle>();
		//guiScene.add(button->setup("ELLIPSE", false)); // Nom du bouton
		v_buttons.push_back(move(button)); // Ajoutez le bouton ï¿½ la liste des boutons
		
		guiScene.add(ellipse);
	}

	if(draw_bezier)
	{
		float x1 = renderer.interface.mouse_press_x;
		float y1 = renderer.interface.mouse_press_y;
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
		forme.setColors(renderer.interface.color_picker_stroke, renderer.interface.colorPickerFill);
		forme.setIsFilled(renderer.interface.fillEnabled);
		forme.setOutlineWeight(renderer.interface.slider_stroke_weight);

		renderer.v_formes.push_back(make_unique<Forme>(Forme::BEZIER, forme.getX1(), forme.getY1(), forme.getXB1(), forme.getYB1(),
			forme.getXB2(), forme.getYB2(), forme.getX2(), forme.getY2(), forme.getColors(), forme.getIsFilled(), forme.getOutlineWeight()));
		renderer.okDessiner = true;
		renderer.bezierColors = { renderer.interface.color_picker_stroke, renderer.interface.colorPickerFill }; // Ajuste les parametres
		renderer.bezierFill = renderer.interface.fillEnabled;
		renderer.bezierStroke = renderer.interface.slider_stroke_weight;
		auto button = make_unique<ofxToggle>();
		//guiScene.add(button->setup("BEZIER", false)); // Nom du bouton
		v_buttons.push_back(move(button)); // Ajoutez le bouton ï¿½ la liste des boutons
		
		guiScene.add(bezier);
	}
  
	if (draw_sphere) //  && drawSphere
	{
		float x = renderer.interface.mouse_press_x / 1.00;
		float y = renderer.interface.mouse_press_y / 1.00;
		ofVec3f viktor(x, y, 0);
		forme.setVSphere(viktor);
		//forme.setXS(x);
		//forme.setYS(y);
		renderer.v_formes.push_back(make_unique<Forme>(Forme::SPHERE, forme.getVSphere(), forme.getSphereRad()));
		//renderer.v_formes.push_back(make_unique<Forme>(Forme::SPHERE, forme.getXS(), forme.getYS(), 0, forme.getSphereRad()));
		renderer.okDessiner = true;
		auto button = make_unique<ofxToggle>();
		//guiScene.add(button->setup("SPHERE", false)); 
		v_buttons.push_back(move(button));

		guiScene.add(sphere);
	}

	if (draw_cube) //  && drawCube
	{
		float x = renderer.interface.mouse_press_x;
		float y = renderer.interface.mouse_press_y;
		ofVec3f viktor(x, y, 0);
		forme.setVSphere(viktor);
		forme.setSizeCube(150);
		renderer.v_formes.push_back(make_unique<Forme>(Forme::CUBE, forme.getVSphere(), forme.getSizeCube()));
		renderer.okDessiner = true;
		auto button = make_unique<ofxToggle>();
		//guiScene.add(button->setup("CUBE", false));
		v_buttons.push_back(move(button));

		guiScene.add(cube);
	}

	if (renderer.interface.delaunay) {
		renderer.delaunayPoints.push_back({ (float)renderer.interface.mouse_press_x, (float)renderer.interface.mouse_press_y});
		renderer.calculateDelaunay();
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

	selectedPointIndex = -1;

	if (button == 0 && y < INTERACTION_BAR_HEIGHT) {
		int index = static_cast<int>(floor(x / (iconWidth)));
		switch (index) {
			case 0:
				//call to import image method
				isImportable = !isImportable;
				renderer.interface.import_activate = !renderer.interface.import_activate;
				break;
			case 1:
				//call to export method
				renderer.toggleExportGUI();
				break;
			case 2:
				//call to animation method
				cout << "animation \n";
				break;
			case 3:
				//call to mesh
				cout << "mesh \n";
				renderer.interface.toggleMailleGUI();
				break;
			case 4:
				renderer.interface.toggleModelOptions();
				cout << "modele \n";
				break;
			case 5:
				renderer.interface.toggleCamOptions();
				break;
			case 6:
				renderer.interface.toggleCurveOptions();
				renderer.interface.curve_activate = !renderer.interface.curve_activate;
				break;
			case 7:
				renderer.interface.toggleLightOptions();
				break;
		}
	}

	if (button == 0 && x > WIDTH - INTERACTION_BAR_HEIGHT) {
		switch (static_cast<int>(floor((y / iconWidth)) - 1)) {
		case 0:
			//call to color wheel
			renderer.toggleColorWheelGUI();
			break;
		case 1:
			//call to pen method
			toggleDrawingGUI(forme.LIGNE);
			drawLine();
			break;
		case 2:
			//call to ellipse method
			toggleDrawingGUI(forme.CERCLE);
			drawCircle();
			break;
		case 3:
			//call to rectangle method
			toggleDrawingGUI(forme.RECTANGLE);
			drawRectangle();
			break;
		case 4:
			//call to triangle method
			toggleDrawingGUI(forme.TRIANGLE);
			drawTriangle();
			break;
		case 5:
			//call to ellipse method
			toggleDrawingGUI(forme.ELLIPSE);
			drawEllipse();
			break;
		}
	}

	renderer.interface.mouse_current_x = x;
	renderer.interface.mouse_current_y = y;


	renderer.mouse_current_x = x;
	renderer.mouse_current_y = y;


	if (draw_line)

	{
		renderer.interface.is_mouse_button_dragged = true;
		renderer.ligne.addVertex(x, y);
		renderer.vecteur_lignes.push_back(renderer.ligne);
		renderer.okDessiner = true;
		renderer.ligne.clear();
	}

	renderer.interface.is_mouse_button_pressed = false;
	renderer.interface.is_mouse_button_dragged = false;
}


void Application::mouseEntered(int x, int y){
	renderer.interface.mouse_current_x = x;
	renderer.interface.mouse_current_y = y;
}

void Application::mouseExited(int x, int y){

	renderer.interface.mouse_current_x = x;
	renderer.interface.mouse_current_y = y;
}

void Application::windowResized(int w, int h){
	WIDTH = w;
	HEIGHT = h;
}

void Application::gotMessage(ofMessage msg){
	//ofEasyCam cam;

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

void Application::button_triangle(bool& value) 
{ 
	if (value)
	{
		draw_triangle = !draw_triangle;
		draw_circle = draw_rectangle = draw_line = draw_ellipse = draw_bezier = draw_sphere = draw_cube = false;
		draw_sphere = draw_cube = false;
		//drawCircle = drawRectangle = drawLine = drawEllipse = drawBezier = drawSphere = drawCube = false;
		if (!renderer.triangleColors.empty())
		{ // Conserve les parametres de la forme pour la reselection
			renderer.interface.colorPickerFill = renderer.triangleColors[1];
			renderer.interface.color_picker_stroke = renderer.triangleColors[0];
			if (renderer.interface.fillButton != renderer.triangleFill)
			{
				renderer.interface.fillButton = renderer.triangleFill;
			}
			renderer.interface.slider_stroke_weight = renderer.triangleStroke;
		}
	}
}

void Application::drawTriangle() 
{
	cout << "drawTriangle 2 \n";
	draw_triangle = !draw_triangle;
	draw_circle = draw_rectangle = draw_line = draw_ellipse = draw_bezier = false;
	draw_sphere = draw_cube = false;
	if (!renderer.triangleColors.empty()) 
	{ // Conserve les parametres de la forme pour la reselection
		renderer.interface.colorPickerFill = renderer.triangleColors[1];
		renderer.interface.color_picker_stroke = renderer.triangleColors[0];
		if (renderer.interface.fillButton != renderer.triangleFill) 
		{
			renderer.interface.fillButton = renderer.triangleFill;

		}
		renderer.interface.slider_stroke_weight = renderer.triangleStroke;
	}
}

void Application::button_circle(bool& value)
{
	if (value)
	{
		draw_circle = !draw_circle;
		draw_triangle = draw_rectangle = draw_line = draw_ellipse = draw_bezier = draw_sphere = draw_cube = false;
		draw_sphere = draw_cube = false;
		//drawTriangle = drawRectangle = drawLine = drawEllipse = drawBezier = drawSphere = drawCube = false;
		if (!renderer.cercleColors.empty())
		{// Conserve les parametres de la forme pour la reselection
			renderer.interface.colorPickerFill = renderer.cercleColors[1];
			renderer.interface.color_picker_stroke = renderer.cercleColors[0];
			if (renderer.interface.fillButton != renderer.cercleFill) {
				renderer.interface.fillButton = renderer.cercleFill;
			}
			renderer.interface.slider_stroke_weight = renderer.cercleStroke;
		}
	}
}

void Application::drawCircle() 
{
	draw_circle = !draw_circle;
	draw_triangle = draw_rectangle = draw_line = draw_ellipse = draw_bezier = false;
	draw_sphere = draw_cube = false;
	if (!renderer.cercleColors.empty()) 
	{// Conserve les parametres de la forme pour la reselection
		renderer.interface.colorPickerFill = renderer.cercleColors[1];
		renderer.interface.color_picker_stroke = renderer.cercleColors[0];
		if (renderer.interface.fillButton != renderer.cercleFill) 
		{
			renderer.interface.fillButton = renderer.cercleFill;

		}
		renderer.interface.slider_stroke_weight = renderer.cercleStroke;
	}
}


void Application::button_rectangle(bool& value)
{
	if (value)
	{
		draw_rectangle = !draw_rectangle;
		draw_triangle = draw_circle = draw_line = draw_ellipse = draw_bezier = draw_sphere = draw_cube = false;
		draw_sphere = draw_cube = false;
		//drawTriangle = drawCircle = drawLine = drawEllipse = drawBezier = drawSphere = drawCube = false;
		if (!renderer.rectangleColors.empty())
		{ // Conserve les parametres de la forme pour la reselection
			renderer.interface.colorPickerFill = renderer.rectangleColors[1];
			renderer.interface.color_picker_stroke = renderer.rectangleColors[0];
			if (renderer.interface.fillButton != renderer.rectangleFill) {
				renderer.interface.fillButton = renderer.rectangleFill;
			}
			renderer.interface.slider_stroke_weight = renderer.rectangleStroke;
		}
	}
}

void Application::drawRectangle() 
{
	draw_rectangle = !draw_rectangle;
	draw_triangle = draw_circle = draw_line = draw_ellipse = draw_bezier = false;
	draw_sphere = draw_cube = false;
	if (!renderer.rectangleColors.empty()) 
	{ // Conserve les parametres de la forme pour la reselection
		renderer.interface.colorPickerFill = renderer.rectangleColors[1];
		renderer.interface.color_picker_stroke = renderer.rectangleColors[0];
		if (renderer.interface.fillButton != renderer.rectangleFill) 
		{
			renderer.interface.fillButton = renderer.rectangleFill;
		}
		renderer.interface.slider_stroke_weight = renderer.rectangleStroke;
	}
}


void Application::button_line(bool& value) 
{
	if (value) 
	{
		draw_line = !draw_line;
		draw_triangle = draw_rectangle = draw_circle = draw_ellipse = draw_bezier = draw_sphere = draw_cube = false;
		draw_sphere = draw_cube = false;
		//drawTriangle = drawRectangle = drawCircle = drawEllipse = drawBezier = drawSphere = drawCube = false;
		renderer.interface.color_picker_stroke = renderer.ligneColor;
		renderer.interface.slider_stroke_weight = renderer.ligneStroke;
	}
}

void Application::button_ellipse(bool& value)
{
	if (value) 
	{
		draw_ellipse = !draw_ellipse;
		draw_triangle = draw_rectangle = draw_circle = draw_line = draw_bezier = draw_sphere = draw_cube = false;
		draw_sphere = draw_cube = false;
		//drawTriangle = drawRectangle = drawCircle = drawLine = drawBezier = drawSphere = drawCube = false;
		if (!renderer.ellipseColors.empty()) 
		{ // Conserve les parametres de la forme pour la reselection
			renderer.interface.colorPickerFill = renderer.ellipseColors[1];
			renderer.interface.color_picker_stroke = renderer.ellipseColors[0];
			if (renderer.interface.fillButton != renderer.ellipseFill) {
				renderer.interface.fillButton = renderer.ellipseFill;
			}
			renderer.interface.slider_stroke_weight = renderer.ellipseStroke;
		}
	}
}

void Application::drawLine() {
	draw_line = !draw_line;
	draw_triangle = draw_rectangle = draw_circle = draw_ellipse = draw_bezier = false;
	draw_sphere = draw_cube = false;
	renderer.interface.color_picker_stroke = renderer.ligneColor;
	renderer.interface.slider_stroke_weight = renderer.ligneStroke;
}

void Application::drawEllipse() 
{
	draw_ellipse = !draw_ellipse;
	draw_triangle = draw_rectangle = draw_circle = draw_line = draw_bezier = false;
	draw_sphere = draw_cube = false;
	if (!renderer.ellipseColors.empty()) 
	{ // Conserve les parametres de la forme pour la reselection
		renderer.interface.colorPickerFill = renderer.ellipseColors[1];
		renderer.interface.color_picker_stroke = renderer.ellipseColors[0];
		if (renderer.interface.fillButton != renderer.ellipseFill) 
		{
			renderer.interface.fillButton = renderer.ellipseFill;
		}
		renderer.interface.slider_stroke_weight = renderer.ellipseStroke;
	}
}

void Application::button_bezier(bool& value)
{
	if (value)
	{
		draw_bezier = !draw_bezier;
		draw_triangle = draw_rectangle = draw_circle = draw_line = draw_ellipse = draw_cube = false;
		draw_sphere = draw_cube = false;
		//drawTriangle = drawRectangle = drawCircle = drawLine = drawEllipse = drawCube = false;
		if (!renderer.bezierColors.empty()) { // Conserve les parametres de la forme pour la reselection
			renderer.interface.colorPickerFill = renderer.bezierColors[1];
			renderer.interface.color_picker_stroke = renderer.bezierColors[0];
			if (renderer.interface.fillButton != renderer.bezierFill) {
				renderer.interface.fillButton = renderer.bezierFill;
			}
			renderer.interface.slider_stroke_weight = renderer.bezierStroke;
		}
	}
}

void Application::drawBezier() {
	draw_bezier = !draw_bezier;
	draw_triangle = draw_rectangle = draw_circle = draw_line = draw_ellipse = false;
	draw_sphere = draw_cube = false;
	if (!renderer.bezierColors.empty()) { // Conserve les parametres de la forme pour la reselection
		renderer.interface.colorPickerFill = renderer.bezierColors[1];
		renderer.interface.color_picker_stroke = renderer.bezierColors[0];
		if (renderer.interface.fillButton != renderer.bezierFill) {
			renderer.interface.fillButton = renderer.bezierFill;

		}
		renderer.interface.slider_stroke_weight = renderer.bezierStroke;
	}
}

void Application::button_sphere(bool& value) {
	if (value) {
		draw_sphere = !draw_sphere; 
		draw_triangle = draw_rectangle = draw_circle = draw_line = draw_ellipse = draw_bezier = draw_cube = false;
		//drawTriangle = drawRectangle = drawCircle = drawLine = drawEllipse = drawBezier = drawCube = false;
	}
}

void Application::button_sphereTexture(bool& value) {
	if (value)
	{
		draw_sphereTexture = !draw_sphereTexture;
		draw_circle = draw_rectangle = draw_line = draw_ellipse = draw_bezier = draw_sphere = draw_cube = draw_triangle = false;
		draw_sphere = draw_cube = false;
	}
}

void Application::button_cube(bool& value) {
	if (value) {
		draw_cube = !draw_cube; 
		draw_triangle = draw_rectangle = draw_circle = draw_line = draw_ellipse = draw_bezier = draw_sphere = false;
		//drawTriangle = drawRectangle = drawCircle = drawLine = drawEllipse = drawBezier = drawSphere = false;
	}
}

void Application::reset(bool& value) {
	if (value) {
		renderer.uiPosition.set(ofVec2f(0));
		renderer.uiAmount.set(1);
		renderer.uiStep.set(ofVec2f(0));
		renderer.uiRotate.set(ofVec3f(0));
		renderer.uiShift.set(ofVec2f(0));
		renderer.uiSize.set(ofVec2f(6));

		draw_sphere = false;
		draw_cube = false;
		draw_triangle = false;
		draw_circle = false;
		draw_rectangle = false;
		resetButton = false;
		rotationButton = false;
		rotation_activate = false;
		mesh_activate = false;
		meshButton = false;
		noise_activate = false;
		meshAnimationButton = false;
		sphereTextureButton = false;
		catmullRomButton = false;
		catmullRom_activate = false;
		hermiteButton = false;
		grayButton = false;
		gray_activate = false;
		sharpen_activate = false;
		sharpenButton = false;
		embossButton = false;
		emboss_activate = false;

		// Texture 
		resetTexture();
		renderer.reset(); 
	}
}


void Application::button_modeDraw(bool& value) {
	if (value) {
		renderer.modeDrawState = !renderer.modeDrawState;
		renderer.modeTransformState = toggleTransform = false;
	}
}

void Application::button_modeTransform(bool& value) {
	if (value) {
		renderer.modeTransformState = !renderer.modeTransformState;
		renderer.modeDrawState = toggleDraw = false;
	}
}


void Application::reset_cam() {
	offset_camera = 500.0f * 3.5f * -1.0f;

	// position initiale de chaque camÃ©ra
	camFront.setPosition(0, 0, -offset_camera);
	camBack.setPosition(0, 0, offset_camera);
	camLeft.setPosition(-offset_camera, 0, 0);
	camRight.setPosition(offset_camera, 0, 0);
	camTop.setPosition(0, offset_camera, 0);
	camBottom.setPosition(0, -offset_camera, 0);

	// orientation de chaque camÃ©ra
	camFront.lookAt(camera_target);
	camBack.lookAt(camera_target);
	camLeft.lookAt(camera_target);
	camRight.lookAt(camera_target);
	camTop.lookAt(camera_target, ofVec3f(1, 0, 0));
	camBottom.lookAt(camera_target, ofVec3f(1, 0, 0));

	camFront.setVFlip(true);
	camBack.setVFlip(true);
	camLeft.setVFlip(true);
	camRight.setVFlip(true);
	camTop.setVFlip(true);
	camBottom.setVFlip(true);

	camera_active = CameraPosition::front;
}

void Application::button_rotation(bool& value) {
	rotation_activate = value;
	if (value) {
		rotation_activate = true;
	}
}

void Application::button_mesh(bool& value) {
	mesh_activate = value;
	if (value) {
		mesh_activate = true;
	}
}

void Application::button_noise(bool& value) {
	noise_activate = value;
	if (value) {
		noise_activate = true;
	}
}


void Application::camera_setup_perspective(float width, float height, float fov, float n, float f)
{
	bool camera_projection_persp_or_ortho = true;
	bool camera_vertical_flip = true;

	int camera_viewport_x = width;
	int camera_viewport_y = height;
	float camera_aspect_ratio = camera_viewport_x / camera_viewport_y;

	float camera_fov = fov;
	float camera_zoom = compute_zoom_from_fov(camera_fov);

	float minimal_side = std::min(camera_viewport_x, camera_viewport_y);
	float fov_half = ofDegToRad(camera_fov / 2.0f);
	float distance = minimal_side / 2.0f / tanf(fov_half);

	glm::vec3 camera_position;

	camera_position.x = camera_viewport_x / 2.0f;
	camera_position.y = camera_viewport_y / 2.0f;
	camera_position.z = distance;

	//camera_clip_n = near > 0.0f ? near : distance / 10.0f;
	//camera_clip_f = far > 0.0f ? far : distance * 10.0f;

	float camera_clip_n = distance / 2.0f;
	float camera_clip_f = distance * 1.5f;

	float camera_depth_range = camera_clip_f - camera_clip_n;

	// configurer l'instance de camÃ©ra de openFrameworks (ofCamera)
	cam.setupPerspective(camera_vertical_flip, camera_fov, camera_clip_n, camera_clip_f, ofVec2f(0, 0));
	cam.setPosition(camera_position.x, camera_position.y, camera_position.z);

	bool camera_state_change = false;
}

float Application::compute_zoom_from_fov(float fov)
{
	return 1.0f / tanf(glm::radians(fov) / 2.0f);;
}

void Application::setupCamera() {
	switch (camera_active) {
		case CameraPosition::front:
			camera = &camFront;
			renderer.interface.camera_name = "avant";
			break;

		case CameraPosition::back:
			camera = &camBack;
			renderer.interface.camera_name = "arriÃ¨re";
			break;

		case CameraPosition::left:
			camera = &camLeft;
			renderer.interface.camera_name = "gauche";
			break;

		case CameraPosition::right:
			camera = &camRight;
			renderer.interface.camera_name = "droite";
			break;

		case CameraPosition::top:
			camera = &camTop;
			renderer.interface.camera_name = "haut";
			break;

		case CameraPosition::down:
			camera = &camBottom;
			renderer.interface.camera_name = "bas";
			break;

		default:
			break;
		}
		camera_position = camera->getPosition();
		camera_orientation = camera->getOrientationQuat();

		camera->setPosition(camera_position);
		camera->setOrientation(camera_orientation);
}

void Application::button_catmullRom(bool& value) {
	catmullRom_activate = value;
	if (value) {
		catmullRom_activate = true;
		catmullRomButton6 = false;
		catmullRom6_activate = false;
		hermiteButton = false;
		hermite_activate = false;
		coonsButton = false;
		renderer.isCoons = false;
	}
}

void Application::button_catmullRom6(bool& value) {
	catmullRom6_activate = value;
	if (value) {
		catmullRom6_activate = true;
		catmullRomButton = false;
		catmullRom_activate = false;
		hermiteButton = false;
		hermite_activate = false;
		coonsButton = false;
		renderer.isCoons = false;
	}
}

void Application::button_hermite(bool& value) {
	hermite_activate = value;
	if (value) {
		hermite_activate = true;
		catmullRomButton6 = false;
		catmullRom6_activate = false;
		catmullRomButton = false;
		catmullRom_activate = false;
		coonsButton = false;
		renderer.isCoons = false;
	}
}

void Application::button_coons(bool& value) {
	renderer.isCoons = value;
	if (value) {
		renderer.isCoons = true;
		catmullRomButton6 = false;
		catmullRom6_activate = false;
		catmullRomButton = false;
		catmullRom_activate = false;
		hermiteButton = false;
		hermite_activate = false;
	}
}

void Application::hermite(float t, const ofVec2f& p0, const ofVec2f& p1, const ofVec2f& p2, const ofVec2f& p3, const ofVec2f& p4, float& x, float& y)
{
	float u = 1 - t;
	float uu = u * u;
	float uuu = uu * u;
	float tt = t * t;
	float ttt = tt * t;

	x = (2 * ttt - 3 * tt + 1) * p1.x + (ttt - 2 * tt + t) * p2.x + (ttt - tt) * p3.x + (-2 * ttt + 3 * tt) * p4.x;
	y = (2 * ttt - 3 * tt + 1) * p1.y + (ttt - 2 * tt + t) * p2.y + (ttt - tt) * p3.y + (-2 * ttt + 3 * tt) * p4.y;
}

ofVec2f Application::catmullRom(float t, const ofVec2f& p0, const ofVec2f& p1, const ofVec2f& p2, const ofVec2f& p3, const ofVec2f& p4) {
	float t2 = t * t;
	float t3 = t2 * t;

	float b0 = 0.5f * (-t3 + 2 * t2 - t);
	float b1 = 0.5f * (3 * t3 - 5 * t2 + 2);
	float b2 = 0.5f * (-3 * t3 + 4 * t2 + t);
	float b3 = 0.5f * (t3 - t2);

	return p0 * b0 + p1 * b1 + p2 * b2 + p3 * b3 + p4 * b3;
}

ofVec2f Application::catmullRom6(float t, const ofVec2f& p0, const ofVec2f& p1, const ofVec2f& p2, const ofVec2f& p3, const ofVec2f& p4, const ofVec2f& p5) {
	float t2 = t * t;
	float t3 = t2 * t;

	float b0 = 0.5f * (-t3 + 2 * t2 - t);
	float b1 = 0.5f * (3 * t3 - 5 * t2 + 2);
	float b2 = 0.5f * (-3 * t3 + 4 * t2 + t);
	float b3 = 0.5f * (t3 - t2);
  
	return p0 * b0 + p1 * b1 + p2 * b2 + p3 * b3 + p4 * b3 + p5 * b3;
}

void Application::button_blackAndWhite(bool& value) {
    gray_activate = value;
	if (value) {
		for (ofImage& img : renderer.imageList) {
			if (!img.isAllocated()) {
				continue;
			}
			if (!originalImagePixels[&img].initialized) {
				originalImagePixels[&img].pixels = img.getPixels();
				originalImagePixels[&img].initialized = true;
			}
		    img.setImageType(OF_IMAGE_GRAYSCALE);
		}
	}
	else {
		for (ofImage& img : renderer.imageList) {
			if (!img.isAllocated()) {
				continue;
			}
			if (originalImagePixels[&img].initialized) {
				img.setFromPixels(originalImagePixels[&img].pixels);
				img.update();
			}
		}
	}
	
}
void Application::button_sharpen(bool& value) {
	sharpen_activate = value;

	if (value) {
		for (ofImage& img : renderer.imageList) {
			if (!img.isAllocated()) {
				continue;
			}

			if (!originalImagePixels[&img].initialized) {
				originalImagePixels[&img].pixels = img.getPixels();
				originalImagePixels[&img].initialized = true;
			}
			ofPixels sharpenedPixels = originalImagePixels[&img].pixels;
			int w = img.getWidth();
			int h = img.getHeight();

			float kernel[3][3] = {
				{-1, -1, -1},
				{-1, 9, -1},
				{-1, -1, -1}
			};

			for (int y = 1; y < h - 1; y++) {
				for (int x = 1; x < w - 1; x++) {
					float sumR = 0, sumG = 0, sumB = 0;
					for (int ky = -1; ky <= 1; ky++) {
						for (int kx = -1; kx <= 1; kx++) {
							int pixelX = x + kx;
							int pixelY = y + ky;
							ofColor color = originalImagePixels[&img].pixels.getColor(pixelX, pixelY);
							sumR += color.r * kernel[ky + 1][kx + 1];
							sumG += color.g * kernel[ky + 1][kx + 1];
							sumB += color.b * kernel[ky + 1][kx + 1];
						}
					}
					sumR = ofClamp(sumR, 0, 255);
					sumG = ofClamp(sumG, 0, 255);
					sumB = ofClamp(sumB, 0, 255);
					sharpenedPixels.setColor(x, y, ofColor(sumR, sumG, sumB));
				}
			}

			img.setFromPixels(sharpenedPixels);
			img.update();
		}
	}
	else {
		for (ofImage& img : renderer.imageList) {
			if (!img.isAllocated()) {
				continue;
			}
			if (originalImagePixels[&img].initialized) {
				img.setFromPixels(originalImagePixels[&img].pixels);
				img.update();
			}
		}
	}
}

void Application::button_emboss(bool& value) {
	emboss_activate = value;

	if (value) {
		for (ofImage& img : renderer.imageList) {
			if (!img.isAllocated()) {
				continue;
			}
			if (!originalImagePixels[&img].initialized) {
				originalImagePixels[&img].pixels = img.getPixels();
				originalImagePixels[&img].initialized = true;
			}
			for (int y = 0; y < img.getHeight(); y++) {
				for (int x = 0; x < img.getWidth(); x++) {
					int pixelBrightness = originalImagePixels[&img].pixels.getColor(x, y).getBrightness();
					int neighborBrightness = originalImagePixels[&img].pixels.getColor(ofClamp(x + 1, 0, img.getWidth() - 1), ofClamp(y + 1, 0, img.getHeight() - 1)).getBrightness();
					int diff = neighborBrightness - pixelBrightness;
					int grayValue = ofClamp(128 + diff, 0, 255);
					img.setColor(x, y, ofColor(grayValue));
				}
			}
			img.update();
		}
	}
	else {
		for (ofImage& img : renderer.imageList) {
			if (!img.isAllocated()) {
				continue;
			}
			if (originalImagePixels[&img].initialized) {
				img.setFromPixels(originalImagePixels[&img].pixels);
				img.update();
			}
		}
	}
}
