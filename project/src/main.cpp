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

	// param�tres du contexte de rendu OpenGL
	ofGLFWWindowSettings windowSettings;
	//windowSettings.setSize(1024, 768);

	// option de redimentionnement de la fen�tre d'affichage
	windowSettings.resizable = true;

	// s�lection de la version de OpenGL
	windowSettings.setGLVersion(3, 3);

	// cr�ation de la fen�tre
	//ofCreateWindow(windowSettings);

	// d�marrer l'ex�cution de l'application
	ofRunApp(new Application());
}
