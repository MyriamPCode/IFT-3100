#pragma once

#include "ofMain.h"
#include "Interface.h"
#include "ofxAssimpModelLoader.h"

using namespace std;

enum class MeshRenderMode { fill, wireframe, vertex };

class Renderer {
	public:
		Interface interface;
		ofImage image;

		list<ofImage> imageList; //Liste d'images import�es
		list<vector<int>> imgPosList; //Positions x et y des images import�es

        ofxAssimpModelLoader teapot;
        ofxAssimpModelLoader bunny;


    int mouse_press_x;
    int mouse_press_y;
    
    int mouse_current_x;
    int mouse_current_y;

    int mouse_drag_x;
    int mouse_drag_y;

    bool is_mouse_button_pressed;
    bool is_mouse_button_dragged;
    bool import_activate;

    void setup();
    void draw();
    void newImage(string filePath, int posX, int posY);
    void draw_cursor(float x, float y) const;
};
