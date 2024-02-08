// IFT3100H23_ImageImport/renderer.h
// Classe responsable du rendu de l'application.

#pragma once

#include "ofMain.h"

class Renderer
{
public:

  ofImage image;

  std::list<ofImage> imageList; //Liste d'images import�es
  std::list<std::vector<int>> imgPosList; //Positions x et y des images import�es

  void setup();
  void draw();
  void newImage(std::string filePath, int posX, int posY);
};
