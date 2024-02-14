// IFT3100/renderer.h
// Classe responsable du rendu de l'application.

using namespace std;

#pragma once

#include "ofMain.h"

// structure d'un localisateur de transformation
struct Locator
{
	float position[3];   // 3 * 4 = 12 octets
	float rotation[3];   // 3 * 4 = 12 octets
	float proportion[3]; // 3 * 4 = 12 octets
};                       //       = 36 octets

class Renderer
{
public:

  ofImage image;
  ofNode node;
  Locator* locators;

  list<ofImage> imageList; //Liste d'images importées
  list<vector<int>> imgPosList; //Positions x et y des images importées

  ofVec3f vector_position;
  ofVec3f vector_rotation;
  ofVec3f vector_proportion;

  int mouse_press_x;
  int mouse_press_y;
  
  int mouse_current_x;
  int mouse_current_y;

  int mouse_drag_x;
  int mouse_drag_y;

  bool is_mouse_button_pressed;
  bool is_mouse_button_dragged;
  bool import_activate;

  float center_x;
  float center_y;

  float offset_x;
  float offset_z;

  float delta_x;
  float delta_z;

  float speed;

  int locator_count;

  int locator_buffer_head;

  bool is_flip_axis_y;

  bool is_active_translation;
  bool is_active_rotation;
  bool is_active_proportion;

  void setup();
  void draw();
  void update();
  void newImage(std::string filePath, int posX, int posY);
  void draw_cursor(float x, float y) const;
  void draw_locator(float scale);
  void dispatch_locators(int count, float range);

  void reset();

  ~Renderer();
};
