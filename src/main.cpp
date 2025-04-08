#include <imgui.h>
#include <chrono>
#include <functional>
#include <iostream>
#include <random>
#include "board.hpp" // Inclure le fichier de la classe Board
#include "glad/glad.h"
#include "maths.hpp"
#include "quick_imgui/quick_imgui.hpp"

int main()
{
    // Créer un générateur de nombres aléatoires avec graine par défaut
    RandomDistributions random;

    // Pour une graine spécifique, décommentez la ligne suivante:
    // RandomDistributions random(12345);

    // Afficher des échantillons de chaque distribution
    // random.generateSamples(10);

    // Démontrer l'utilisation individuelle de chaque distribution
    std::cout << "\n==== Utilisation individuelle des distributions ====\n";

    // 1. Uniforme discrète
    std::cout << "\nJet d'un dé (uniforme discrète [1,6]): " << random.uniformDiscrete(1, 6) << std::endl;

    // 2. Bernoulli
    std::cout << "Lancer une pièce pipée (p=0.7): " << (random.bernoulli(0.7) ? "Face" : "Pile") << std::endl;

    // 3. Binomiale
    std::cout << "Nombre de faces en 20 lancers de pièce équilibrée: " << random.binomial(20, 0.5) << std::endl;

    // 4. Géométrique
    std::cout << "Nombre de tentatives avant un succès (p=0.25): " << random.geometric(0.25) << std::endl;

    // 5. Uniforme continue
    std::cout << "Nombre réel aléatoire entre 10 et 20: " << random.uniformContinuous(10, 20) << std::endl;

    // 6. Exponentielle
    std::cout << "Temps d'attente (distribution exponentielle, lambda=0.5): "
              << random.exponential(0.5) << " minutes" << std::endl;

    // 7. Normale
    std::cout << "Taille aléatoire (normale, moyenne=170cm, écart-type=10cm): "
              << random.normal(170, 10) << " cm" << std::endl;

    // 8. Gamma
    std::cout << "Durée d'un processus (gamma, shape=2, scale=1): "
              << random.gamma(2, 1) << " heures" << std::endl;

    // Calculer et afficher quelques statistiques
    std::cout << "\n==== Statistiques pour 1000 échantillons ====\n";

    // Normale
    double somme = 0.0, sommeCarres = 0.0;
    int    n = 1000;
    for (int i = 0; i < n; i++)
    {
        double val = random.normal(100, 15);
        somme += val;
        sommeCarres += val * val;
    }
    double moyenne  = somme / n;
    double variance = sommeCarres / n - moyenne * moyenne;
    std::cout << "Distribution normale (théorique: µ=100, σ=15):" << std::endl;
    std::cout << "  Moyenne empirique: " << moyenne << std::endl;
    std::cout << "  Écart-type empirique: " << std::sqrt(variance) << std::endl;

    // Exponentielle
    somme         = 0.0;
    sommeCarres   = 0.0;
    double lambda = 2.0;
    for (int i = 0; i < n; i++)
    {
        double val = random.exponential(lambda);
        somme += val;
        sommeCarres += val * val;
    }
    moyenne  = somme / n;
    variance = sommeCarres / n - moyenne * moyenne;
    std::cout << "Distribution exponentielle (théorique: µ=1/λ=" << 1 / lambda << ", σ=1/λ=" << 1 / lambda << "):" << std::endl;
    std::cout << "  Moyenne empirique: " << moyenne << std::endl;
    std::cout << "  Écart-type empirique: " << std::sqrt(variance) << std::endl;

    Board board;

    quick_imgui::loop(
        "Chess",
        {
            .init = [&]() { board.init(); },
            .loop =
                [&]() {
                    // ImGui::ShowDemoWindow();
                    ImGui::Begin("Chess 2D");
                    board.render();
                    ImGui::End();
                },
            // .key_callback             = [](int key, int scancode, int action, int mods) { std::cout << "Key: " << key << " Scancode: " << scancode << " Action: " << action << " Mods: " << mods << '\n'; },
            // .mouse_button_callback    = [](int button, int action, int mods) { std::cout << "Button: " << button << " Action: " << action << " Mods: " << mods << '\n'; },
            // .cursor_position_callback = [](double xpos, double ypos) { std::cout << "Position: " << xpos << ' ' << ypos << '\n'; },
            // .scroll_callback          = [](double xoffset, double yoffset) { std::cout << "Scroll: " << xoffset << ' ' << yoffset << '\n'; },
            // .window_size_callback     = [](int width, int height) { std::cout << "Resized: " << width << ' ' << height << '\n'; },
        }
    );
    return 0;
}
