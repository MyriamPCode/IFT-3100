#pragma once

#include "ofMain.h"
#include "Application.h"
#include "Constants.h"

int main( ){
	//Use ofGLFWWindowSettings for more options like multi-monitor fullscreen
	ofGLFWWindowSettings settings;
	settings.setSize(WIDTH, HEIGHT);
	settings.setGLVersion(3, 3);
	settings.numSamples = 2;
	settings.doubleBuffering = true;
	settings.windowMode = OF_WINDOW; //can also be OF_FULLSCREEN

	auto window = ofCreateWindow(settings);

	ofViewport(0, 0, WIDTH, HEIGHT);

	ofRunApp(window, make_shared<Application>());
	ofRunMainLoop();
}
