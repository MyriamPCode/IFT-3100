#pragma once

#include "ofMain.h"

class Renderer : public ofBaseApp
{
public:
	/*
	ofColor color_background;
	ofColor color_tint;
	ofColor color_random;
	*/

	ofShader shader;

	bool filter;

	void setup();
	void draw();

	void image_export(const string name, const string extension) const;
	void captureImage();
	void update();
	int frameCounter, captureInterval, compteur;
	bool isRecording;


	vector<ofVec2f> points; // Vector pour stocker les positions du curseur de la souris

	ofColor logoColor;
	ofVec2f logoPosition;
	float logoSize;

	ofColor color_random;
	void select_random_colors();
	
	void drawGrid(int spacing);
};