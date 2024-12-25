# OpenMP Fractales

Ce dépôt contient trois implémentations de l'ensemble de Mandelbrot ainsi que leurs exécutables :

- **Mandelbrot.cpp** & **Mandelbrot** : Implémentation avec `#pragma omp for collapse(2)` et `critical` (que l'on peut éventuellement commenter).
- **Mandelbrot_lock.cpp** & **Mandelbrot_lock** : Implémentation avec les verrous.
- **Mandelbrot_modes.cpp** & **Mandelbrot_modes** : Implémentation avec les fonctions d'OpenMP.

---

## Programmes supplémentaires

Ce dépôt contient également deux programmes supplémentaires pour l'affichage des fractales en couleur :

- **Mandelbrot_colored.cpp** & **Mandelbrot_colored** : Affichage de l'ensemble de Mandelbrot.
- **fractales.cpp** & **fractales** : Permet de choisir entre 4 fractales de l'ensemble de Julia directement depuis le terminal.

---

## Images associées

Pour chaque programme, des images BMP associées à des résolutions "hautes" ont été générées.

---

## Paramètres modifiables

Chaque script contient des variables permettant de modifier :
- La résolution.
- Le nombre de threads actifs.
