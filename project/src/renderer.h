// IFT3100H23_ImageImport/renderer.h
// Classe responsable du rendu de l'application.

#pragma once

#include "ofMain.h"

class Renderer
{
public:

  ofImage image;

  std::list<ofImage> imageList; //Liste d'images importées
  std::list<std::vector<int>> imgPosList; //Positions x et y des images importées

  void setup();
  void draw();
  void newImage(std::string filePath, int posX, int posY);
};
