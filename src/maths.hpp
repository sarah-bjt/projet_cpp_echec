// #ifndef RANDOM_DISTRIBUTIONS_H
// #define RANDOM_DISTRIBUTIONS_H

#pragma once

#include <cmath>   // Pour les calculs mathématiques
#include <cstdlib> // Pour rand() et srand()
#include <ctime>   // Pour time()
#include <iostream>
#include <vector>

class RandomDistributions {
public:
    // Constructeur avec initialisation de la graine par défaut
    RandomDistributions();

    // Constructeur avec graine spécifiée
    RandomDistributions(unsigned seed);

    // Fonction utilitaire pour générer un nombre aléatoire entre 0 et 1
    double getUniformUnit();

    // ------------------ Lois discrètes ------------------

    // 1. Distribution uniforme discrète sur [min, max]
    int uniformDiscrete(int min, int max);

    // 2. Distribution de Bernoulli (p = probabilité de succès)
    bool bernoulli(double p);

    // 3. Distribution binomiale (n essais, p = probabilité de succès)
    int binomial(int n, double p);

    // 4. Distribution géométrique (p = probabilité de succès)
    int geometric(double p);

    // ------------------ Lois continues ------------------

    // 5. Distribution uniforme continue sur [min, max]
    double uniformContinuous(double min, double max);

    // 6. Distribution exponentielle (lambda = paramètre)
    double exponential(double lambda);

    // 7. Distribution normale (moyenne, écart-type) - méthode Box-Muller
    double normal(double mean, double stddev);

    // 8. Distribution gamma (k = shape, theta = scale)
    double gamma(double k, double theta);

    // Méthodes pour générer et afficher plusieurs échantillons
    void generateSamples(int count);

private:
    unsigned seed;
    // Variable pour stocker la deuxième valeur normale générée par Box-Muller
    bool   hasSpareNormal;
    double spareNormal;

    // Fonction factorielle pour les calculs
    double factorial(int n);
};

// Déclaration de la variable globale
extern RandomDistributions globalRandom;

// #endif // RANDOM_DISTRIBUTIONS_H