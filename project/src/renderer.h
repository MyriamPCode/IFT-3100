#pragma once

#include "ofMain.h"
#include "Interface.h"
#include "ofxAssimpModelLoader.h"
#include "Forme.h"
#include <memory>
#include "ofxGui.h"
#include <array>

#include "ofxAssimpModelLoader.h"


using namespace std;

//enum class MeshRenderMode { fill, wireframe, vertex };
enum class ConvolutionKernel {identity, emboss, sharpen, edge_detect, blur};

const std::array<float, 9> convolution_kernel_identity
{
  0.0f,  0.0f,  0.0f,
  0.0f,  1.0f,  0.0f,
  0.0f,  0.0f,  0.0f
};

// kernel de convolution (3x3) : aiguiser
const std::array<float, 9> convolution_kernel_sharpen
{
  0.0f, -1.0f,  0.0f,
 -1.0f,  5.0f, -1.0f,
  0.0f, -1.0f,  0.0f
};

// kernel de convolution (3x3) : détection de bordure
const std::array<float, 9> convolution_kernel_edge_detect
{
  0.0f,  1.0f,  0.0f,
  1.0f, -4.0f,  1.0f,
  0.0f,  1.0f,  0.0f
};

// kernel de convolution (3x3) : bosseler
const std::array<float, 9> convolution_kernel_emboss
{
 -2.0f, -1.0f,  0.0f,
 -1.0f,  1.0f,  1.0f,
  0.0f,  1.0f,  2.0f
};

// kernel de convolution (3x3) : flou
const std::array<float, 9> convolution_kernel_blur
{
  1.0f / 9.0f,  1.0f / 9.0f,  1.0f / 9.0f,
  1.0f / 9.0f,  1.0f / 9.0f,  1.0f / 9.0f,
  1.0f / 9.0f,  1.0f / 9.0f,  1.0f / 9.0f
};

class Renderer {
public:
    Interface interface;

    ConvolutionKernel kernel_type;

    string kernel_name;

    ofImage image;

    ofShader shader;

    float mix_factor;
    ofColor tint;

    ofMesh mesh;
    int size = 96; // Taille de la maille

    list<ofImage> imageList; //Liste d'images import�es
    list<vector<int>> imgPosList; //Positions x et y des images import�es

        ofxAssimpModelLoader teapotOrtho;
        ofxAssimpModelLoader teapotMultiple;
        ofxAssimpModelLoader model1;
        ofxAssimpModelLoader model2;
        ofxAssimpModelLoader model3;
        //MeshRenderMode typeRender;


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
    void setup(vector<Forme*>& v_formes);
    void draw();
    void newImage(string filePath, int posX, int posY);
    void draw_cursor(float x, float y) const;

      Forme forme; 
        vector<unique_ptr<Forme>>* v_formes_ptr;
        vector<unique_ptr<Forme>> v_formes;
        void setup(vector<unique_ptr<Forme>>& v_formes);
        bool okDessiner; 
        void dessinerTriangle();
        vector<ofColor> triangleColors = {}; // Contient les couleurs du dessin
        bool triangleFill = false; // Indique si l'interieur de la forme doit etre colore
        float triangleStroke; // Taille du outline
        void dessinerCercle(); 
        vector<ofColor> cercleColors = {}; // Contient les couleurs du dessin
        bool cercleFill = false; // Indique si l'interieur de la forme doit etre colore
        float cercleStroke; // Taille du outline
        void dessinerRectangle(); 
        vector<ofColor> rectangleColors = {}; // Contient les couleurs du dessin
        bool rectangleFill = false; // Indique si l'interieur de la forme doit etre colore
        float rectangleStroke; // Taille du outline
        void dessinerLigne(); 
        ofColor ligneColor; // Contient la couleur du dessin
        float ligneStroke; // Taille du outline
        void dessinerEllipse();
        vector<ofColor> ellipseColors = {}; // Contient les couleurs du dessin
        bool ellipseFill = false; // Indique si l'interieur de la forme doit etre colore
        float ellipseStroke; // Taille du outline
        void dessinerBezier();
        vector<ofColor> bezierColors = {}; // Contient les couleurs du dessin
        bool bezierFill = false; // Indique si l'interieur de la forme doit etre colore
        float bezierStroke; // Taille du outline

    void dessinerSphere();
    void dessinerCube(); 

    vector<unique_ptr<Forme>>* getVecteurFormesPtr() {
        return &v_formes;
    }

    bool modeDrawState, modeTransformState; // pour indiquer si en mode draw ou transformation

    ofPolyline ligne;
    void addPoint(const ofVec3f& point) {
        // Ajouter le point à une liste de points
        ligne.addVertex(point);
    }
    vector<ofPolyline> vecteur_lignes;

    //Capture images
    void update();
    void captureImage();
    int frameCounter, captureInterval, compteur;
    bool isRecording;
  
      void toggleExportGUI();
        void toggleColorWheelGUI();

    ofParameter<ofVec2f> uiPosition, uiStep, uiShift, uiSize;
    ofParameter <ofVec3f> uiRotate;
    ofParameter <int> uiAmount; // Total de la liste formes 

    void filter();

    ofxInputField<int> inputIndex; 

    private:
        ofxPanel gui;
        ofParameter<string> nameField;
        ofParameter<bool> visible = false;
        ofParameter<bool> exportButton = false;

        void image_export(const string name, const string extension) const;

};
