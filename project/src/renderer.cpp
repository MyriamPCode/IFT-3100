// IFT3100/renderer.cpp
// Classe responsable du rendu de l'application.

using namespace std;

#include "renderer.h"


void Renderer::setup()
{
	ofSetFrameRate(60);
	ofEnableDepthTest();
	is_mouse_button_pressed = false;
	is_mouse_button_dragged = false;
	import_activate = false;

	is_flip_axis_y = false;
	is_active_translation = true;
	is_active_rotation = false;
	is_active_proportion = false;

	mouse_current_x = mouse_current_y = mouse_press_x = mouse_press_y = mouse_drag_x = mouse_drag_y = 0;

	locator_count = 100;
	speed = 100.0f;
	offset_x = 0.0f;
	offset_z = 0.0f;
	delta_x = speed;
	delta_z = speed;

	locator_buffer_head = 0;

	// allocation d'un espace m�moire suffisamment grand pour contenir les donn�es de l'ensemble des localisateurs
	locators = (Locator*)std::malloc(locator_count * sizeof(Locator));

	reset();
}

void Renderer::draw()
{
	auto currImg = imgPosList.begin();
	for (list<ofImage>::iterator iter = imageList.begin(); iter != imageList.end(); ++iter) {
		vector temp = *currImg;
		iter->draw(temp[0], temp[1]);
		++currImg;
	}

	draw_cursor(mouse_current_x, mouse_current_y);

	ofPushMatrix();
	ofTranslate(center_x + offset_x, center_y, offset_z);

	// inverser l'axe Y pour qu'il pointe vers le haut
	ofScale(1.0f, is_flip_axis_y ? -1.0f : 1.0f);

	// transformer l'origine de la sc�ne au milieu de la fen�tre d'affichage
	ofTranslate(center_x + offset_x, is_flip_axis_y ? -center_y : center_y, offset_z);

	for (int index = 0; index < locator_buffer_head; ++index)
	{
		ofPushMatrix();
	if (is_active_translation)
	{
		// extraction des attributs de translation
		vector_position.x = locators[index].position[0];
		vector_position.y = locators[index].position[1];
		vector_position.z = locators[index].position[2];
	}
	else
	{
		// attributs de translation par d�faut
		vector_position.x = 0.0f;
		vector_position.y = 0.0f;
		vector_position.z = 0.0f;
	}

	// assignation des attributs de translation
	node.setPosition(
		vector_position.x,
		vector_position.y,
		vector_position.z);

	if (is_active_rotation)
	{
		// extraction des attributs de rotation
		vector_rotation.x = locators[index].rotation[0];
		vector_rotation.y = locators[index].rotation[1];
		vector_rotation.z = locators[index].rotation[2];
	}
	else
	{
		// attributs de rotation par d�faut
		vector_rotation.x = 0.0f;
		vector_rotation.y = 0.0f;
		vector_rotation.z = 0.0f;
	}

	// assignation des attributs de rotation
	node.setOrientation(vector_rotation);

	if (is_active_proportion)
	{
		// extraction des attributs de proportion
		vector_proportion.x = locators[index].proportion[0];
		vector_proportion.y = locators[index].proportion[1];
		vector_proportion.z = locators[index].proportion[2];
	}
	else
	{
		// attributs de proportion par d�faut
		vector_proportion.x = 1.0f;
		vector_proportion.y = 1.0f;
		vector_proportion.z = 1.0f;
	}

	// assignation des attributs de proportion
	node.setScale(
		vector_proportion.x,
		vector_proportion.y,
		vector_proportion.z);

	// dessiner le localisateur
	node.draw();

	ofPopMatrix();
    }
	draw_locator(5.0f);
	ofPopMatrix();
}

void Renderer::update() {
	// Calculer les coordonn�es du centre du framebuffer
	center_x = ofGetWidth() / 2.0f;
	center_y = ofGetHeight() / 2.0f;
}

void Renderer::newImage(std::string filePath, int posX, int posY) {
	ofImage newImage;
	imageList.push_back(newImage);
	imageList.back().load(filePath);
	imgPosList.push_back({posX, posY});
}

void Renderer::draw_cursor(float x, float y) const {
	float length = 10.0f;
	float offset = 5.0f;

	ofSetLineWidth(2);

	if (is_mouse_button_dragged) {
		ofSetColor(254, 142, 118); // Couleur rouge
	}

	else if (is_mouse_button_pressed) {
		ofSetColor(249, 220, 40); // Couleur jaune
	}
	else if (import_activate) {
		ofSetColor(135, 210, 88); // Couleur verte
	}

	else
		ofSetColor(255); // Couleur blanche

	ofDrawLine(x + offset, y, x + offset + length, y);
	ofDrawLine(x - offset, y, x - offset - length, y);
	ofDrawLine(x, y + offset, x, y + offset + length);
	ofDrawLine(x, y - offset, x, y - offset - length);

}

void Renderer::reset() {
	// distribuer les localisateurs dans l'espace visible de la sc�ne
	dispatch_locators(locator_count, std::min(ofGetWidth(), ofGetHeight()));

	offset_x = 0.0f;
	offset_z = 0.0f;

	ofLog() << "<reset>"; 
}

// fonction qui distribue les localisateurs dans un espace cubique
void Renderer::dispatch_locators(int count, float range)
{
	// variable temporaire
	float scale;

	// validations
	if (count <= 0 || range <= 0 || count > locator_count)
		return;

	// calculer la valeur de la moiti� du diam�tre du cube
	float halfRange = range / 2.0f;

	// configurer le nombre de localisateurs
	locator_buffer_head = count;

	for (int index = 0; index < locator_buffer_head; ++index)
	{
		// d�terminer des valeurs de position al�atoires dans le cube
		vector_position.x = ofRandom(-halfRange, halfRange);
		vector_position.y = ofRandom(-halfRange, halfRange);
		vector_position.z = ofRandom(-halfRange, halfRange);

		// d�terminer une rotation al�atoire sur l'axe Y
		vector_rotation.x = 0;
		vector_rotation.y = ofRandom(0, 360);
		vector_rotation.z = 0;

		// d�terminer une proportion uniforme au hasard entre 10% et 200%
		scale = ofRandom(0.1f, 2.0f);
		vector_proportion.x = scale;
		vector_proportion.y = scale;
		vector_proportion.z = scale;

		// configurer les attributs de transformation du localisateur
		locators[index].position[0] = vector_position.x;
		locators[index].position[1] = vector_position.y;
		locators[index].position[2] = vector_position.z;

		locators[index].rotation[0] = vector_rotation.x;
		locators[index].rotation[1] = vector_rotation.y;
		locators[index].rotation[2] = vector_rotation.z;

		locators[index].proportion[0] = vector_proportion.x;
		locators[index].proportion[1] = vector_proportion.y;
		locators[index].proportion[2] = vector_proportion.z;
	}
}

void Renderer::draw_locator(float scale) {
	ofSetLineWidth(2);
	ofSetColor(255);
	ofFill();
	ofPushMatrix();
	ofScale(scale, scale, scale);
	node.setPosition(0.0f, 0.0f, 0.0f);
	node.draw();
	ofPopMatrix();
}

Renderer::~Renderer()
{
	std::free(locators);
}