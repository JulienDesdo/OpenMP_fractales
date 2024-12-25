#include "CImg.h" // Bibliothèque graphique
#include <complex> // Pour les nombres complexes
#include <iostream> // Pour les entrées/sorties
#include <omp.h>    // Pour OpenMP

using namespace cimg_library;
using namespace std;

// Fonction pour calculer si un point appartient à Julia et ses variantes
int julia(const complex<double>& z0, const complex<double>& c, int max_iter) {
    complex<double> z = z0;
    int iter = 0;
    while (abs(z) <= 2.0 && iter < max_iter) {
        z = z * z + c;
        iter++;
    }
    return iter;
}

int main() {
    // Menu interactif pour choisir la fractale
    int fractal_choice;
    cout << "Choisissez une fractale Julia à afficher :\n"
         << "1. Julia classique (z² - 0.7 + 0.27015i)\n"
         << "2. Lapin de Douady (z² - 0.12 + 0.75i)\n"
         << "3. Fractale personnalisée (z² - 0.39 - 0.59i)\n"
         << "4. Neurone (z² - i)\n"
         << "Votre choix : ";
    cin >> fractal_choice;

    // Paramètres par défaut
    int width = 10*800;   // Largeur de l'image
    int height = 10*600;  // Hauteur de l'image
    double x_min = -2.0, x_max = 2.0; // Intervalle sur l'axe des x
    double y_min = -1.5, y_max = 1.5; // Intervalle sur l'axe des y
    int max_iter = 1000;              // Nombre maximum d'itérations
    int num_threads = 20;              // Nombre de threads OpenMP
    complex<double> c(0.0, 0.0);      // Constante c par défaut

    // Configuration spécifique pour chaque fractale Julia
    switch (fractal_choice) {
    case 1: // Julia classique
        c = complex<double>(-0.7, 0.27015);
        break;
    case 2: // Lapin de Douady
        c = complex<double>(-0.12, 0.75);
        break;
    case 3: // Fractale personnalisée
        c = complex<double>(-0.39, -0.59);
        break;
    case 4: // Neurone
        c = complex<double>(0.0, -1.0);
        break;
    default:
        cout << "Choix invalide. Julia classique sera affichée par défaut.\n";
        c = complex<double>(-0.7, 0.27015);
        break;
    }

    // Configuration d'OpenMP
    omp_set_num_threads(num_threads);

    // Création de l'image
    CImg<unsigned char> img(width, height, 1, 3, 0); // Image RGB, noir par défaut

    // Génération de la fractale Julia
    double dx = (x_max - x_min) / width;  // Résolution sur l'axe des x
    double dy = (y_max - y_min) / height; // Résolution sur l'axe des y

    #pragma omp parallel for collapse(2) schedule(dynamic)
    for (int px = 0; px < width; ++px) {
        for (int py = 0; py < height; ++py) {
            // Conversion des coordonnées pixel en coordonnées complexes
            double x = x_min + px * dx;
            double y = y_min + py * dy;
            complex<double> z0(x, y);

            // Calcul de la fractale Julia
            int iter = julia(z0, c, max_iter);

            // Couleur en fonction du nombre d'itérations
            unsigned char color[3];
            if (iter == max_iter) {
                color[0] = color[1] = color[2] = 0; // Noir pour les points internes
            } else {
                double t = (double)iter / max_iter;
                color[0] = (unsigned char)(9 * (1 - t) * t * t * t * 255); // Rouge
                color[1] = (unsigned char)(15 * (1 - t) * (1 - t) * t * t * 255); // Vert
                color[2] = (unsigned char)(8.5 * (1 - t) * (1 - t) * (1 - t) * t * 255); // Bleu
            }

            // Dessiner le point dans l'image
            img.draw_point(px, py, color);
        }
    }

    // Affichage et sauvegarde de l'image
    string filename = "julia_";
    switch (fractal_choice) {
        case 1: filename += "classique.bmp"; break;
        case 2: filename += "douady.bmp"; break;
        case 3: filename += "custom.bmp"; break;
        case 4: filename += "neurone.bmp"; break;
        default: filename += "default.bmp"; break;
    }

    img.display("Fractale Julia");
    img.save(filename.c_str());

    return 0;
}
