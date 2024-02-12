// IFT3100/renderer.cpp
// Classe responsable du rendu de l'application.

using namespace std;

#include "Renderer.h"


void Renderer::setup()
{

}

void Renderer::draw()
{
	auto currImg = imgPosList.begin();
	for (list<ofImage>::iterator iter = imageList.begin(); iter != imageList.end(); ++iter) {
		vector temp = *currImg;
		iter->draw(temp[0], temp[1]);
		++currImg;
	}
}

void Renderer::newImage(std::string filePath, int posX, int posY) {
	ofImage newImage;
	imageList.push_back(newImage);
	imageList.back().load(filePath);
	imgPosList.push_back({posX, posY});
}
