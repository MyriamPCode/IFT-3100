// IFT3100/renderer.h
// Classe responsable du rendu de l'application.

using namespace std;

#pragma once

#include "ofMain.h"

class Renderer
{
public:

  ofImage image;

  list<ofImage> imageList; //Liste d'images importées
  list<vector<int>> imgPosList; //Positions x et y des images importées

  void setup();
  void draw();
  void newImage(std::string filePath, int posX, int posY);
};
