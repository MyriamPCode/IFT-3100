// IFT3100H23_ImageImport/renderer.cpp
// Classe responsable du rendu de l'application.

#include "renderer.h"

void Renderer::setup()
{

}

void Renderer::draw()
{
	auto currImg = imgPosList.begin();
	for (std::list<ofImage>::iterator iter = imageList.begin(); iter != imageList.end(); ++iter) {
		std::vector temp = *currImg;
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
