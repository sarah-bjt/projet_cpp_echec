// maths.cpp

#include "maths.hpp"

// Constructeur par défaut (initialise avec le temps actuel)
RandomDistributions::RandomDistributions()
    : hasSpareNormal(false), spareNormal(0.0)
{
    seed = static_cast<unsigned>(time(nullptr));
    srand(seed);
}

// Constructeur avec graine spécifiée
RandomDistributions::RandomDistributions(unsigned seed)
    : seed(seed), hasSpareNormal(false), spareNormal(0.0)
{
    srand(seed);
}

// Variable globale accessible depuis n'importe où
RandomDistributions globalRandom;

// Génère un nombre aléatoire entre 0 et 1
double RandomDistributions::getUniformUnit()
{
    return static_cast<double>(rand()) / RAND_MAX;
}

// ------------------ Lois discrètes ------------------

// 1. Distribution uniforme discrète sur [min, max]
int RandomDistributions::uniformDiscrete(int min, int max)
{
    return min + rand() % (max - min + 1);
}

// 2. Distribution de Bernoulli (p = probabilité de succès)
bool RandomDistributions::bernoulli(double p)
{
    if (p < 0.0)
        p = 0.0;
    if (p > 1.0)
        p = 1.0;

    return getUniformUnit() < p;
}

// 3. Distribution binomiale (n essais, p = probabilité de succès)
int RandomDistributions::binomial(int n, double p)
{
    if (p < 0.0)
        p = 0.0;
    if (p > 1.0)
        p = 1.0;
    if (n < 0)
        n = 0;

    // Méthode directe: simuler n essais de Bernoulli
    int successes = 0;
    for (int i = 0; i < n; i++)
    {
        if (bernoulli(p))
        {
            successes++;
        }
    }
    return successes;
}

// 4. Distribution géométrique (p = probabilité de succès)
int RandomDistributions::geometric(double p)
{
    if (p <= 0.0 || p > 1.0)
    {
        return 0; // Valeur par défaut pour p invalide
    }

    // Calculer le nombre d'échecs avant le premier succès
    // Utilisant la formule: log(U)/log(1-p) où U est uniforme(0,1)
    double u = getUniformUnit();
    while (u == 0.0)
    { // Éviter log(0)
        u = getUniformUnit();
    }

    return static_cast<int>(std::log(u) / std::log(1.0 - p)) + 1;
}

// ------------------ Lois continues ------------------

// 5. Distribution uniforme continue sur [min, max]
double RandomDistributions::uniformContinuous(double min, double max)
{
    return min + (max - min) * getUniformUnit();
}

// 6. Distribution exponentielle (lambda = paramètre)
double RandomDistributions::exponential(double lambda)
{
    if (lambda <= 0.0)
        return 0.0; // Protection contre les valeurs invalides

    // Méthode d'inversion: -ln(U)/lambda où U est uniforme(0,1)
    double u = getUniformUnit();
    while (u == 0.0)
    { // Éviter log(0)
        u = getUniformUnit();
    }

    return -std::log(u) / lambda;
}

// 7. Distribution normale (moyenne, écart-type) - méthode Box-Muller
double RandomDistributions::normal(double mean, double stddev)
{
    // Si on a déjà une valeur en réserve, on la retourne
    if (hasSpareNormal)
    {
        hasSpareNormal = false;
        return mean + stddev * spareNormal;
    }

    // Sinon, on génère deux nouvelles valeurs
    double u, v, s;
    do
    {
        u = 2.0 * getUniformUnit() - 1.0; // Uniforme sur [-1,1]
        v = 2.0 * getUniformUnit() - 1.0; // Uniforme sur [-1,1]
        s = u * u + v * v;
    } while (s >= 1.0 || s == 0.0);

    // Transformer en distribution normale standard
    s              = std::sqrt(-2.0 * std::log(s) / s);
    spareNormal    = v * s; // On garde la seconde valeur pour le prochain appel
    hasSpareNormal = true;

    return mean + stddev * u * s;
}

// Fonction factorielle pour les calculs
double RandomDistributions::factorial(int n)
{
    if (n <= 1)
        return 1.0;
    double result = 1.0;
    for (int i = 2; i <= n; i++)
    {
        result *= i;
    }
    return result;
}

// 8. Distribution gamma (k = shape, theta = scale)
double RandomDistributions::gamma(double k, double theta)
{
    // Méthode basée sur l'algorithme de Marsaglia et Tsang (2000)
    if (k <= 0.0 || theta <= 0.0)
        return 0.0; // Protection contre les valeurs invalides

    // Pour k < 1, on utilise la relation gamma(k,1) = gamma(k+1,1) * U^(1/k)
    if (k < 1.0)
    {
        double u = getUniformUnit();
        return gamma(k + 1.0, theta) * std::pow(u, 1.0 / k);
    }

    // Pour k >= 1
    const double d = k - 1.0 / 3.0;
    const double c = 1.0 / std::sqrt(9.0 * d);

    while (true)
    {
        double x, v;
        double u;

        do
        {
            // Générer une variable normale standard
            x = normal(0.0, 1.0);
            v = 1.0 + c * x;
        } while (v <= 0.0);

        v = v * v * v; // v^3
        u = getUniformUnit();

        // Teste d'acceptation/rejet
        if (u < 1.0 - 0.0331 * x * x * x * x)
        {
            return d * v * theta;
        }

        if (std::log(u) < 0.5 * x * x + d * (1.0 - v + std::log(v)))
        {
            return d * v * theta;
        }
    }
}

// // Méthode pour générer et afficher plusieurs échantillons
// void RandomDistributions::generateSamples(int count)
// {
//     std::cout << "\n==== Échantillons des 8 distributions (n=" << count << ") ====\n";

//     std::cout << "\n1. Distribution uniforme discrète [1, 6]:\n";
//     for (int i = 0; i < count; ++i)
//     {
//         std::cout << uniformDiscrete(1, 6) << " ";
//     }

//     std::cout << "\n\n2. Distribution de Bernoulli (p=0.3):\n";
//     for (int i = 0; i < count; ++i)
//     {
//         std::cout << (bernoulli(0.3) ? "1" : "0") << " ";
//     }

//     std::cout << "\n\n3. Distribution binomiale (n=10, p=0.3):\n";
//     for (int i = 0; i < count; ++i)
//     {
//         std::cout << binomial(10, 0.3) << " ";
//     }

//     std::cout << "\n\n4. Distribution géométrique (p=0.2):\n";
//     for (int i = 0; i < count; ++i)
//     {
//         std::cout << geometric(0.2) << " ";
//     }

//     std::cout << "\n\n5. Distribution uniforme continue [0, 1]:\n";
//     for (int i = 0; i < count; ++i)
//     {
//         std::cout << uniformContinuous(0, 1) << " ";
//     }

//     std::cout << "\n\n6. Distribution exponentielle (lambda=2):\n";
//     for (int i = 0; i < count; ++i)
//     {
//         std::cout << exponential(2.0) << " ";
//     }

//     std::cout << "\n\n7. Distribution normale (mean=0, stddev=1):\n";
//     for (int i = 0; i < count; ++i)
//     {
//         std::cout << normal(0.0, 1.0) << " ";
//     }

//     std::cout << "\n\n8. Distribution gamma (k=2.0, theta=1.5):\n";
//     for (int i = 0; i < count; ++i)
//     {
//         std::cout << gamma(2.0, 1.5) << " ";
//     }

//     std::cout << std::endl;
// }
