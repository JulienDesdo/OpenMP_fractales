#define cimg_use_openmp // Activer OpenMP pour CImg
#include "CImg.h"       // Bibliothèque graphique
#include <complex>      // Pour les nombres complexes
#include <vector>       // Pour les palettes de couleurs
#include <iostream>     // Pour les entrées/sorties
#include <omp.h>        // Pour OpenMP

using namespace cimg_library;
using namespace std;

// Fonction pour calculer si un point appartient à l'ensemble de Mandelbrot
int mandelbrot(const complex<double> &c, int max_iter) {
    complex<double> z = 0;
    int iter = 0;
    while (abs(z) <= 2.0 && iter < max_iter) {
        z = z * z + c; // La suite z pour un c complex du plan est-elle bornée ?
        iter++;
    }
    return iter;
}

// Fonction pour générer une palette de couleurs
vector<unsigned char> generate_palette(int max_iter) {
    vector<unsigned char> palette(max_iter * 3); // 3 pour RGB
    for (int i = 0; i < max_iter; ++i) {
        double t = (double)i / max_iter; // Normalisation entre 0 et 1
        palette[i * 3 + 0] = (unsigned char)(9 * (1 - t) * t * t * t * 255); // Rouge
        palette[i * 3 + 1] = (unsigned char)(15 * (1 - t) * (1 - t) * t * t * 255); // Vert
        palette[i * 3 + 2] = (unsigned char)(8.5 * (1 - t) * (1 - t) * (1 - t) * t * 255); // Bleu
    }
    return palette;
}

int main() {
    /// Temps Programme
    double t_debut = omp_get_wtime();

    /// Paramètres
    int width = 5*1600; // Largeur de l'image
    int height = 5*1200; // Hauteur de l'image
    double x_min = -2.0, x_max = 1.0; // Intervalle sur l'axe des x
    double y_min = -1.0, y_max = 1.0; // Intervalle sur l'axe des y
    int max_iter = 1000;              // Nombre maximum d'itérations
    int num_threads = 16;             // Nombre de threads OpenMP

    // Configuration OpenMP
    omp_set_num_threads(num_threads);

    // Création de l'image
    CImg<unsigned char> img(width, height, 1, 3, 0); // Image RGB, noir par défaut

    // Génération de la palette de couleurs
    vector<unsigned char> palette = generate_palette(max_iter);

    // Génération de l'ensemble de Mandelbrot
    double dx = (x_max - x_min) / width;  // Résolution sur l'axe des x
    double dy = (y_max - y_min) / height; // Résolution sur l'axe des y

    #pragma omp parallel for collapse(2) schedule(dynamic)
    for (int px = 0; px < width; ++px) {
        for (int py = 0; py < height; ++py) {
            // Conversion des coordonnées pixel en coordonnées complexes
            double x = x_min + px * dx;
            double y = y_min + py * dy;
            complex<double> c(x, y);

            // Calcul de l'ensemble de Mandelbrot
            int iter = mandelbrot(c, max_iter);

            // Si le point appartient à l'ensemble (atteint max_iter), couleur noire
            if (iter == max_iter) {
                unsigned char black[3] = {0, 0, 0};
                img.draw_point(px, py, black);
            } else {
                // Couleurs de la palette en fonction de l'itération
                unsigned char color[3] = {
                    palette[iter * 3 + 0], // Rouge
                    palette[iter * 3 + 1], // Vert
                    palette[iter * 3 + 2]  // Bleu
                };
                img.draw_point(px, py, color);
            }
        }
    }

    /// Temps d'exécution
    double t_fin = omp_get_wtime();
    printf("Temps d'exécution : %lf secondes\n", t_fin - t_debut);

    // Affichage et sauvegarde de l'image
    img.display("Mandelbrot Set");
    img.save("mandelbrot_colored.bmp");

    return 0;
}
