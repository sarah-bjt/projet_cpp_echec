#include <imgui.h>
#include <chrono>
#include <functional>
#include <iostream>
#include <random>
#include "glm/glm.hpp"
#include <iostream>
#include "lib/glmax/src/Camera.hpp"
#include "Model3D.hpp"
#include "board.hpp"
#include "glad/glad.h"
#include "maths.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/fwd.hpp"
#include "quick_imgui/quick_imgui.hpp"

#include "Renderer3D.hpp" // 🔹 Ajouté

int main()
{
    // Créer un générateur de nombres aléatoires avec graine par défaut
    RandomDistributions random;

    Board board;
    // Board board;
    Renderer3D renderer; // 🔹 Création du renderer 3D

    quick_imgui::loop(
        "Chess 3D",
        {
            .init = [&]() {
                // board.init();
                renderer.init(); // 🔹 Initialisation du renderer
            },
            .loop = [&]() {
                // 🔸 Partie ImGui désactivée pour le moment
                ImGui::Begin("Chess 2D");
                board.render();
                ImGui::End();

                // 🔹 Matrices de projection et vue
                glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1280.f / 720.f, 0.1f, 100.0f);
                glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f),
                                             glm::vec3(0.0f, 0.0f, 0.0f),
                                             glm::vec3(0.0f, 1.0f, 0.0f));

                renderer.render(projection, view); // 🔹 Rendu de la skybox
            },
            .key_callback = [](int key, int scancode, int action, int mods) {
                std::cout << "Key: " << key << " Scancode: " << scancode
                          << " Action: " << action << " Mods: " << mods << '\n';
            },
            .mouse_button_callback = [](int button, int action, int mods) {
                std::cout << "Button: " << button << " Action: " << action
                          << " Mods: " << mods << '\n';
            },
            .cursor_position_callback = [](double xpos, double ypos) {
                std::cout << "Position: " << xpos << ' ' << ypos << '\n';
            },
            .scroll_callback = [](double xoffset, double yoffset) {
                std::cout << "Scroll: " << xoffset << ' ' << yoffset << '\n';
            },
            .window_size_callback = [](int width, int height) {
                std::cout << "Resized: " << width << ' ' << height << '\n';
            },
        }
    );
    return 0;
}