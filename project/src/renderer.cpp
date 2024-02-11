#include "renderer.h"

void Renderer::setup()
{

    // fréquence de rafraichissement du rendu de la fenêtre d'affichage par seconde
    ofSetFrameRate(60);
    frameCounter = 0;
    captureInterval = 30;
    isRecording = false;

    // couleur de l'arrière-plan
    ofSetBackgroundColor(31);

    // résolution de la fenêtre d'affichage en fonction de l'image
    ofSetWindowShape(2048 / 2, 2048 / 2);

    // chargement du code source des shaders
    shader.load("image_tint_330_fs.glsl",
        "image_tint_330_vs.glsl");


}
void Renderer::draw()
{
    // Afficher un message si l'enregistrement est activé
    if (isRecording)
        ofDrawBitmapString("Enregistrement enmouse cours...", 20, 20);

    int gridSize = 20; // Espacement de la grille

    // Obtenir les coordonnées de la souris
    int mouseX = ofGetMouseX();
    int mouseY = ofGetMouseY();

    // Convertir les coordonnées de la souris dans l'espace de la grille
    int gridX = mouseX / gridSize;
    int gridY = mouseY / gridSize;

    // Dessiner les coordonnées de la souris sur la grille
    ofSetColor(255); // Couleur noire
    ofDrawBitmapString("Mouse X: " + ofToString(mouseX) + ", Mouse Y: " + ofToString(mouseY), 10, 20);
    ofDrawBitmapString("Grid X: " + ofToString(gridX) + ", Grid Y: " + ofToString(gridY), 10, 40);

    //rouge
    ofSetColor(175, 30, 45);
    ofDrawRectangle(0, 0, 1024, 128);
    //blanc
    ofSetColor(255, 255, 255);
    ofDrawRectangle(0, 128, 1024, 64);
    //bleu
    ofSetColor(25, 33, 104);
    ofDrawRectangle(0, 192, 1024, 192);
    //blanc
    ofSetColor(255, 255, 255);
    ofDrawRectangle(0, 384, 1024, 64);
    //rouge
    ofSetColor(175, 30, 45);
    ofDrawRectangle(0, 448, 1024, 128);

    // H
    //blanc

   // ofSetColor(255, 255, 255);
   // ofDrawRectangle(350, 210, 30, 150);
   // //ofDrawRectangle(440, 210, 30, 150);
   // //ofDrawRectangle(375, 270, 90, 30);
   // // Définir l'épaisseur de la ligne de la bordure
   // ofSetLineWidth(5);
   // ofNoFill(); // Pas de remplissage pour l'ellipse
   // ofDrawEllipse(450, 270, 350, 350);


   // //blanc
   // //ofSetColor(255);
   // //ofDrawCurve(250.0, 250.0, 350.0, 210.0, 350.0, 360.0);
   // //ofPolyline curve; 
   // //curve.addVertex(250, 250);
   // //curve.addVertex(350, 210);
   // //curve.draw();


   //// ofFill(); // Rétablir le remplissage par défaut
   // ofSetLineWidth(1); // Rétablir l'épaisseur de ligne par défaut
   // // Dessiner la grille
   // drawGrid(gridSize);

}

void Renderer::drawGrid(int spacing) {
    ofSetColor(200); // Couleur de la grille (gris clair)

    // Dessiner les lignes verticales
    for (int x = 0; x < ofGetWidth(); x += spacing) {
        ofDrawLine(x, 0, x, ofGetHeight());
    }

    // Dessiner les lignes horizontales
    for (int y = 0; y < ofGetHeight(); y += spacing) {
        ofDrawLine(0, y, ofGetWidth(), y);
    }
}


void Renderer::image_export(const string name, const string extension)const
{
    ofImage image;

    // extraire des données temporelles formatées
    string time_stamp = ofGetTimestampString("-%y%m%d-%H%M%S-%i");

    // générer un nom de fichier unique et ordonné
    string file_name = name + time_stamp + "." + extension;

    // capturer le contenu du framebuffer actif
    image.grabScreen(0, 0, ofGetWindowWidth(), ofGetWindowHeight());

    // sauvegarder le fichier image
    image.save(file_name);

    ofLog() << "<export image: " << file_name << ">";
}


void Renderer::update()
{

    if (isRecording)
    {
        frameCounter++;
        //captureInterval++; 
        //compteur++;
        if (frameCounter == captureInterval)
        {
            ofSaveScreen(ofToString(frameCounter / captureInterval) + ".png");
            //ofSaveScreen(ofToString(frameCounter) + ".png");
            frameCounter = 0;
            //captureInterval -= compteur;
        }
    }
}

void Renderer::captureImage() {
    // Exporter l'image
    ofSaveScreen(ofToString(frameCounter) + ".png");
}

// fonction qui permet de sélectionner une nouvelle couleur aléatoire
void Renderer::select_random_colors()
{
    // choisir une nouvelle valeur aléatoire par chaque composante de la couleur
    color_random.set(ofRandom(255), ofRandom(255), ofRandom(255));

    ofLog() << "<renderer::select random colors (" << color_random << ")>";
}