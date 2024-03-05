#pragma once

#include "ofMain.h"
#include "Application.h"
#include "Constants.h"

int main( ){
	//Use ofGLFWWindowSettings for more options like multi-monitor fullscreen
	ofGLWindowSettings settings;
	settings.setSize(WIDTH, HEIGHT);
	settings.windowMode = OF_WINDOW; //can also be OF_FULLSCREEN

	auto window = ofCreateWindow(settings);

	ofRunApp(window, make_shared<Application>());
	ofRunMainLoop();


}
