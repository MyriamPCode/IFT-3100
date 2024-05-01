#pragma once
#include "ofMain.h"
#include "Application.h"
#include <vector>

class Sphere
{
public: 
	double radius;   // rayon de la sphère
	Vector position; // position du centre de la sphère
	Vector emission; // couleur émissive de la sphère
	Vector color;    // couleur diffuse de la sphère

	SurfaceType material; // type de réflexion de la sphère

	// constructeur
	Sphere(double r, Vector p, Vector e, Vector c, SurfaceType m) : radius(r), position(p), emission(e), color(c), material(m) {}

	void draw() {
		//ofSetColor(color);
		ofDrawSphere(position.x, position.y, position.z, radius);
	}

	// fonction d'intersection entre la sphère et un rayon
	double intersect(const Ray& ray) const
	{
		// distance de l'intersection la plus près si elle existe
		double distance;

		// seuil de tolérance numérique du test d'intersection
		double epsilon = 1e-4;

		// distance du point d'intersection
		double t;

		// vecteur entre le centre de la sphère et l'origine du rayon
		Vector delta = position - ray.origin;

		// calculer a
		double a = delta.dot(delta);

		// calculer b
		double b = delta.dot(ray.direction);

		// calculer c
		double c = radius * radius;

		// calculer le discriminant de l'équation quadratique
		double discriminant = b * b - a + c;

		// valider si le discriminant est négatif
		if (discriminant < 0)
		{
			// il n'y a pas d'intersection avec cette sphère
			return distance = 0;
		}

		// calculer la racine carrée du discriminant seulement si non négatif
		discriminant = sqrt(discriminant);

		// déterminer la distance de la première intersection
		t = b - discriminant;

		// valider si la distance de la première intersection est dans le seuil de tolérance
		if (t > epsilon)
			distance = t;
		else
		{
			// déterminer la distance de la première intersection
			t = b + discriminant;

			// valider si la distance de la seconde intersection est dans le seuil de tolérance
			if (t > epsilon)
				distance = t;
			else
				distance = 0;
		}

		// retourner la distance du point d'intersection
		return distance;
	}
};