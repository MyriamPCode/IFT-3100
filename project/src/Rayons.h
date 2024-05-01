#pragma once

#include "ofMain.h"
#include "ofxGui.h"

#define _USE_MATH_DEFINES 
#include <cmath>

#include <iostream> 
#include <sstream>  
#include <fstream>

#include <vector>
#include <random>
#include <chrono>

struct Vector
{
    double x, y, z;

    // construction
    constexpr Vector() : x(0.0), y(0.0), z(0.0) {}
    constexpr Vector(double x) : x(x), y(0.0), z(0.0) {}
    constexpr Vector(double x, double y) : x(x), y(y), z(0.0) {}
    constexpr Vector(double x, double y, double z) : x(x), y(y), z(z) {}

    // produit scalaire (dot product)
    double dot(const Vector& v) const
    {
        return x * v.x + y * v.y + z * v.z;
    }

    // produit vectoriel (cross product)
    Vector cross(const Vector& v) const
    {
        return Vector(
            y * v.z - z * v.y,
            z * v.x - x * v.z,
            x * v.y - y * v.x);
    }

    // multiplication vectorielle
    Vector multiply(const Vector& v) const
    {
        return Vector(
            x * v.x,
            y * v.y,
            z * v.z);
    }

    // multiplication scalaire
    Vector operator*(double s) const
    {
        return Vector(
            x * s,
            y * s,
            z * s);
    }

    // addition vectorielle
    Vector operator+(const Vector& v) const
    {
        return Vector(
            x + v.x,
            y + v.y,
            z + v.z);
    }

    // soustraction vectorielle
    Vector operator-(const Vector& v) const
    {
        return Vector(
            x - v.x,
            y - v.y,
            z - v.z);
    }

    // normalisation
    Vector& normalize()
    {
        return *this = *this * (1.0 / sqrt(x * x + y * y + z * z));
    }
};

struct Ray
{
    Vector origin;
    Vector direction;

    Ray(Vector o, Vector d) : origin(o), direction(d) {}
};

struct Camera
{
    Vector position;
    Vector orientation;
    Vector target;

    Vector axis_x;
    Vector axis_y;
    Vector axis_z;

    Vector up;

    float viewport_width;
    float viewport_height;

    float fov;

    Camera(Vector p, Vector o) : position(p), orientation(o) {}

    // fonction qui permet de calibrer la caméra en fonction la valeur courante de ses attributs
    void calibrate()
    {
        axis_z = orientation;
        axis_x = Vector(viewport_width * fov / viewport_height);
        axis_y = axis_x.cross(axis_z).normalize() * fov;
    }
};

struct Image
{
    int width;  // largeur de l'image
    int height; // hauteur de l'image
    int count;  // nombre de pixels
    int index;  // index d'un des pixels

    double size; // poids en mémoire (en mégaoctets)

    // pointeur vers une séquence de pixels sous forme vectorielle
    Vector* pixel;

    // constructeurs
    Image() : width(1), height(1) {}
    Image(int w) : width(w), height(w) {}
    Image(int w, int h) : width(w), height(h) {}

    // fonction qui redimensionne l'image
    void resize(int w, int h)
    {
        if (w > 0 && h > 0)
        {
            width = w;
            height = h;

            // calculer le nombre de pixels dans l'image
            count = width * height;

            // calculer la taille de l'espace mémoire (en mégaoctets)
            size = sizeof(Vector) * count / pow(1024.0, 2);

            // allocation de la mémoire pour les pixels de l'image
            pixel = new Vector[count];
        }
    }

    ~Image()
    {
        if (pixel != nullptr)
            delete[] pixel;
    }
};

// types de matériau
enum class SurfaceType { diffuse, specular, refraction };

class Rayons {
public:
    ofxPanel rayonGui;
    Image image;

    // source d'entropie
    std::random_device rd;

    // générateur de nombres pseudo-aléatoires (algorithme mersenne twister)
    std::mt19937 rng{ rd() };

    // distribution uniforme entre 0 et 1
    std::uniform_real_distribution<double> random01{ 0.0, 1.0 };

    void setup();
    void update();
    void draw();

    double clamp(double x);
    int format_color_component(double value);
    bool raycast(const Ray& ray, double& distance, int& id);
    Vector compute_radiance(const Ray& ray, int depth);
    void init();
    void post_render();
    void render();
    void save_image_file(int width, int height, int ray_per_pixel, const Vector* pixel);
};
