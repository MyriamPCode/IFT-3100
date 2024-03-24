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
	ofDisableArbTex();

	tint.set(255, 255, 255);
	mix_factor = 0.618f;
	shader.load("image_filter_330_vs.glsl", "image_filter_330_fs.glsl");
	
	gui.setup("Exportation");
	nameField.set("Image name", "name");
	gui.add(nameField);
	exportButton.setName("Export");
	gui.add(exportButton);
	gui.setPosition(200, 40);

	textureImage.load("img/teapot.jpg");

	setTeapotMaterials();
	setSphereMaterials();
	setCubeMaterials();

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

	mesh.setMode(OF_PRIMITIVE_LINE_STRIP);

	// Création de la maille
	for (int x = 0; x < size; x++) {
		for (int y = 0; y < size; y++) {
			mesh.addVertex(ofPoint(1000 - (size / 10) * x, 1000 - (size / 10) * y));
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
	//////////////////////////////////////
	// Parametres pour modele illumination 
	isModeIllumination = true;  /// mettre a false pour desactiver au lancement 
	oscillation_amplitude = 32.0f;
	oscillation_frequency = 7500.0f;
	speed_motion = 150.0f;
	initial_x = 0.0f;
	initial_z = -100.0f;
	scale_cube = 100.0f;
	scale_sphere = 80.0f;
	scale_modele_ill_1 = 0.618f;
	scale_modele_ill_2 = 0.618f;
	offset_x = initial_x;
	offset_z = initial_z;
	delta_x = speed_motion;
	delta_z = speed_motion;
	modele_illumination1.loadModel("models/teapot.obj");
	modele_illumination2.loadModel("models/pomu.obj");
	//modele_illumination1.loadModel("teapot.obj");
	//modele_illumination2.loadModel("pomu.obj");
	modele_illumination1.disableMaterials();
	modele_illumination2.disableMaterials();
	shader_color_fill.load(
		"shader/color_fill_330_vs.glsl",
		"shader/color_fill_330_fs.glsl");

	shader_lambert.load(
		"shader/lambert_330_vs.glsl",
		"shader/lambert_330_fs.glsl");

	shader_gouraud.load(
		"shader/gouraud_330_vs.glsl",
		"shader/gouraud_330_fs.glsl");

	shader_phong.load(
		"shader/phong_330_vs.glsl",
		"shader/phong_330_fs.glsl");

	shader_blinn_phong.load(
		"shader/blinn_phong_330_vs.glsl",
		"shader/blinn_phong_330_fs.glsl");
	// shader actif au lancement de la scène
	shader_active = ShaderType::blinn_phong;
	//////////////////////////////////////
	// initialisation de la scène
	reset();

	ofDisableArbTex();
	ofLoadImage(textu, "img/teapot.jpg");
}

// a ete ajoute avec le mode Illumination 
void Renderer::reset()
{
	/// Modele illumination
	// centre du framebuffer
	center_x = ofGetWidth() / 2.0f;
	center_y = ofGetHeight() / 2.0f;

	// caméra à sa position initiale
	offset_x = initial_x;
	offset_z = initial_z;

	// déterminer la position des géométries
	//position_cube.set(-ofGetWidth() * (1.0f / 4.0f), 0.0f, 0.0f);
	position_modele_ill_2.set(-ofGetWidth() * (1.0f / 4.0f), 0.0f, 0.0f);
	position_sphere.set(0.0f, 0.0f, 0.0f);
	position_modele_ill_1.set(ofGetWidth() * (1.0f / 4.0f), 50.0f, 0.0f);
	//////////////////////////////////////

	ofLog() << "<reset>";
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

	shader.begin();
	shader.setUniformTexture("image", image.getTexture(), 1);
	shader.setUniform3f("tint", tint.r / 255.0f, tint.g / 255.0f, tint.b / 255.0f);
	shader.setUniform1f("factor", mix_factor);
	ofPushMatrix();
	ofTranslate(image.getWidth(), image.getHeight());
	ofSetColor(tint); // Définir la couleur de dessin avec la teinte sélectionnée
	image.draw(0, 0, image.getWidth(), image.getHeight());
	ofPopMatrix();
	shader.end();

	auto currImg = imgPosList.begin();
	for (list<ofImage>::iterator iter = imageList.begin(); iter != imageList.end(); ++iter) {
		vector temp = *currImg;
		iter->draw(temp[0], temp[1]);
		++currImg;

	}
	// activer le filtre
	//shader.begin();

	// passer les attributs uniformes au shader

	//ofTexture textu = textureImage.getTexture();
	

	/*glActiveTexture(GL_TEXTURE_2D);
	GLuint texture_id;
	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	int image_size = textureImage.getWidth() * textureImage.getHeight() * 10;
	GLubyte* pixels = (GLubyte*)std::malloc(image_size * sizeof(GLubyte));
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureImage.getWidth(), textureImage.getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	mesh.enableTextures();*/

	if (interface.mesh_activate) {
		//mesh.drawWireframe();
		textu.generateMipmap();
		textu.bind();
		mesh.draw();
		textu.unbind();
	}

	ofSpherePrimitive sphere;
	sphere.setPosition(0, 0, 50);
	sphere.setRadius(200);
	textu.generateMipmap();
	textu.bind();
	sphere.draw();
	textu.unbind();

	// désactiver le filtre
	//shader.end();
	

	//////////////////////////////////////////////////////////////////

	if (okDessiner)
	{
		if (interface.textureFillButton) {
			shader.load("filters/colors.vert", "filters/colors.frag");
			shader.begin();
			shader.setUniform1f("u_time", ofGetElapsedTimef());
			shader.setUniform2f("u_resolution", ofGetWidth(), ofGetHeight());
			ofRect(0, 0, ofGetWidth(), ofGetHeight());
		}

		// draw Illumination 
		if (isModeIllumination) {
			ofEnableLighting();
			light.enable();
			ofPushMatrix;
			shader_illumination->begin();
			dessinerSphere();
			ofPopMatrix();
			shader_illumination->end();
			light.disable();
			ofDisableLighting();
		}

		dessinerTriangle();
		dessinerCercle();
		dessinerRectangle();
		dessinerEllipse();
		//dessinerSphere(); //en commentaire sinon ca pogne en conflit avec Illumination
		dessinerCube();

		if (interface.textureFillButton) {
			shader.end();
		}

		dessinerLigne();
		dessinerBezier();
	}

	//////////////////////////////////////////////////////////////////
	if (interface.getShowModel()) {

		if (interface.showTeapotMaterials) {
			// désactiver le matériau par défaut du modèle
			teapotMultiple.disableMaterials();
			teapotOrtho.disableMaterials();

			setTeapotMaterials();
			// activer le matériau
			material_teapot.begin();
		}

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

		if (interface.showTeapotMaterials) {
			// désactiver le matériau
			material_teapot.end();

			// activer le matériau par défaut 
			teapotMultiple.enableMaterials();
			teapotOrtho.enableMaterials();

		}
	}

	// Afficher un message si l'enregistrement est activé
	if (isRecording) {
		ofDrawBitmapString("Enregistrement enmouse cours...", 20, 20);
	}

	/// Modele illumination
	// Load les 2 modeles 3D et un sphere au milieu
	//activer_Illumination(); // Le commenter pour ne pas charger au lancement 
}

void Renderer::setTeapotMaterials() {
	// configurer le matériau du teapot
	material_teapot.setAmbientColor(ofColor(interface.teapotAmbientColorPicker));
	material_teapot.setDiffuseColor(ofColor(interface.teapotDiffuseColorPicker));
	material_teapot.setEmissiveColor(ofColor(interface.teapotEmissiveColorPicker));
	material_teapot.setSpecularColor(ofColor(interface.teapotSpecularColorPicker));
	material_teapot.setShininess(interface.teapotShininess);
}

// Nouvelle fonction pour Illumination
void Renderer::activer_Illumination() {
	// activer l'éclairage dynamique
	ofEnableLighting();

	// activer la lumière dynamique
	light.enable();

	ofPushMatrix();

	// transformer l'origine de la scène au milieu de la fenêtre d'affichage
	ofTranslate(center_x + offset_x, center_y, offset_z);

	ofPushMatrix();

	//// positionnner le cube
	//ofTranslate(
	//	position_cube.x,
	//	position_cube.y,
	//	position_cube.z);
	//// rotation locale
	//ofRotateDeg(45.0f, 1.0f, 0.0f, 0.0f);
	// 
	// positionner pomudachi
	modele_illumination2.setPosition(
		position_modele_ill_2.x,
		position_modele_ill_2.y + 15.0f,
		position_modele_ill_2.z);

	// dimension pomudachi
	modele_illumination2.setScale(
		scale_modele_ill_2,
		scale_modele_ill_2,
		scale_modele_ill_2);

	// activer le shader
	shader_illumination->begin();

	// dessiner un cube
	//ofDrawBox(0.0f, 0.0f, 0.0f, scale_cube);
	// dessiner pomudachi 
	modele_illumination2.draw(OF_MESH_FILL);

	ofPopMatrix();

	ofPushMatrix();

	// positionner la sphère
	ofTranslate(
		position_sphere.x,
		position_sphere.y,
		position_sphere.z);

	// dessiner une sphère
	ofDrawSphere(0.0f, 0.0f, 0.0f, scale_sphere);

	ofPopMatrix();

	ofPushMatrix();

	// positionner le teapot
	modele_illumination1.setPosition(
		position_modele_ill_1.x,
		position_modele_ill_1.y + 15.0f,
		position_modele_ill_1.z);

	// dimension du teapot
	modele_illumination1.setScale(
		scale_modele_ill_1,
		scale_modele_ill_1,
		scale_modele_ill_1);

	// dessiner un teapot 
	modele_illumination1.draw(OF_MESH_FILL);


	ofPopMatrix();

	ofPopMatrix();

	// désactiver le shader
	shader_illumination->end();

	// désactiver la lumière
	light.disable();

	// désactiver l'éclairage dynamique
	ofDisableLighting();
}
// fonction d'oscillation pour Illumination 
float Renderer::oscillate(float time, float frequency, float amplitude)
{
	return sinf(time * 2.0f * PI / frequency) * amplitude;
}

void Renderer::dessinerSphere(){
	if (interface.showSphereMaterials) {
		setSphereMaterials();
		material_sphere.begin();
	}

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

	if (interface.showSphereMaterials) {
		material_sphere.end();
	}
}

void Renderer::setSphereMaterials() {
	// configurer le matériau du teapot
	material_sphere.setAmbientColor(ofColor(interface.sphereAmbientColorPicker));
	material_sphere.setDiffuseColor(ofColor(interface.sphereDiffuseColorPicker));
	material_sphere.setEmissiveColor(ofColor(interface.sphereEmissiveColorPicker));
	material_sphere.setSpecularColor(ofColor(interface.sphereSpecularColorPicker));
	material_sphere.setShininess(interface.sphereShininess);
}

void Renderer::dessinerCube() {
	if (interface.showCubeMaterials) {
		setCubeMaterials();
		material_cube.begin();
	}

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

	if (interface.showCubeMaterials) {
		material_cube.end();
	}
}

void Renderer::setCubeMaterials() {
	// configurer le matériau du teapot
	material_cube.setAmbientColor(ofColor(interface.cubeAmbientColorPicker));
	material_cube.setDiffuseColor(ofColor(interface.cubeDiffuseColorPicker));
	material_cube.setEmissiveColor(ofColor(interface.cubeEmissiveColorPicker));
	material_cube.setSpecularColor(ofColor(interface.cubeSpecularColorPicker));
	material_cube.setShininess(interface.cubeShininess);
}

void Renderer::dessinerTriangle() {
	if (v_formes_ptr) {
		for (int i = 0; i < v_formes_ptr->size(); ++i) {
			const auto& formeCourante = (*v_formes_ptr)[i];
			if (formeCourante->getType() == Forme::TRIANGLE) {
				//if (triangleFill) {
					//ofFill();
					ofSetLineWidth(formeCourante->getOutlineWeight());
					ofSetColor(formeCourante->getColors()[1]);
					// Dessin du triangle avec translation si l'index correspond et le mode de transformation est activé
					if (i == inputIndex && modeTransformState) {
						ofPushMatrix();
						ofTranslate(uiPosition->x, uiPosition->y); // positionnement 
						ofTranslate(uiAmount * uiStep->x, uiAmount * uiStep->y);
	
						ofRotateXDeg(uiRotate->x); // pivoter sur x 
						ofRotateYDeg(uiRotate->y); // pivoter sur y 
						ofRotateZDeg(uiRotate->z); // pivoter sur z 
						ofScale(uiSize->x, uiSize->y);
							if (formeCourante->getIsFilled()) { // Remplissage
								ofFill();
								ofSetColor(formeCourante->getColors()[1]);
								ofBeginShape();
								ofDrawTriangle(formeCourante->getX1(), formeCourante->getY1(),
									formeCourante->getX2(), formeCourante->getY2(),
									formeCourante->getX3(), formeCourante->getY3());
								ofEndShape();
							}
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
						if (formeCourante->getIsFilled()) { // Remplissage
							ofFill();
							ofSetColor(formeCourante->getColors()[1]);
							ofDrawTriangle(formeCourante->getX1(), formeCourante->getY1(),
								formeCourante->getX2(), formeCourante->getY2(),
								formeCourante->getX3(), formeCourante->getY3());
						}
						// Dessin du triangle sans translation
						ofNoFill();
						ofSetLineWidth(formeCourante->getOutlineWeight());
						ofSetColor(formeCourante->getColors()[0]); // Couleur de contour normale
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
					ofSetLineWidth(formeCourante->getOutlineWeight());
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
						if (formeCourante->getIsFilled()) { // Remplissage
							ofFill();
							ofSetColor(formeCourante->getColors()[1]);
							ofBeginShape();
							ofDrawCircle(formeCourante->getXC(), formeCourante->getYC(), formeCourante->getRayon());
							ofEndShape();
						}
						ofNoFill();
						ofBeginShape();
						ofSetColor(255, 0, 0); // Rouge
						ofDrawCircle(formeCourante->getXC(), formeCourante->getYC(), formeCourante->getRayon());
						ofEndShape();
						ofPopMatrix();
					}
					else {
						// Dessiner le rectangle sans transformation
						if (formeCourante->getIsFilled()) { // Remplissage
							ofFill();
							ofSetColor(formeCourante->getColors()[1]);
							ofDrawCircle(formeCourante->getXC(), formeCourante->getYC(), formeCourante->getRayon());
						}
						ofNoFill(); // Outline
						ofSetLineWidth(formeCourante->getOutlineWeight());
						ofSetColor(formeCourante->getColors()[0]);
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
					ofSetLineWidth(formeCourante->getOutlineWeight());
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
						if (formeCourante->getIsFilled()) { // Remplissage
						/*	if (interface.textureFillButton) {
								shader.begin();

								shader.setUniform2f("u_resolution", formeCourante->getWidth(), formeCourante->getHeight());
							}*/

							ofFill();
							ofSetColor(formeCourante->getColors()[1]);
							ofBeginShape();
							ofDrawRectangle(formeCourante->getXR(), formeCourante->getYR(),
								formeCourante->getWidth(), formeCourante->getHeight());

							ofEndShape();
							//shader.end();
						}
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
						if (formeCourante->getIsFilled()) { // Remplissage
							ofFill();
							ofSetColor(formeCourante->getColors()[1]);
							ofDrawRectangle(formeCourante->getXR(), formeCourante->getYR(),
								formeCourante->getWidth(), formeCourante->getHeight());
						}
						ofNoFill(); // Outline
						ofSetLineWidth(formeCourante->getOutlineWeight());
						ofSetColor(formeCourante->getColors()[0]);
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
					ofSetLineWidth(formeCourante->getOutlineWeight());
					ofSetColor(ellipseColors[1]);
					// Dessiner l'ellipse avec transformation si l'index correspond et le mode de transformation est activé
					if (i == inputIndex && modeTransformState) {
						ofPushMatrix();
						ofScale(uiSize->x, uiSize->y);
						ofTranslate(uiPosition->x, uiPosition->y); // translation 
						ofRotateXDeg(uiRotate->x); // pivoter sur x 
						ofRotateYDeg(uiRotate->y); // pivoter sur y 
						ofRotateZDeg(uiRotate->z); // pivoter sur z
						if (formeCourante->getIsFilled()) { // Remplissage
							ofFill();
							ofSetColor(formeCourante->getColors()[1]);
							ofBeginShape();
							ofDrawEllipse(formeCourante->getXR(), formeCourante->getYR(),
								formeCourante->getWidth(), formeCourante->getHeight());
							ofEndShape();
						}
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
						if (formeCourante->getIsFilled()) { // Remplissage
							ofFill();
							ofSetColor(formeCourante->getColors()[1]);
							ofDrawEllipse(formeCourante->getXR(), formeCourante->getYR(),
								formeCourante->getWidth(), formeCourante->getHeight());
						}
						ofNoFill(); // Outline
						ofSetLineWidth(formeCourante->getOutlineWeight());
						ofSetColor(formeCourante->getColors()[0]);
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
					ofSetLineWidth(formeCourante->getOutlineWeight());
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
						if (formeCourante->getIsFilled()) { // Remplissage
							ofFill();
							ofSetColor(formeCourante->getColors()[1]);
							ofBeginShape();
							ofDrawBezier(formeCourante->getX1(), formeCourante->getX2(), formeCourante->getXB1(), formeCourante->getYB1(),
								formeCourante->getXB2(), formeCourante->getYB2(), formeCourante->getX2(), formeCourante->getY2());
							ofEndShape();
						}
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
						if (formeCourante->getIsFilled()) { // Remplissage
							ofFill();
							ofSetColor(formeCourante->getColors()[1]);
							ofDrawBezier(formeCourante->getX1(), formeCourante->getX2(), formeCourante->getXB1(), formeCourante->getYB1(),
								formeCourante->getXB2(), formeCourante->getYB2(), formeCourante->getX2(), formeCourante->getY2());
						}
						ofNoFill(); // Outline
						ofSetLineWidth(formeCourante->getOutlineWeight());
						ofSetColor(formeCourante->getColors()[0]);
						ofDrawBezier(formeCourante->getX1(), formeCourante->getX2(), formeCourante->getXB1(), formeCourante->getYB1(),
							formeCourante->getXB2(), formeCourante->getYB2(), formeCourante->getX2(), formeCourante->getY2());
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

	if (interface.noise_activate) {
		int count = 0;
		for (int x = 0; x < size; x++) {
			for (int y = 0; y < size; y++) {
				ofVec3f vertex = mesh.getVertex(count);
				vertex.z = ofMap(ofNoise(count, ofGetElapsedTimef()), 0, 1, 0, 30);
				mesh.setVertex(count, vertex);
				count++;
			}
		}
	}

	/// Modele illumination 
	// transformer la lumière
	light.setGlobalPosition(
		ofMap(ofGetMouseX() / (float)ofGetWidth(), 0.0f, 1.0f, -ofGetWidth() / 2.0f, ofGetWidth() / 2.0f),
		ofMap(ofGetMouseY() / (float)ofGetHeight(), 0.0f, 1.0f, -ofGetHeight() / 2.0f, ofGetHeight() / 2.0f),
		-offset_z * 1.5f);
	// mise à jour d'une valeur numérique animée par un oscillateur
	float oscillation = oscillate(ofGetElapsedTimeMillis(), oscillation_frequency, oscillation_amplitude) + oscillation_amplitude;
	// passer les attributs uniformes au shader de sommets
	switch (shader_active)
	{
	case ShaderType::color_fill:
		shader_name = "Color Fill";
		shader_illumination = &shader_color_fill;
		shader_illumination->begin();
		shader_illumination->setUniform3f("color", 1.0f, 1.0f, 0.0f);
		shader_illumination->end();
		break;

	case ShaderType::lambert:
		shader_name = "Lambert";
		shader_illumination = &shader_lambert;
		shader_illumination->begin();
		shader_illumination->setUniform3f("color_ambient", 0.1f, 0.1f, 0.1f);
		shader_illumination->setUniform3f("color_diffuse", 0.6f, 0.6f, 0.6f);
		shader_illumination->setUniform3f("light_position", light.getGlobalPosition());
		shader_illumination->end();
		break;

	case ShaderType::gouraud:
		shader_name = "Gouraud";
		shader_illumination = &shader_gouraud;
		shader_illumination->begin();
		shader_illumination->setUniform3f("color_ambient", 0.1f, 0.1f, 0.1f);
		shader_illumination->setUniform3f("color_diffuse", 0.6f, 0.6f, 0.0f);
		shader_illumination->setUniform3f("color_specular", 1.0f, 1.0f, 0.0f);
		shader_illumination->setUniform1f("brightness", oscillation);
		shader_illumination->setUniform3f("light_position", light.getGlobalPosition());
		shader_illumination->end();
		break;

	case ShaderType::phong:
		shader_name = "Phong";
		shader_illumination = &shader_phong;
		shader_illumination->begin();
		shader_illumination->setUniform3f("color_ambient", 0.1f, 0.1f, 0.1f);
		shader_illumination->setUniform3f("color_diffuse", 0.6f, 0.0f, 0.6f);
		shader_illumination->setUniform3f("color_specular", 1.0f, 1.0f, 0.0f);
		shader_illumination->setUniform1f("brightness", oscillation);
		shader_illumination->setUniform3f("light_position", light.getGlobalPosition());
		shader_illumination->end();
		break;

	case ShaderType::blinn_phong:
		shader_name = "Blinn-Phong";
		shader_illumination = &shader_blinn_phong;
		shader_illumination->begin();
		shader_illumination->setUniform3f("color_ambient", 0.1f, 0.1f, 0.1f);
		shader_illumination->setUniform3f("color_diffuse", 0.0f, 0.6f, 0.6f);
		shader_illumination->setUniform3f("color_specular", 1.0f, 1.0f, 0.0f);
		shader_illumination->setUniform1f("brightness", oscillation);
		shader_illumination->setUniform3f("light_position", light.getGlobalPosition());
		shader_illumination->end();
		break;

	default:
		break;
	}
	//////////////////////////////////////
}

void Renderer::toggleColorWheelGUI() {
	interface.toggleColorWheel();
}

void Renderer::toggleExportGUI() {
	visible = !visible;
	interface.export_activate = !interface.export_activate;
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

