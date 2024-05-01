#pragma once
#include "ofMain.h"
#include "Application.h"
#include <vector>

class Sphere
{
public: 
	double radius;   // rayon de la sph�re
	Vector position; // position du centre de la sph�re
	Vector emission; // couleur �missive de la sph�re
	Vector color;    // couleur diffuse de la sph�re

	SurfaceType material; // type de r�flexion de la sph�re

	// constructeur
	Sphere(double r, Vector p, Vector e, Vector c, SurfaceType m) : radius(r), position(p), emission(e), color(c), material(m) {}

	void draw() {
		//ofSetColor(color);
		ofDrawSphere(position.x, position.y, position.z, radius);
	}

	// fonction d'intersection entre la sph�re et un rayon
	double intersect(const Ray& ray) const
	{
		// distance de l'intersection la plus pr�s si elle existe
		double distance;

		// seuil de tol�rance num�rique du test d'intersection
		double epsilon = 1e-4;

		// distance du point d'intersection
		double t;

		// vecteur entre le centre de la sph�re et l'origine du rayon
		Vector delta = position - ray.origin;

		// calculer a
		double a = delta.dot(delta);

		// calculer b
		double b = delta.dot(ray.direction);

		// calculer c
		double c = radius * radius;

		// calculer le discriminant de l'�quation quadratique
		double discriminant = b * b - a + c;

		// valider si le discriminant est n�gatif
		if (discriminant < 0)
		{
			// il n'y a pas d'intersection avec cette sph�re
			return distance = 0;
		}

		// calculer la racine carr�e du discriminant seulement si non n�gatif
		discriminant = sqrt(discriminant);

		// d�terminer la distance de la premi�re intersection
		t = b - discriminant;

		// valider si la distance de la premi�re intersection est dans le seuil de tol�rance
		if (t > epsilon)
			distance = t;
		else
		{
			// d�terminer la distance de la premi�re intersection
			t = b + discriminant;

			// valider si la distance de la seconde intersection est dans le seuil de tol�rance
			if (t > epsilon)
				distance = t;
			else
				distance = 0;
		}

		// retourner la distance du point d'intersection
		return distance;
	}
};