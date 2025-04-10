#include "maths.hpp"

RandomDistributions::RandomDistributions()
    : hasSpareNormal(false), spareNormal(0.0)
{
    seed = static_cast<unsigned>(time(nullptr));
    srand(seed);
}

double RandomDistributions::getUniformUnit()
{
    return static_cast<double>(rand()) / RAND_MAX;
}

// ------------------ Lois discrètes ------------------

// distribution uniforme discrète sur [min, max]
int RandomDistributions::uniformDiscrete(int min, int max)
{
    return min + rand() % (max - min + 1);
}

// distribution de Bernoulli (p = probabilité de succès)
bool RandomDistributions::bernoulli(double p)
{
    if (p < 0.0)
        p = 0.0;
    if (p > 1.0)
        p = 1.0;

    return getUniformUnit() < p;
}

// distribution binomiale (n essais, p = probabilité de succès)
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

// distribution géométrique (p = probabilité de succès)
int RandomDistributions::geometric(double p)
{
    if (p <= 0.0 || p > 1.0)
    {
        return 0; //  p invalide
    }

    double u = getUniformUnit();
    while (u == 0.0)
    {
        u = getUniformUnit();
    }

    return static_cast<int>(std::log(u) / std::log(1.0 - p)) + 1;
}

// ------------------ Lois continues ------------------

// distribution uniforme continue sur [min, max]
double RandomDistributions::uniformContinuous(double min, double max)
{
    return min + (max - min) * getUniformUnit();
}

// distribution exponentielle (lambda = paramètre)
double RandomDistributions::exponential(double lambda)
{
    if (lambda <= 0.0)
        return 0.0; // Protection contre les valeurs invalides

    // méthode d'inversion
    double u = getUniformUnit();
    while (u == 0.0)
    { // Éviter log(0)
        u = getUniformUnit();
    }

    return -std::log(u) / lambda;
}

// distribution normale (moyenne, écart-type) - méthode Box-Muller
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

// fonction factorielle pour les calculs
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

// distribution gamma (k = shape, theta = scale)
double RandomDistributions::gamma(double k, double theta)
{
    // algorithme de Marsaglia et Tsang
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

// Variable globale accessible depuis n'importe où
RandomDistributions globalRandom;