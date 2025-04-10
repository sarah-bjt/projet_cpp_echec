#include <glad/glad.h>
#include <imgui.h>
#include <chrono>
#include <functional>
#include <iostream>
#include <random>
#include "3D/Camera.hpp" // Inclure la classe Camera
#include "3D/Model3D.hpp"
#include "3D/Renderer3D.hpp"
#include "board.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/fwd.hpp"
#include "glm/glm.hpp"
#include "maths.hpp"
#include "quick_imgui/quick_imgui.hpp"

int main()
{
    // Créer un générateur de nombres aléatoires avec graine par défaut
    RandomDistributions random;

    // Créer une instance de Board et Renderer3D
    Board      board;
    Renderer3D renderer;

    // Initialiser la caméra avec une position (ici, 3 unités en avant sur l'axe Z)
    Camera camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);

    // Variable pour calculer le deltaTime
    float lastFrame = 0.0f;

    quick_imgui::loop(
        "Chess 3D",
        {
            // Initialisation
            .init = [&]() {
                board.init();
                renderer.init();
                glEnable(GL_DEPTH_TEST); // Activer le test de profondeur
            },

            // Boucle principale
            .loop = [&]() {
                // Calcul du temps écoulé depuis le dernier frame pour le deltaTime
                float currentFrame = static_cast<float>(glfwGetTime());
                float deltaTime = currentFrame - lastFrame;
                lastFrame = currentFrame;

                // 🎮 Mouvements de la caméra (clavier)
                camera.processKeyboard(glfwGetCurrentContext(), deltaTime);

                // Début de la fenêtre ImGui pour afficher le plateau 2D
                ImGui::Begin("Chess 2D");
                board.render();
                ImGui::End();

                // Matrices de transformation
                glm::mat4 projection = glm::perspective(glm::radians(camera.getZoom()), 1280.f / 720.f, 0.1f, 100.0f);
                glm::mat4 view = camera.getViewMatrix();

                // Rendu 3D
                renderer.render(projection, glfwGetCurrentContext(), deltaTime, camera); },

            // Callbacks pour les événements de clavier, souris et redimensionnement de fenêtre
            .key_callback = [](int key, int scancode, int action, int mods) { std::cout << "Key: " << key << " Scancode: " << scancode
                                                                                        << " Action: " << action << " Mods: " << mods << '\n'; },

            .mouse_button_callback = [](int button, int action, int mods) { std::cout << "Button: " << button << " Action: " << action
                                                                                      << " Mods: " << mods << '\n'; },

            .cursor_position_callback = [&](double xpos, double ypos) {
                // Traiter le mouvement de la souris pour la caméra
                camera.processMouseMovement(xpos, ypos); // Mise à jour de la caméra avec les nouveaux mouvements
            },

            .scroll_callback = [&](double xoffset, double yoffset) {
                // Traiter le défilement pour le zoom
                camera.processMouseScroll(yoffset); // Mise à jour du zoom de la caméra
            },

            .window_size_callback = [](int width, int height) { std::cout << "Resized: " << width << ' ' << height << '\n'; },
        }
    );

    return 0;
}
