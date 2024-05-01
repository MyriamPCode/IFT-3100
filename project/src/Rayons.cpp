#include "Rayons.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "Constants.h"

// param�tres du programme
const int max_depth = 5;
const double camera_fov = 0.5135; // ~30 degr�s
constexpr double gamma_correction = 1 / 2.2;

// orientation initiale de la cam�ra
Vector camera_orientation(Vector(0, -0.042612, -1).normalize());

// position initiale de la cam�ra
constexpr Vector camera_position(50, 50, 300);

// d�claration de la cam�ra utilis�e pour rendre la sc�ne
Camera camera(camera_position, camera_orientation);

struct Sphere
{
    double radius;   // rayon de la sph�re
    Vector position; // position du centre de la sph�re
    Vector emission; // couleur �missive de la sph�re
    Vector color;    // couleur diffuse de la sph�re

    SurfaceType material; // type de r�flexion de la sph�re

    // constructeur
    Sphere(double r, Vector p, Vector e, Vector c, SurfaceType m) : radius(r), position(p), emission(e), color(c), material(m) {}

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

// d�claration du graphe de sc�ne
std::vector<Sphere> scene;

void Rayons::setup() {
    //rayonGui.setup();
}

void Rayons::update() {
    auto t1 = std::chrono::high_resolution_clock::now();

    // rendu de la sc�ne
    render();

    auto t2 = std::chrono::high_resolution_clock::now();

    // temps de calcul
    auto render_time = t2 - t1;

    std::cout << "render time : " << std::chrono::duration_cast<std::chrono::seconds>(render_time).count() << "s" << std::endl;
}

void Rayons::draw() {

    // initialisation de la sc�ne
    init();

    // faire un rendu de la sc�ne par lancer de rayons
    update();

    // proc�dure post-rendu (sauvegarde de l'image et d�sallocation de la m�moire)
    post_render();
}

// fonction pour borner une valeur num�rique entre 0 et 1
double Rayons::clamp(double x)
{
    return x < 0 ? 0 : x > 1 ? 1 : x;
}

// traitement de la couleur d'un pixel
int Rayons::format_color_component(double value)
{
    // appliquer la correction gamma
    value = pow(clamp(value), gamma_correction);

    // convertir la valeur dans l'espace de couleur
    value = value * 255 + 0.5;

    // conversion num�rique de r�el vers entier
    return static_cast<int>(value);
}

// fonction qui valide s'il y a intersection entre un rayon et les g�om�tries de la sc�ne
// retourne l'intersection la plus pr�s de la cam�ra (distance et index de l'�l�ment)
bool Rayons::raycast(const Ray& ray, double& distance, int& id)
{
    // variable temporaire pour la distance d'une intersection entre un rayon et une sph�re
    double d;

    // initialiser la distance � une valeur suffisamment �loign�e pour qu'on la consid�re comme l'infinie
    double infinity = distance = 1e20;

    // nombre d'�l�ments dans la sc�ne
    int n = static_cast<int>(scene.size());

    // parcourir tous les �l�ments de la sc�ne
    for (int index = 0; index < n; ++index)
    {
        // test d'intersection entre le rayon et la g�om�trie � cet index
        d = scene[index].intersect(ray);

        // valider s'il y a eu intersection et si la distance est inf�rieure aux autres intersections
        if (d && d < distance)
        {
            // nouvelle valeur courante de la distance et l'index de l'intersection la plus rapproch�e de la cam�ra
            distance = d;
            id = index;
        }
    }

    // il y a eu intersection si la distance est plus petite que l'infini
    if (distance < infinity)
        return true;
    else
        return false;
}

// fonction r�cursive qui calcule la radiance
Vector Rayons::compute_radiance(const Ray& ray, int depth)
{
    // valeur de la radiance
    Vector radiance;

    // distance de l'intersection
    double distance;

    // identifiant de la g�om�trie en intersection
    int id = 0;

    // valider s'il n'y a pas intersection
    if (!raycast(ray, distance, id))
        return Vector(); // couleur par d�fault (noir)

    // r�f�rence sur une g�om�trie en intersection avec un rayon
    const Sphere& obj = scene[id];

    // calculer les coordonn�es du point d'intersection
    Vector x = ray.origin + ray.direction * distance;

    // calculer la normale au point d'intersection
    Vector n = (x - obj.position).normalize();

    // ajustement de la direction de la normale
    Vector na = n.dot(ray.direction) < 0 ? n : n * -1;

    // isoler la composante de couleur la plus puissante
    Vector f = obj.color;
    double threshold = f.x > f.y && f.x > f.z ? f.x : f.y > f.z ? f.y : f.z;

    // valider si la limite du nombre de r�cursions est atteinte
    if (++depth > max_depth)
    {
        // test de probabilit�
        if (random01(rng) < threshold)
            f = f * (1 / threshold);
        else
            return obj.emission;
    }

    if (obj.material == SurfaceType::diffuse)
    {
        // mat�riau avec r�flexion diffuse

        double r1 = 2 * M_PI * random01(rng);
        double r2 = random01(rng);
        double r2s = sqrt(r2);

        Vector w = na;
        Vector u = ((fabs(w.x) > 0.1 ? Vector(0, 1) : Vector(1)).cross(w)).normalize();
        Vector v = w.cross(u);
        Vector d = (u * cos(r1) * r2s + v * sin(r1) * r2s + w * sqrt(1 - r2)).normalize();

        radiance = obj.emission + f.multiply(compute_radiance(Ray(x, d), depth));

        return radiance;
    }
    else if (obj.material == SurfaceType::specular)
    {
        // mat�riau avec r�flexion sp�culaire

        radiance = obj.emission + f.multiply(compute_radiance(Ray(x, ray.direction - n * 2.0 * n.dot(ray.direction)), depth));

        return radiance;
    }
    else if (obj.material == SurfaceType::refraction)
    {
        // mat�riau avec r�flexion r�fraction

        Ray reflection_ray(x, ray.direction - n * 2.0 * n.dot(ray.direction));

        bool into = n.dot(na) > 0;

        double ior = 1.5; // indice de r�fraction du verre
        double nc = 1.0;
        double nt = ior;
        double nnt = into ? nc / nt : nt / nc;
        double ddn = ray.direction.dot(na);
        double cos2t;

        if ((cos2t = 1.0 - nnt * nnt * (1.0 - ddn * ddn)) < 0.0)
        {
            radiance = obj.emission + f.multiply(compute_radiance(reflection_ray, depth));

            return radiance;
        }

        Vector tdir = (ray.direction * nnt - n * ((into ? 1.0 : -1.0) * (ddn * nnt + sqrt(cos2t)))).normalize();

        // effet de fresnel
        double a = nt - nc;
        double b = nt + nc;
        double r0 = a * a / (b * b);
        double c = 1.0 - (into ? -ddn : tdir.dot(n));
        double re = r0 + (1.0 - r0) * c * c * c * c * c;
        double tr = 1 - re;
        double p = 0.25 + 0.5 * re;
        double rp = re / p;
        double tp = tr / (1.0 - p);

        radiance = obj.emission + f.multiply(depth > 2 ? (random01(rng) < p ?
            compute_radiance(reflection_ray, depth) * rp : compute_radiance(Ray(x, tdir), depth) * tp) :
            compute_radiance(reflection_ray, depth) * re + compute_radiance(Ray(x, tdir), depth) * tr);

        return radiance;
    }
    else
    {
        return radiance;
    }
}

// fonction d'initialisation de la sc�ne
void Rayons::init()
{
    std::cout << "scene setup" << std::endl;

    constexpr double anchor = 1e5;
    constexpr double wall_radius = anchor;

    constexpr double box_size_x = 100.0;
    constexpr double box_size_y = 81.6;
    constexpr double box_size_z = 81.6;

    constexpr double box_x_min = 0.0;
    constexpr double box_x_max = box_size_x;
    constexpr double box_y_min = 0.0;
    constexpr double box_y_max = box_size_y;
    constexpr double box_z_min = 0.0;
    constexpr double box_z_max = box_size_z;

    constexpr double box_center_x = (box_x_max - box_x_min) / 2.0;
    constexpr double box_center_y = (box_y_max - box_y_min) / 2.0;
    constexpr double box_center_z = (box_z_max - box_z_min) / 2.0;

    // vider la sc�ne de son contenu
    scene.clear();

    // g�n�ration du contenu de la sc�ne
    scene.insert(scene.begin(), {

        // approximation d'une bo�te de Cornell avec des sph�res surdimensionn�es qui simulent des surfaces planes
        Sphere(wall_radius, Vector(box_center_x,  anchor,            box_size_z),   Vector(), Vector(0.75, 0.75, 0.75), SurfaceType::diffuse),    // plancher
        Sphere(wall_radius, Vector(box_center_x, -anchor + box_size_y, box_size_z),   Vector(), Vector(0.75, 0.75, 0.75), SurfaceType::diffuse),    // plafond
        Sphere(wall_radius, Vector(anchor + 1,      box_center_y,      box_size_z),   Vector(), Vector(0.75, 0.25, 0.25), SurfaceType::diffuse),    // mur gauche
        Sphere(wall_radius, Vector(box_center_x,  box_center_y,      anchor),       Vector(), Vector(0.25, 0.75, 0.25), SurfaceType::diffuse),    // mur arri�re
        Sphere(wall_radius, Vector(-anchor + 99,     box_center_y,      box_size_z),   Vector(), Vector(0.25, 0.25, 0.75), SurfaceType::diffuse),    // mur droit
        Sphere(wall_radius, Vector(box_center_x,  box_center_y,     -anchor + 170),   Vector(), Vector(0.0,  0.0,  0.0),  SurfaceType::diffuse),    // mur avant

        // ensemble des sph�res situ�es � l'int�rieur de la bo�te de Cornell
        Sphere(22.5,        Vector(30,            30,                40),           Vector(), Vector(1.0, 1.0, 1.0),    SurfaceType::specular),   // sph�re mirroir
        Sphere(17.5,        Vector(75,            40,                75),           Vector(), Vector(1.0, 1.0, 1.0),    SurfaceType::refraction), // sph�re de verre

        Sphere(600,  Vector(box_center_x, 600.0 + box_size_z - 0.27, box_size_z), Vector(15, 15, 15), Vector(0.0, 0.0, 0.0), SurfaceType::diffuse) // sph�re lumineuse
        });

    // allocation de la m�moire de l'image en fonction des param�tres du programme
    image.resize(image_width, image_height);

    std::cout << "image resize to " << image.width << "x" << image.height << " (" << image.count << " pixels, " << image.size << " MB)" << std::endl;

    // calibration de la cam�ra
    camera.viewport_width = image.width;
    camera.viewport_height = image.height;
    camera.fov = camera_fov;
    camera.calibrate();
}

// fonction appel�e quand le rendu de la sc�ne est termin�e
void Rayons::post_render()
{
    // �crire les pixels dans un fichier image
    save_image_file(image.width, image.height, ray_per_pixel, image.pixel);

    std::cout << "raytracer task is done" << std::endl;
}

void Rayons::save_image_file(int width, int height, int ray_per_pixel, const Vector* pixel)
{
    // nom du fichier image de type .ppm (portable pixmap)
    std::stringstream ss;
    ss << "image" << width << "x" << height << "_" << ray_per_pixel << ".ppm";
    std::string filename = ss.str();

    // d�claration et ouverture du fichier en mode �criture
    std::ofstream file;
    file.open(filename, std::ios::out);

    // ent�te du ficher pour une image avec un espace de couleur RGB 24 bits (P3 pour pixmap)
    file << "P3\n";

    // largeur et hauteur de l'image sur la seconde ligne de l'ent�te
    file << width << ' ' << height << '\n';

    // valeur maximale de l'espace de couleur sur la troisi�me ligne de l'ent�te
    file << "255\n";

    // �criture des pixels dans le fichier image
    for (int index = 0; index < width * height; ++index)
    {
        // �crire la couleur du pixel dans le fichier image
        file << static_cast<std::uint32_t>(format_color_component(pixel[index].x)) << " ";
        file << static_cast<std::uint32_t>(format_color_component(pixel[index].y)) << " ";
        file << static_cast<std::uint32_t>(format_color_component(pixel[index].z)) << " ";
    }

    // fermeture du fichier
    file.close();
}

// fonction de rendu de la sc�ne
void Rayons::render()
{
    std::cout << "render start" << std::endl;

    unsigned short x = 0;

    int index, y, s, sx, sy = 0;

    float progression = 0.0f;

    double r1, r2 = 0.0;
    double dx, dy = 0.0;

    Vector radiance;

    Vector distance;

    cout << "avant boucle for" << endl;
    cout << "image height" << image_height << endl;

    // it�ration sur les rang�es de pixels
    for (y = 0; y < image_height; ++y)
    {
        cout << "dans boucle for" << endl;
        // calculer le pourcentage de progression
        progression = 100.0f * y / (image_height - 1.0f);

        // afficher le pourcentage de progression du rendu dans la console
        fprintf(stderr, "\rraytracing (%d rays per pixel) : %4.1f %%", ray_per_pixel, progression);
        cout << "raytracing % :" << ray_per_pixel << "progression : " << progression << endl;

        // it�ration sur les colonnes de pixels
        for (x = 0; x < image_width; ++x)
        {
            // d�terminer l'index du pixel
            index = (image_height - y - 1) * image_width + x;

            // it�ration sur les rang�es du bloc de 2x2 �chantillons
            for (sy = 0; sy < 2; ++sy)
            {
                // it�ration sur les colonnes du bloc de 2x2 �chantillons
                for (sx = 0; sx < 2; ++sx)
                {
                    // initialiser la radiance
                    radiance = Vector();

                    // it�ration des sur les rayons par pixel
                    for (s = 0; s < ray_per_pixel; ++s)
                    {
                        // filtre de la tente
                        r1 = 2.0 * random01(rng);
                        dx = r1 < 1.0 ? sqrt(r1) - 1.0 : 1.0 - sqrt(2.0 - r1);

                        r2 = 2.0 * random01(rng);
                        dy = r2 < 1.0 ? sqrt(r2) - 1.0 : 1.0 - sqrt(2.0 - r2);

                        // calculer la distance de l'�chantillon
                        distance = camera.axis_x * (((sx + 0.5 + dx) / 2.0 + x) / image_width - 0.5) +
                            camera.axis_y * (((sy + 0.5 + dy) / 2.0 + y) / image_height - 0.5) + camera.axis_z;

                        // appel r�cursif du calcul de la radiance
                        radiance = radiance + compute_radiance(Ray(camera.position + distance * 140, distance.normalize()), 0) * (1.0 / ray_per_pixel);
                    }

                    image.pixel[index] = image.pixel[index] + Vector(clamp(radiance.x), clamp(radiance.y), clamp(radiance.z)) * 0.25;
                }
            }
        }
    }

    std::cout << "\nrender done" << std::endl;
}

/*

// fonction qui permet de sauvegarder des pixels dans un fichier image (format .ppm)
void Rayons::save_image_file(int width, int height, int ray_per_pixel, const Vector* pixel)
{
    // nom du fichier image de type .ppm (portable pixmap)
    std::stringstream ss;
    ss << "image" << width << "x" << height << "_" << ray_per_pixel << ".ppm";
    std::string filename = ss.str();

    // d�claration et ouverture du fichier en mode �criture
    std::ofstream file;
    file.open(filename, std::ios::out);

    // ent�te du ficher pour une image avec un espace de couleur RGB 24 bits (P3 pour pixmap)
    file << "P3\n";

    // largeur et hauteur de l'image sur la seconde ligne de l'ent�te
    file << width << ' ' << height << '\n';

    // valeur maximale de l'espace de couleur sur la troisi�me ligne de l'ent�te
    file << "255\n";

    // �criture des pixels dans le fichier image
    for (int index = 0; index < width * height; ++index)
    {
        // �crire la couleur du pixel dans le fichier image
        file << static_cast<std::uint32_t>(format_color_component(pixel[index].x)) << " ";
        file << static_cast<std::uint32_t>(format_color_component(pixel[index].y)) << " ";
        file << static_cast<std::uint32_t>(format_color_component(pixel[index].z)) << " ";
    }

    // fermeture du fichier
    file.close();
}

*/
