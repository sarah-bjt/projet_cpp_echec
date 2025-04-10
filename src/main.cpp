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

// Pour faire un push nul

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

    // Initialisation d'ImGui et des autres composants
    quick_imgui::loop(
        "Chess 3D",
        {
            // Initialisation
            .init = [&]() {
                board.init();
                renderer.init();
                glEnable(GL_DEPTH_TEST); // Activer le test de profondeur
                glfwSetWindowSizeLimits(glfwGetCurrentContext(), 840, 720,840, 720); },

            // Boucle principale
            .loop = [&]() {
                // Calcul du temps écoulé depuis le dernier frame pour le deltaTime
                float currentFrame = static_cast<float>(glfwGetTime());
                float deltaTime    = currentFrame - lastFrame;
                lastFrame          = currentFrame;

                // 🎮 Mouvements de la caméra (clavier)
                camera.processMovement(deltaTime); // Traite les déplacements de la caméra

                // Début de la fenêtre ImGui pour afficher le plateau 2D
                ImGui::SetNextWindowPos(ImVec2(850, 20), ImGuiCond_Once);
                ImGui::SetNextWindowSize(ImVec2(500, 720), ImGuiCond_Once);
                ImGui::Begin("Chess 2D");
                board.render();
                ImGui::End();

                // Matrices de transformation
                glm::mat4 projection = glm::perspective(glm::radians(camera.getZoom()), 1280.f / 720.f, 0.1f, 100.0f);
                glm::mat4 view       = camera.getViewMatrix();

                // Rendu 3D
                renderer.render(projection, glfwGetCurrentContext(), deltaTime, camera); // Passer window
            },

            // Callbacks pour les événements de clavier, souris et redimensionnement de fenêtre
            .key_callback = [&camera](int key, int scancode, int action, int mods) {
                // Mise à jour de l'état des touches dans la caméra
                if (action == GLFW_PRESS || action == GLFW_REPEAT) {
                    camera.keyState(key, true);  // Appuyer sur la touche
                } else if (action == GLFW_RELEASE) {
                    camera.keyState(key, false); // Relâcher la touche
                } },

            .mouse_button_callback = [](int button, int action, int mods) {
                // Nous n'avons plus besoin de gérer les événements de souris ici.
                // Tu peux soit laisser ce callback vide, soit supprimer cette fonction
                // si tu ne souhaites pas gérer les clics de souris du tout.
            },

            // Suppression du callback de mouvement de souris (nous n'avons plus besoin de ça)
            .cursor_position_callback = [](double xpos, double ypos) {
                // Ne rien faire ici
            },

            .window_size_callback = [](int width, int height) { std::cout << "Resized: " << width << ' ' << height << '\n'; },
        }
    );

    return 0;
}
