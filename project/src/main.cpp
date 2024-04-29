#pragma once

#include "ofMain.h"
#include "Application.h"
#include "Constants.h"

int main(int argc, char** argv){
	//Use ofGLFWWindowSettings for more options like multi-monitor fullscreen
	ofGLWindowSettings settings;
	settings.setSize(WIDTH, HEIGHT);
	settings.setGLVersion(3, 3);
	settings.windowMode = OF_WINDOW; //can also be OF_FULLSCREEN

	auto window = ofCreateWindow(settings);

	for (int index = 0; index < argc; ++index)
		std::cout << "arg[" << index << "]:\t" << argv[index];

	ofViewport(0, 0, WIDTH, HEIGHT);

	ofRunApp(window, make_shared<Application>());
	ofRunMainLoop();
}
