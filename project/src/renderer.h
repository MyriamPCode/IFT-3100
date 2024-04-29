#pragma once

#include "ofMain.h"
#include "Interface.h"
#include "ofxAssimpModelLoader.h"
#include "Forme.h"
#include <memory>
#include "ofxGui.h"
#include "ofxAssimpModelLoader.h"
#include <iostream>
#include <vector>
#include <glm/glm.hpp>


using namespace std;

//enum class MeshRenderMode { fill, wireframe, vertex };

// Illumination 
enum class ShaderType { color_fill, lambert, gouraud, phong, blinn_phong };

class Renderer {
    public:
        Interface interface;
        ofImage image;
        ofShader shader;
        float mix_factor;
        ofColor tint;

    ofShader shader;
    ofShader shaderLight;

    ofMesh mesh;
    int size = 96; // Taille de la maille
    ofImage textureImage;
    ofTexture textu;
    ofTexture textuSphere;
    list<ofImage> imageList; //Liste d'images import�es
    list<vector<int>> imgPosList; //Positions x et y des images import�es

    ofxAssimpModelLoader teapotOrtho;
    ofxAssimpModelLoader teapotMultiple;
    //MeshRenderMode typeRender;

    ofColor lightAmbient;
    ofLight lightArea;
    ofLight lightDirectionnal;
    ofLight lightPoint;
    ofLight lightSpot;

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
    void lightSetup();
    void setup(vector<Forme*>& v_formes);
    void draw();
    void drawLighting();
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

        ofxInputField<int> inputIndex; 

        // Modele Illumination 
        void reset();
        ShaderType shader_active;
        ofShader shader_color_fill, shader_lambert, shader_gouraud, shader_phong, shader_blinn_phong;
        ofShader* shader_illumination;
        ofLight light;
        ofxAssimpModelLoader modele_illumination1, modele_illumination2;
        ofVec3f position_cube, position_sphere, position_modele_ill_1, position_modele_ill_2;
        string shader_name;
        float oscillation, oscillation_frequency, oscillation_amplitude;
        float scale_cube, scale_sphere, scale_modele_ill_1, scale_modele_ill_2;
        float speed_motion;
        float offset_x, offset_z;
        float delta_x, delta_z;
        float initial_x, initial_z;
        float center_x, center_y;
        float oscillate(float time, float frequency, float amplitude);
        void activer_Illumination();
        bool isModeIllumination;

        void setSphereMaterials();
        void setCubeMaterials();

    private:
        ofMaterial material_teapot;
        ofMaterial material_sphere;
        ofMaterial material_cube;

        ofxPanel gui;
        ofParameter<string> nameField;
        ofParameter<bool> visible = false;
        ofParameter<bool> exportButton = false;

        void image_export(const string name, const string extension) const;
        void setTeapotMaterials();
};
