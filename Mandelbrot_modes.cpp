#include "CImg.h" // Bibliothèque graphique
#include <complex> // Pour les nombres complexes
#include <iostream> // Pour les entrées/sorties
#include <omp.h>    // Pour OpenMP

using namespace cimg_library;
using namespace std;

// Fonction pour calculer si un point appartient à l'ensemble de Mandelbrot
int mandelbrot(const complex<double> &c, int max_iter) {
    complex<double> z = 0;
    int iter = 0;
    while (abs(z) <= 2.0 && iter < max_iter) {
        z = z * z + c; // la suite z pour un c complex du plan est elle bornée ?
        iter++;
    }
    return iter;
}

int main() {
    /// Temps Programme

    double t_debut = omp_get_wtime();

    /// Paramètres

    // Image
    int width = 2*800;
    int height = 2*600;
    double x_min = -2.0, x_max = 1.0; // Intervalle sur l'axe des x
    double y_min = -1.0, y_max = 1.0; // Intervalle sur l'axe des y
    int max_iter = 1000;              // Nombre maximum d'itérations
    int num_threads = 8;             // Nombre de threads OpenMP

    // Configuration d'OpenMP
    omp_set_num_threads(num_threads);

    // Création de l'image
    CImg<unsigned char> img(width, height, 1, 3, 0); // Image RGB, noir par défaut

    // Génération de l'ensemble de Mandelbrot
    double dx = (x_max - x_min) / width;  // Résolution sur l'axe des x
    double dy = (y_max - y_min) / height; // Résolution sur l'axe des y

    /// Calcul

    //cimg::openmp_set(num_threads); // Utiliser le même nombre de threads qu'OpenMP
    for (int mode = 0; mode <= 2; ++mode) {
        cimg::openmp_mode(mode); // Définir le mode OpenMP pour CImg
        double t_debut = omp_get_wtime();

        #pragma omp parallel for collapse(2) schedule(dynamic)
        for (int px = 0; px < width; ++px) {
            for (int py = 0; py < height; ++py) {
                // Conversion des coordonnées pixel en coordonnées complexes
                double x = x_min + px * dx;
                double y = y_min + py * dy;
                complex<double> c(x, y);

                // Calcul de l'ensemble de Mandelbrot
                int iter = mandelbrot(c, max_iter);

                // Couleur en fonction du nombre d'itérations
                unsigned char color[3] = {
                    (unsigned char)(255 * iter / max_iter), // Rouge
                    (unsigned char)(255 * iter / max_iter), // Vert
                    (unsigned char)(255 * iter / max_iter)  // Bleu
                };

                img.draw_point(px, py, color);
            }
        }
        double t_fin = omp_get_wtime();
        printf("Mode OpenMP : %d, Temps d'exécution : %lf\n", mode, t_fin - t_debut);
}


    /// Temps d'execution.

    double t_fin = omp_get_wtime();
    printf("Temps d'execution : %lf\n",t_fin-t_debut);

    // Affichage et sauvegarde de l'image
    img.display("Mandelbrot Set");
    img.save("mandelbrot.bmp");

    return 0;
}
