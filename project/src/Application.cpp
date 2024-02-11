#include "application.h"

// fonction appel�e � l'initialisation de l'application
void Application::setup()
{
	ofSetWindowTitle("lab");

	ofLog() << "<app::setup>";

	renderer.setup();
}

void Application::keyPressed(int key) {
	// D�marrer/arr�ter l'enregistrement lors de l'appui sur la touche 'r'
	if (key == 'r') {
		renderer.isRecording = !renderer.isRecording;
		if (renderer.isRecording) {
			renderer.frameCounter = 0; // R�initialiser le compteur de frames lors du d�marrage de l'enregistrement
			cout << "Enregistrement d�marr�." << endl;
			//renderer.update();
			//renderer.captureImage();
		}
		else {
			cout << "Enregistrement arr�t�." << endl;
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
		// Mettez � jour et capturez l'image � intervalles r�guliers
		renderer.update();
		renderer.captureImage();
	}
}

// fonction appel�e � chaque mise � jour du rendu graphique de l'application
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

// fonction appel�e juste avant de quitter l'application
void Application::exit()
{
	ofLog() << "<app::exit>";
}