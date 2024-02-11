#include "application.h"

// fonction appelée à l'initialisation de l'application
void Application::setup()
{
	ofSetWindowTitle("lab");

	ofLog() << "<app::setup>";

	renderer.setup();
}

void Application::keyPressed(int key) {
	// Démarrer/arrêter l'enregistrement lors de l'appui sur la touche 'r'
	if (key == 'r') {
		renderer.isRecording = !renderer.isRecording;
		if (renderer.isRecording) {
			renderer.frameCounter = 0; // Réinitialiser le compteur de frames lors du démarrage de l'enregistrement
			cout << "Enregistrement démarré." << endl;
			//renderer.update();
			//renderer.captureImage();
		}
		else {
			cout << "Enregistrement arrêté." << endl;
		}
	}
	else if (key == 'c')
	{
		ofLog() << "<app::keyReleased: " << key << ">";
		color_swap = true;
	}
	else if (key == 'n')
	{
		//ofLog() << "<app::keyReleased: " << key << ">";
		color_swap = false;
	}

}
void Application::mouseDragged(int x, int y, int button) {
	// Ajouter la position actuelle du curseur de la souris aux points
   // points.push_back(ofVec2f(x, y));
	ligne.addVertex(x, y);
}

void Application::mousePressed(int x, int y, int button)
{
	//ligne.clear();
	ligne.addVertex(x, y);
}

void Application::mouseReleased(int x, int y, int button)
{
	ligne.addVertex(x, y);
}

void Application::update()
{
	if (renderer.isRecording) {
		// Mettez à jour et capturez l'image à intervalles réguliers
		renderer.update();
		renderer.captureImage();
	}
}

// fonction appelée à chaque mise à jour du rendu graphique de l'application
void Application::draw()
{
	renderer.draw();

	//if (color_swap == false)
	//{
	//	ofSetColor(0);
	//	ofSetLineWidth(2);
	//}
	//else 
	//{
	//	renderer.select_random_colors();
	//	ofSetLineWidth(2);
	//}

	//ligne.draw();

}

// fonction appelée juste avant de quitter l'application
void Application::exit()
{
	ofLog() << "<app::exit>";
}