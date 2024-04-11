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

	kernel_type = ConvolutionKernel::identity;
	kernel_name = "identité";

	filter();

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

	if (interface.mesh_activate) {
		mesh.drawWireframe();
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
							ofFill();
							ofSetColor(formeCourante->getColors()[1]);
							ofBeginShape();
							ofDrawRectangle(formeCourante->getXR(), formeCourante->getYR(),
								formeCourante->getWidth(), formeCourante->getHeight());
							ofEndShape();
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

void Renderer::filter() {
	const int kernel_size = 3;
	const int kernel_offset = kernel_size / 2;
	const int color_component_count = 3;

	int x, y;
	int i, j;
	int xi, yj;
	int c;

	int pixel_index_img_src;
	int pixel_index_img_dst;
	int kernel_index;
	float kernel_value;

	ofPixels pixel_array_src = image.getPixels(); // Utilisation de l'image de Renderer
	ofPixels pixel_array_dst = image.getPixels();

	ofColor pixel_color_src;
	ofColor pixel_color_dst;

	float sum[color_component_count];

	for (y = 0; y < image.getHeight(); ++y) { // Utilisation de getHeight() de l'image de Renderer
		for (x = 0; x < image.getWidth(); ++x) { // Utilisation de getWidth() de l'image de Renderer
			for (c = 0; c < color_component_count; ++c)
				sum[c] = 0;

			pixel_index_img_dst = (image.getWidth() * y + x) * color_component_count;

			for (j = -kernel_offset; j <= kernel_offset; ++j) {
				for (i = -kernel_offset; i <= kernel_offset; ++i) {
					xi = x - i;
					yj = y - j;

					if (xi < 0 || xi >= image.getWidth() || yj < 0 || yj >= image.getHeight())
						continue;

					pixel_index_img_src = (image.getWidth() * yj + xi) * color_component_count;
					pixel_color_src = pixel_array_src.getColor(pixel_index_img_src);

					kernel_index = kernel_size * (j + kernel_offset) + (i + kernel_offset);

					switch (kernel_type) {
					case ConvolutionKernel::identity:
						kernel_value = convolution_kernel_identity.at(kernel_index);
						break;
					case ConvolutionKernel::emboss:
						kernel_value = convolution_kernel_emboss.at(kernel_index);
						break;
					case ConvolutionKernel::sharpen:
						kernel_value = convolution_kernel_sharpen.at(kernel_index);
						break;
					case ConvolutionKernel::edge_detect:
						kernel_value = convolution_kernel_edge_detect.at(kernel_index);
						break;
					case ConvolutionKernel::blur:
						kernel_value = convolution_kernel_blur.at(kernel_index);
						break;
					default:
						kernel_value = convolution_kernel_identity.at(kernel_index);
						break;
					}

					for (c = 0; c < color_component_count; ++c) {
						sum[c] = sum[c] + kernel_value * pixel_color_src[c];
					}
				}
			}

			for (c = 0; c < color_component_count; ++c) {
				pixel_color_dst[c] = (int)ofClamp(sum[c], 0, 255);
			}

			pixel_array_dst.setColor(pixel_index_img_dst, pixel_color_dst);
		}
	}

	image.setFromPixels(pixel_array_dst);

	ofLog() << "<filtre de convolution complété (" << kernel_name << ")>";
}