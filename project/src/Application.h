#pragma once

#include "ofMain.h"
#include "renderer.h"

class Application : public ofBaseApp
{
public:

	Renderer renderer;

	void setup();

	void draw();

	//void windowResized(int w, int h);

	//void dragEvent(ofDragInfo dragInfo);

	//void keyReleased(int key);

	void keyPressed(int key);
	void update();

	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	ofPolyline ligne;

	bool color_swap = false;

	void exit();
};