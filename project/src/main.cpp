#include "ofMain.h"
#include "application.h"
#include "renderer.h"

//========================================================================
int main( ){

	//settings.windowMode = OF_WINDOW; //can also be OF_FULLSCREEN

	//auto window = ofCreateWindow(settings);

	////ofRunApp(window, make_shared<ofApp>());
	//ofRunMainLoop();

	ofSetupOpenGL(1024, 768, OF_WINDOW);

	// paramètres du contexte de rendu OpenGL
	ofGLFWWindowSettings windowSettings;
	//windowSettings.setSize(1024, 768);

	// option de redimentionnement de la fenêtre d'affichage
	windowSettings.resizable = true;

	// sélection de la version de OpenGL
	windowSettings.setGLVersion(3, 3);

	// création de la fenêtre
	//ofCreateWindow(windowSettings);

	// démarrer l'exécution de l'application
	ofRunApp(new Application());
}
