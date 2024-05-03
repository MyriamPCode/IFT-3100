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
#include <GL/glu.h>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

using namespace std;

//enum class MeshRenderMode { fill, wireframe, vertex };

// Illumination 
enum class ShaderType { color_fill, lambert, gouraud, phong, blinn_phong };

class Renderer {
    public:
        Interface interface;
        ofImage image;
        ofShader shader;
        ofShader shaderFiltres;
        float mix_factor;
        ofColor tint;
    ofShader shaderLight;
    ofShader lightTest;

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
        float offset_x, offset_y, offset_z;
        float delta_x, delta_y, delta_z;
        float initial_x, initial_y, initial_z;
        float center_x, center_y, center_z;
        float oscillate(float time, float frequency, float amplitude);
        void activer_Illumination();
        bool isModeIllumination;
        // COONS 
        ofVec3f controlPoint0, controlPoint1, controlPoint2, controlPoint3;
        ofVec3f controlPoint0A, controlPoint1A, controlPoint2A, controlPoint3A;
        ofVec3f controlPoint0B, controlPoint1B, controlPoint2B, controlPoint3B;
        ofVec3f positionsCoons;
        ofPolyline line_renderer;
        int line_resolution, index;
        vector<ofVec3f> controlPointCoons;
        vector<vector<ofVec3f>> controlPointCoons2;
        vector<ofVec3f> controlPointCourbes;

        void bezierCubic(float t,
            float p1x, float p1y, float p1z,
            float p2x, float p2y, float p2z,
            float p3x, float p3y, float p3z,
            float p4x, float p4y, float p4z,
            float& x, float& y, float& z);

        ofVec3f interpolation_linear(const ofVec3f& value1, const ofVec3f& value2, float t);
        ofVec3f interpolation_bilineaire(const ofVec3f& c1, const ofVec3f& c2, const ofVec3f& c3, const ofVec3f& c4, float u, float v);
        ofVec3f surfaceCoons(const vector<ofVec3f>& controlPoints, float u, float v);
        void drawCoons(const vector<vector<ofVec3f>>& controlPoints, int resolutionU, int resolutionV);
        void drawSetupCoons();
        void setupCoons();
        bool isCoons;
        ofVec3f* selected_ctrl_point;
        ofPolyline coonsPoints;
        vector<ofPolyline> v_coonsPoints;
        
        //////////////
        // Texture
        ofShader shaderTexture; 
        ofColor material_color_ambient, material_color_diffuse, material_color_specular;
        float material_metallic, material_roughness, material_occlusion, material_brightness;
        glm::vec3 material_fresnel_ior; 
        ofImage texture_diffuse, texture_metallic, texture_roughness, texture_occlusion;
        ofLight lightTexture; 
        ofColor light_color; 
        float light_intensity;
        bool light_motion, tone_mapping_toggle;
        float tone_mapping_exposure, tone_mapping_gamma;
        ofxAssimpModelLoader teapotTexture; 
        ofVec3f position_cube_texture, position_sphere_texture, position_teapot_texture; 
        float scale_cube_texture, scale_sphere_texture, scale_teapot_texture;
        float speed_motion_texture, speed_rotation_texture;
        float offsetTexture_x, offsetTexture_z, deltaTexture_x, deltaTexture_y, deltaTexture_z;
        float rotation_y;
        float initialTexture_x, initialTexture_z, centerTexture_x, centerTexture_y; 
        bool isTexture = false; 
        void afficherTexture(); 

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
