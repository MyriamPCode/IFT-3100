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

  int mouse_press_x;
  int mouse_press_y;
  
  int mouse_current_x;
  int mouse_current_y;

  int mouse_drag_x;
  int mouse_drag_y;

  bool is_mouse_button_pressed;
  bool is_mouse_button_dragged;

  void setup();
  void draw();
  void newImage(std::string filePath, int posX, int posY);
  void draw_cursor(float x, float y) const;
};
