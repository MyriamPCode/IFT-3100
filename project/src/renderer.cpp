#include "Renderer.h"
#include "Interface.h"

#include "Constants.h"

using namespace std;

void Renderer::setup() {
	ofSetFrameRate(60);
	interface.setup();
}

void Renderer::draw() {
	interface.draw();

	auto currImg = imgPosList.begin();
	for (list<ofImage>::iterator iter = imageList.begin(); iter != imageList.end(); ++iter) {
		vector temp = *currImg;
		iter->draw(temp[0], temp[1]);
		++currImg;
	}
}

void Renderer::newImage(string filePath, int posX, int posY) {
	ofImage newImage;
	imageList.push_back(newImage);
	imageList.back().load(filePath);
	imgPosList.push_back({posX, posY});
}