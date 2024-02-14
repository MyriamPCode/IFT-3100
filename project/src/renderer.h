#pragma once

#include "ofMain.h"
#include "Interface.h"

using namespace std;

class Renderer {
	public:
		Interface interface;
		ofImage image;

		list<ofImage> imageList; //Liste d'images import�es
		list<vector<int>> imgPosList; //Positions x et y des images import�es

		void setup();
		void draw();
		void newImage(std::string filePath, int posX, int posY);
	};
