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

	tint.set(255, 255, 255);
	mix_factor = 0.618f;
	//shaderFiltres.load("image_filter_330_vs.glsl", "image_filter_330_fs.glsl");
	
	gui.setup("Exportation");
	nameField.set("Image name", "name");
	gui.add(nameField);
	exportButton.setName("Export");
	gui.add(exportButton);
	gui.setPosition(200, 40);

	ofSetLogLevel(OF_LOG_VERBOSE);

	setTeapotMaterials();
	setSphereMaterials();
	setCubeMaterials();

	teapotMultiple.loadModel("models/teapot.obj");
	teapotMultiple.setPosition(0, 0, 0);

	teapotOrtho.loadModel("models/teapotOrtho.obj");
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
	isModeIllumination = false;  /// mettre a false pour desactiver au lancement 
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
	// initialisation de la scène
	reset();

	ofDisableArbTex();
	ofLoadImage(textu, "img/testure.jpg");
	ofImage tempEarth; tempEarth.load("img/earth_texture.jpg"); tempEarth.mirror(false, true); //S'occupe de bien préparer l'image
	textuSphere = tempEarth.getTexture();
	lightSetup();
}

void Renderer::lightSetup() {
	// configurer la lumière ambiante
	lightAmbient.set(interface.ambientLightColor);

	lightArea.setDiffuseColor(ofColor(interface.areaLightColor));
	lightArea.setSpecularColor(ofColor(interface.areaLightColor));
	lightArea.setPosition(ofVec3f(interface.areaLightPositionX, interface.areaLightPositionY,
		interface.areaLightPositionZ));
	lightArea.setOrientation(ofVec3f(interface.areaLightOrientationX, interface.areaLightOrientationY,
		interface.areaLightOrientationZ));
	lightArea.setAreaLight(interface.areaLightWidth,interface.areaLightHeight);

	// configurer la lumière directionnelle
	lightDirectionnal.setDiffuseColor(ofColor(interface.directionnalLightColor));
	lightDirectionnal.setSpecularColor(ofColor(interface.directionnalLightColor));
	lightDirectionnal.setPosition(ofVec3f(interface.directionnalLightPositionX, interface.directionnalLightPositionY,
		interface.directionnalLightPositionZ));
	lightDirectionnal.setOrientation(ofVec3f(interface.directionnalLightOrientationX, interface.directionnalLightOrientationY,
		interface.directionnalLightOrientationZ));
	lightDirectionnal.setDirectional();
	//lightDirectionnal.setAreaLight(200,200); Peut etre mettre un Area Light dans le mix
	
	// configurer la lumière ponctuelle
	lightPoint.setDiffuseColor(ofColor(interface.pointLightColor));
	lightPoint.setSpecularColor(ofColor(interface.pointLightColor));
	lightPoint.setPointLight();

	// configurer la lumière projecteur
	lightSpot.setDiffuseColor(ofColor(interface.spotLightColor));
	lightSpot.setSpecularColor(ofColor(interface.spotLightColor));
	lightSpot.setOrientation(ofVec3f(interface.spotLightOrientationX, interface.spotLightOrientationY,
		interface.spotLightOrientationZ));
	lightSpot.setSpotConcentration(2);
	lightSpot.setSpotlightCutOff(30);
	lightSpot.setSpotlight();

	lightTest.load("shader/multiLighting330_vs.glsl","shader/multiLighting330_fs.glsl");

	//shaderLight.load("shaders/color_fill_330_vs.glsl","shaders/color_fill_330_fs.glsl"); //Implementer en suivant EX02 du module 7
	//shaderLight.setUniform3f("color", 230.0f, 145.0f, 200.0f);
	//shaderLight.load("shaders/lambert_330_vs.glsl", "shaders/lambert_330_fs.glsl");
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
	if (interface.showAmbientLight) {
		lightAmbient.set(interface.ambientLightColor);
	}
	if (interface.showAreaLight) {
		lightArea.setDiffuseColor(ofColor(interface.areaLightColor));
		lightArea.setSpecularColor(ofColor(interface.areaLightColor));
		lightArea.setPosition(ofVec3f(interface.areaLightPositionX, interface.areaLightPositionY,
			interface.areaLightPositionZ));
		lightArea.setOrientation(ofVec3f(interface.areaLightOrientationX, interface.areaLightOrientationY,
			interface.areaLightOrientationZ));
		lightArea.setAreaLight(interface.areaLightWidth, interface.areaLightHeight);
	}
	if (interface.showDirectionnalLight)
	{
		lightDirectionnal.setDiffuseColor(ofColor(interface.directionnalLightColor));
		lightDirectionnal.setSpecularColor(ofColor(interface.directionnalLightColor));
		lightDirectionnal.setPosition(ofVec3f(interface.directionnalLightPositionX, interface.directionnalLightPositionY, 
			interface.directionnalLightPositionZ));
		lightDirectionnal.setOrientation(ofVec3f(interface.directionnalLightOrientationX, interface.directionnalLightOrientationY,
			interface.directionnalLightOrientationZ));
	}

	if (interface.showPointLight)
	{
		lightPoint.setDiffuseColor(ofColor(interface.pointLightColor));
		lightPoint.setSpecularColor(ofColor(interface.pointLightColor));
		lightPoint.setPosition(ofVec3f(interface.pointLightPositionX, interface.pointLightPositionY,
			interface.pointLightPositionZ));
	}

	if (interface.showSpotLight)
	{
		// configurer la lumière projecteur
		lightSpot.setDiffuseColor(ofColor(interface.spotLightColor));
		lightSpot.setSpecularColor(ofColor(interface.spotLightColor));
		lightSpot.setOrientation(ofVec3f(interface.spotLightOrientationX, interface.spotLightOrientationY,
			interface.spotLightOrientationZ));
		lightSpot.setPosition(ofVec3f(interface.spotLightPositionX, interface.spotLightPositionY,
			interface.spotLightPositionZ));
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
	switch (interface.illuminationType)
	{
	case 0:
		shader_name = "Color Fill";
		shader_illumination = &shader_color_fill;
		shader_illumination->begin();
		shader_illumination->setUniform3f("color", 1.0f, 1.0f, 0.0f);
		shader_illumination->end();
		break;

	case 1:
		shader_name = "Lambert";
		shader_illumination = &shader_lambert;
		shader_illumination->begin();
		shader_illumination->setUniform3f("color_ambient", 0.1f, 0.1f, 0.1f);
		shader_illumination->setUniform3f("color_diffuse", 0.6f, 0.6f, 0.6f);
		shader_illumination->setUniform3f("light_position", light.getGlobalPosition());
		shader_illumination->end();
		break;

	case 2:
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

	case 3:
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

	case 4:
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

	lightTest.begin();
	//Update Ambient Struct
	lightTest.setUniform3f("ambLight.color", lightAmbient.r, lightAmbient.g, lightAmbient.b);
	//Update Directionnal Struct
	lightTest.setUniform3f("dirLight.direction", //Calcule les directions avec des rotations vectorielles
		cos(ofDegToRad(interface.directionnalLightOrientationZ)) - sin(ofDegToRad(interface.directionnalLightOrientationZ))
		+ cos(ofDegToRad(interface.directionnalLightOrientationY)) + sin(ofDegToRad(interface.directionnalLightOrientationY)),
		sin(ofDegToRad(interface.directionnalLightOrientationZ)) + cos(ofDegToRad(interface.directionnalLightOrientationZ)),
		cos(ofDegToRad(interface.directionnalLightOrientationY)) - sin(ofDegToRad(interface.directionnalLightOrientationY)));
	lightTest.setUniform3f("dirLight.ambient", lightDirectionnal.getAmbientColor().r, lightDirectionnal.getAmbientColor().g, 
		lightDirectionnal.getAmbientColor().b);
	lightTest.setUniform3f("dirLight.diffuse", lightDirectionnal.getDiffuseColor().r, lightDirectionnal.getDiffuseColor().g, 
		lightDirectionnal.getDiffuseColor().b);
	lightTest.setUniform3f("dirLight.specular", lightDirectionnal.getSpecularColor().r, lightDirectionnal.getSpecularColor().g, 
		lightDirectionnal.getSpecularColor().b);
	//Update Point Struct
	lightTest.setUniform3f("pointLight.position", lightPoint.getPosition());
	lightTest.setUniform1f("pointLight.constant", lightPoint.getAttenuationConstant());
	lightTest.setUniform1f("pointLight.linear", lightPoint.getAttenuationLinear());
	lightTest.setUniform1f("pointLight.quadratic", lightPoint.getAttenuationQuadratic());
	lightTest.setUniform3f("pointLight.ambient", lightPoint.getAmbientColor().r, lightPoint.getAmbientColor().g, 
		lightPoint.getAmbientColor().b);
	lightTest.setUniform3f("pointLight.diffuse", lightPoint.getDiffuseColor().r, lightPoint.getDiffuseColor().g, 
		lightPoint.getDiffuseColor().b);
	lightTest.setUniform3f("pointLight.specular", lightPoint.getSpecularColor().r, lightPoint.getSpecularColor().g, 
		lightPoint.getSpecularColor().b);
	//Update Spot Struct
	lightTest.setUniform3f("spotLight.position", lightSpot.getPosition());
	lightTest.setUniform3f("spotLight.direction", 
		cos(ofDegToRad(interface.spotLightOrientationZ)) - sin(ofDegToRad(interface.spotLightOrientationZ))
		+ cos(ofDegToRad(interface.spotLightOrientationY)) + sin(ofDegToRad(interface.spotLightOrientationY)),
		sin(ofDegToRad(interface.spotLightOrientationZ)) + cos(ofDegToRad(interface.spotLightOrientationZ)),
		cos(ofDegToRad(interface.spotLightOrientationY)) - sin(ofDegToRad(interface.spotLightOrientationY)));
	lightTest.setUniform1f("spotLight.cutoff", lightSpot.getSpotlightCutOff());
	lightTest.setUniform1f("spotLight.constant", lightSpot.getAttenuationConstant());
	lightTest.setUniform1f("spotLight.linear", lightSpot.getAttenuationLinear());
	lightTest.setUniform1f("spotLight.quadratic", lightSpot.getAttenuationQuadratic());
	lightTest.setUniform3f("spotLight.ambient", lightSpot.getAmbientColor().r, lightSpot.getAmbientColor().g,
		lightSpot.getAmbientColor().b);
	lightTest.setUniform3f("spotLight.diffuse", lightSpot.getDiffuseColor().r, lightSpot.getDiffuseColor().g, 
		lightSpot.getDiffuseColor().b);
	lightTest.setUniform3f("spotLight.specular", lightSpot.getSpecularColor().r, lightSpot.getSpecularColor().g,
		lightSpot.getSpecularColor().b);
	//Set Model Variables
	lightTest.setUniform3f("color_ambient", 0.1f, 0.1f, 0.1f);
	lightTest.setUniform3f("color_diffuse", 0.6f, 0.6f, 0.6f);
	lightTest.setUniform3f("color_specular", 0.2f, 0.2f, 0.2f);
	lightTest.setUniform1f("brightness", 0.5f);
	if (interface.showAmbientLight) {
		lightTest.setUniform1f("ambientActive", 1.0f);
	} else lightTest.setUniform1f("ambientActive", 0.0f);
	if (interface.showDirectionnalLight) {
		lightTest.setUniform1f("directionnalActive", 1.0f);
	}
	else lightTest.setUniform1f("directionnalActive", 0.0f);
	if (interface.showPointLight) {
		lightTest.setUniform1f("pointActive", 1.0f);
	}
	else lightTest.setUniform1f("pointActive", 0.0f);
	if (interface.showSpotLight) {
		lightTest.setUniform1f("spotActive", 1.0f);
	}
	else lightTest.setUniform1f("spotActive", 0.0f);

	lightTest.end();

	shaderLight.begin();
	shaderLight.setUniform3f("color_ambient", 0.1f, 0.1f, 0.1f);
	shaderLight.setUniform3f("color_diffuse", 0.6f, 0.6f, 0.6f);
	shaderLight.setUniform3f("pointLightPosition", lightPoint.getPosition());
	shaderLight.setUniform3f("spotLightPosition", lightSpot.getPosition());
	shaderLight.end();
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

	//shaderFiltre.begin();
	shaderFiltre.setUniformTexture("image", image.getTexture(), 1);
	shaderFiltre.setUniform3f("tint", tint.r / 255.0f, tint.g / 255.0f, tint.b / 255.0f);
	shaderFiltre.setUniform1f("factor", mix_factor);
	ofPushMatrix();
	ofTranslate(image.getWidth(), image.getHeight());
	ofSetColor(tint); // Définir la couleur de dessin avec la teinte sélectionnée
	image.draw(0, 0, image.getWidth(), image.getHeight());
	ofPopMatrix();
	//shaderFiltre.end();
	
	auto currImg = imgPosList.begin();
	for (list<ofImage>::iterator iter = imageList.begin(); iter != imageList.end(); ++iter) {
		vector temp = *currImg;
		iter->draw(temp[0], temp[1]);
		++currImg;

	}
	ofEnableLighting();
	drawLighting();
	ofEnableDepthTest();

	if (interface.mesh_activate) {
		mesh.draw();
	}

	if (interface.delaunay) {
		drawTriangulation();
	}

	if (interface.texturedSphere) {
		ofSpherePrimitive sphere;
		sphere.setPosition(0, 0, 50);
		sphere.setRadius(200);
		textuSphere.generateMipmap();
		//sphere.rotateDeg(180, ofVec3f(0, 1, 0));
		sphere.rotateDeg(180, ofVec3f(1, 0, 0));
		textuSphere.bind();
		sphere.draw();
		textuSphere.unbind();
	}

	if (interface.texturedBox) {
		ofBoxPrimitive cube;
		cube.mapTexCoordsFromTexture(textu);
		cube.setPosition(400, 0, 50);
		cube.setScale(3.0f, 3.0f, 3.0f);
		cube.rotateDeg(180, ofVec3f(0, 1, 0));
		cube.rotateDeg(180, ofVec3f(1, 0, 0));
		textu.bind();
		cube.draw();
		textu.unbind();
	}

	if (interface.texturedPanel) {
		ofPlanePrimitive plane;
		plane.mapTexCoordsFromTexture(textu);
		plane.setPosition(800, 700, 0);
		plane.setScale(ofVec3f(2, 3, 1));
		plane.rotateDeg(180, ofVec3f(1, 0, 0));
		textu.generateMipmap();
		textu.bind();
		plane.draw();
		textu.unbind();
	}


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
	ofPushMatrix();
	//shaderLight.begin();
	if (interface.activateMultiShader) {
		lightTest.begin();
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
			if (interface.activateMultiShader) {
				teapotMultiple.disableMaterials();
			}
			teapotMultiple.draw(OF_MESH_FILL);
			textu.generateMipmap();
			textu.bind();
			//teapotOrtho.disableMaterials();
			teapotOrtho.draw(OF_MESH_FILL);
			textu.unbind();
		}
		else if (interface.getRenderType() == MeshRenderMode::vertex) {
			teapotMultiple.draw(OF_MESH_POINTS);
			teapotOrtho.draw(OF_MESH_POINTS);
		}
	}
	ofPopMatrix();
	//shaderLight.end();

	/// Modele illumination
	// Load les 2 modeles 3D et un sphere au milieu
	if (interface.activateModelesIllumination) {
		activer_Illumination(); // Le commenter pour ne pas charger au lancement 
	}

	if (interface.showTeapotMaterials) {
		// désactiver le matériau
		material_teapot.end();

		// activer le matériau par défaut 
		teapotMultiple.enableMaterials();
		teapotOrtho.enableMaterials();
	}
	
	if (interface.activateMultiShader) {
		lightTest.end();
	}
	//shaderLight.end();
	ofDisableDepthTest();

	ofSetGlobalAmbientColor(ofColor(0, 0, 0));
	lightArea.disable();
	lightDirectionnal.disable();
	lightPoint.disable();
	lightSpot.disable();

	ofDisableLighting();
	// Afficher un message si l'enregistrement est activé
	if (isRecording) {
		ofDrawBitmapString("Enregistrement enmouse cours...", 20, 20);
	}
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
	/*modele_illumination2.setPosition(
		position_modele_ill_2.x,
		position_modele_ill_2.y + 15.0f,
		position_modele_ill_2.z);

	// dimension pomudachi
	modele_illumination2.setScale(
		scale_modele_ill_2,
		scale_modele_ill_2,
		scale_modele_ill_2);*/

	// dessiner un cube
	//ofDrawBox(0.0f, 0.0f, 0.0f, scale_cube);

	// activer le shader
	ofPopMatrix();

	shader_illumination->begin();

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

	// dessiner pomudachi 
	modele_illumination2.draw(OF_MESH_FILL);
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
					ofPopMatrix();
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

void Renderer::drawTriangulation() {
	for (auto iter = delaunayPoints.begin(); iter != delaunayPoints.end(); ++iter) {
		ofDrawCircle(iter->x, iter->y, 10);
	}
	for (auto iter = delaunayEdges.begin(); iter != delaunayEdges.end(); ++iter) {
		vector<ofVec2f> tempArray = *iter;
		ofDrawLine(tempArray[0].x, tempArray[0].y, tempArray[1].x, tempArray[1].y);
	}
}

void Renderer::calculateDelaunay() {
	list<vector<ofVec2f>> triangles;
	ofLog() << "Check 1";
	vector<ofVec2f> superTriangle = { ofVec2f(1000,200),ofVec2f(600,900),ofVec2f(1400,900) };
	triangles.push_front({ofVec2f(1000,200),ofVec2f(600,900),ofVec2f(1400,900)});
	ofLog() << "Check 2";
	for (auto iter = delaunayPoints.begin(); iter != delaunayPoints.end(); ++iter) {
		ofLog() << "Check 3";
		list<vector<ofVec2f>> badTriangles;
		for (auto it2 = triangles.begin(); it2 != triangles.end(); ++it2) {
			ofLog() << "Check 4";
			vector<ofVec2f> tempTriangle = *it2;
			if (isInsideCircumcircle(tempTriangle[0], tempTriangle[1], tempTriangle[2], *iter)) {
				ofLog() << "Check 5";
				badTriangles.push_back(*it2);
			}
		}
		list<vector<ofVec2f>> polygon;
		for (auto it3 = badTriangles.begin(); it3 != badTriangles.end(); ++it3) {
			ofLog() << "Check 6";
			vector<ofVec2f> currTri = *it3;
			vector<ofVec2f> edge1 = {currTri[0], currTri[1]};
			if (isEdgeUnique(badTriangles, edge1, currTri)) polygon.push_back(edge1);
			vector<ofVec2f> edge2 = {currTri[1], currTri[2]};
			if (isEdgeUnique(badTriangles, edge2, currTri)) polygon.push_back(edge2);
			vector<ofVec2f> edge3 = {currTri[2], currTri[0]};
			if (isEdgeUnique(badTriangles, edge3, currTri)) polygon.push_back(edge3);
		}
		for (auto it4 = badTriangles.begin(); it4 != badTriangles.end(); ++it4) {
			ofLog() << "Check 7";
			for (auto it5 = triangles.begin(); it5 != triangles.end();) {
				ofLog() << "Check 8";
				if (*it4 == *it5) {
					ofLog() << "Check 8^";
					it5 = triangles.erase(it5);
				}
				else ++it5;
			}
		}
		for (auto it6 = polygon.begin(); it6 != polygon.end(); ++it6) {
			ofLog() << "Check 9";
			vector<ofVec2f> currEdge = *it6;
			triangles.push_back({ currEdge[0], currEdge[1], *iter });
		}
	}
	for (auto it7 = triangles.begin(); it7 != triangles.end();) {
		ofLog() << "Check 10";
		vector<ofVec2f> currTri2 = *it7;
		if (hasSharedVertex(currTri2, superTriangle)) it7 = triangles.erase(it7);
		else ++it7;
	}
	ofLog() << "Check 11";
	delaunayEdges.clear();
	for (auto it8 = triangles.begin(); it8 != triangles.end(); ++it8) {
		ofLog() << "Check 12";
		vector<ofVec2f> currTri3 = *it8;
		delaunayEdges.push_back({{currTri3[0].x, currTri3[0].y}, {currTri3[1].x, currTri3[1].y }});
		delaunayEdges.push_back({{currTri3[1].x, currTri3[1].y}, {currTri3[2].x, currTri3[2].y }});
		delaunayEdges.push_back({{currTri3[2].x, currTri3[2].y}, {currTri3[0].x, currTri3[0].y }});
	}
}

bool Renderer::isInsideCircumcircle(ofVec2f p1, ofVec2f p2, ofVec2f p3, ofVec2f pTest) {
	ofLog() << "Check 4*";
	float p1tx = p1.x - pTest.x;
	float p1ty = p1.y - pTest.y;
	float p2tx = p2.x - pTest.x;
	float p2ty = p2.y - pTest.y;
	float p3tx = p3.x - pTest.x;
	float p3ty = p3.y - pTest.y;
	float c1 = (pow(p1tx, 2) + pow(p1ty, 2)) * (p2tx * p3ty - p3tx * p2ty);
	float c2 = (pow(p2tx, 2) + pow(p2ty, 2)) * (p1tx * p3ty - p3tx * p1ty);
	float c3 = (pow(p3tx, 2) + pow(p3ty, 2)) * (p1tx * p2ty - p2tx * p1ty);
	float det = c1 - c2 + c3;
	return det < 0;
}

bool Renderer::isEdgeUnique(list<vector<ofVec2f>> badTriangles, vector<ofVec2f> edge, vector<ofVec2f> currTri) {
	ofLog() << "Check 6*";
	for (auto itEdge = badTriangles.begin(); itEdge != badTriangles.end(); ++itEdge) {
		vector<ofVec2f> testTri = *itEdge;
		if (testTri == currTri) continue;
		vector<ofVec2f> edgeT1 = { testTri[0], testTri[1] };
		vector<ofVec2f> edgeT1Swap = { testTri[1], testTri[0] };
		vector<ofVec2f> edgeT2 = { testTri[1], testTri[2] };
		vector<ofVec2f> edgeT2Swap = { testTri[2], testTri[1] };
		vector<ofVec2f> edgeT3 = { testTri[2], testTri[0] };
		vector<ofVec2f> edgeT3Swap = { testTri[0], testTri[2] };
		if (edge == edgeT1 || edge == edgeT2 || edge == edgeT3 || edge == edgeT1Swap || edge == edgeT2Swap || edge == edgeT3Swap) {
			return false;
		}
	}
	return true;
}

bool Renderer::hasSharedVertex(vector<ofVec2f> triangle1, vector<ofVec2f> triangle2) {
	ofLog() << "Check 10*";
	if (triangle1[0] == triangle2[0] || triangle1[0] == triangle2[1] || triangle1[0] == triangle2[2]) return true;
	if (triangle1[1] == triangle2[0] || triangle1[1] == triangle2[1] || triangle1[1] == triangle2[2]) return true;
	if (triangle1[2] == triangle2[0] || triangle1[2] == triangle2[1] || triangle1[2] == triangle2[2]) return true;
	return false;
}

void Renderer::drawLighting()
{
	if (interface.showAmbientLight)
		ofSetGlobalAmbientColor(lightAmbient);
	else
		ofSetGlobalAmbientColor(ofColor(0, 0, 0));

	if (interface.showAreaLight) {
		lightArea.enable();
		lightArea.draw();
	}
	else lightArea.disable();

	if (interface.showDirectionnalLight) {
		lightDirectionnal.enable();
		lightDirectionnal.draw();
	}
	else lightDirectionnal.disable();

	if (interface.showPointLight) {
		lightPoint.enable();
		lightPoint.draw();
	}
	else lightPoint.disable();

	if (interface.showSpotLight) {
		lightSpot.enable();
		lightSpot.draw();
	}
	else lightSpot.disable();
}

void Renderer::newImage(string filePath, int posX, int posY) {
	ofImage newImage;
	imageList.push_back(newImage);
	imageList.back().load(filePath);
	imgPosList.push_back({posX, posY});
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

