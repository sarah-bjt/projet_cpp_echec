// #include <imgui.h>
// #include <iostream>
// #include "quick_imgui/quick_imgui.hpp"
// #include <backends/imgui_impl_glfw.h>
// #include <backends/imgui_impl_opengl3.h>
// #include <GLFW/glfw3.h>
// #include "board.hpp"

// GLFWwindow* window;

// void setup_imgui(GLFWwindow* window) {
//     IMGUI_CHECKVERSION();
//     ImGui::CreateContext();
//     ImGuiIO& io = ImGui::GetIO(); (void)io;
//     ImGui::StyleColorsDark();
    
//     // Setup Platform/Renderer bindings
//     ImGui_ImplGlfw_InitForOpenGL(window, true);
//     ImGui_ImplOpenGL3_Init("#version 130");
// }

// void cleanup_imgui() {
//     ImGui_ImplOpenGL3_Shutdown();
//     ImGui_ImplGlfw_Shutdown();
//     ImGui::DestroyContext();
// }

// int main() {
//     // Initialisation de GLFW
//     if (!glfwInit()) {
//         std::cerr << "Échec de l'initialisation de GLFW!" << std::endl;
//         return -1;
//     }

//     window = glfwCreateWindow(800, 600, "Plateau d'échecs IMGui", nullptr, nullptr);
//     if (window == nullptr) {
//         std::cerr << "Échec de la création de la fenêtre GLFW!" << std::endl;
//         glfwTerminate();
//         return -1;
//     }
//     glfwMakeContextCurrent(window);
//     glfwSwapInterval(1); // Activer le V-Sync

//     // Setup ImGui
//     setup_imgui(window);

//     // Créer un plateau d'échecs
//     Board board;

//     // Boucle de rendu
//     while (!glfwWindowShouldClose(window)) {
//         glfwPollEvents();
        
//         // Début du frame ImGui
//         ImGui_ImplOpenGL3_NewFrame();
//         ImGui_ImplGlfw_NewFrame();
//         ImGui::NewFrame();

//         // Afficher le plateau
//         board.render();

//         // Rendu des éléments ImGui
//         ImGui::Render();
//         glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
//         glClear(GL_COLOR_BUFFER_BIT);
//         ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

//         // Échanger les buffers
//         glfwSwapBuffers(window);
//     }

//     cleanup_imgui();
//     glfwDestroyWindow(window);
//     glfwTerminate();
//     return 0;
// }

// #include <imgui.h>
// #include <iostream>
// #include "quick_imgui/quick_imgui.hpp"

// int main()
// {
//     float value{0.f};

//     quick_imgui::loop(
//         "Chess",
//         {
//             .init = [&]() {},
//             .loop =
//                 [&]() {
//                     ImGui::ShowDemoWindow(); // This opens a window which shows tons of examples of what you can do with ImGui. You should check it out! Also, you can use the "Item Picker" in the top menu of that demo window: then click on any widget and it will show you the corresponding code directly in your IDE!

//                     ImGui::Begin("Example");

//                     ImGui::SliderFloat("My Value", &value, 0.f, 3.f);

//                     if (ImGui::Button("1", ImVec2{50.f, 50.f}))
//                         std::cout << "Clicked button 1\n";
//                     ImGui::SameLine(); // Draw the next ImGui widget on the same line as the previous one. Otherwise it would be below it

//                     ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{1.f, 0.f, 0.f, 1.f}); // Changes the color of all buttons until we call ImGui::PopStyleColor(). There is also ImGuiCol_ButtonActive and ImGuiCol_ButtonHovered

//                     ImGui::PushID(2); // When some ImGui items have the same label (for exemple the next two buttons are labeled "Yo") ImGui needs you to specify an ID so that it can distinguish them. It can be an int, a pointer, a string, etc.
//                                       // You will definitely run into this when you create a button for each of your chess pieces, so remember to give them an ID!
//                     if (ImGui::Button("Yo", ImVec2{50.f, 50.f}))
//                         std::cout << "Clicked button 2\n";
//                     ImGui::PopID(); // Then pop the id you pushed after you created the widget

//                     ImGui::SameLine();
//                     ImGui::PushID(3);
//                     if (ImGui::Button("Yo", ImVec2{50.f, 50.f}))
//                         std::cout << "Clicked button 3\n";
//                     ImGui::PopID();

//                     ImGui::PopStyleColor();

//                     ImGui::End();
//                 },
//         }
//     );
// }

// #include <imgui.h>
// #include <iostream>
// #include "quick_imgui/quick_imgui.hpp"
// #include <GLFW/glfw3.h>
// #include "board.hpp"

// int main() {
//     // Utilisation de quick_imgui::loop
//     quick_imgui::loop(
//         "Plateau d'échecs IMGui",  // Titre de la fenêtre
//         [&](quick_imgui::Settings& settings) {
//             // Initialisation de GLFW et création de la fenêtre
//             if (!glfwInit()) {
//                 std::cerr << "Échec de l'initialisation de GLFW!" << std::endl;
//                 exit(-1);
//             }

//             settings.window_width = 800;  // Largeur de la fenêtre
//             settings.window_height = 600; // Hauteur de la fenêtre
//             settings.window_title = "Plateau d'échecs IMGui";  // Titre de la fenêtre

//             // Créer la fenêtre
//             settings.window = glfwCreateWindow(settings.window_width, settings.window_height, settings.window_title, nullptr, nullptr);
//             if (settings.window == nullptr) {
//                 std::cerr << "Échec de la création de la fenêtre GLFW!" << std::endl;
//                 glfwTerminate();
//                 exit(-1);
//             }

//             glfwMakeContextCurrent(settings.window);
//             glfwSwapInterval(1); // Activer le V-Sync
//         },
//         [&](quick_imgui::Settings& settings) {
//             // Créer un plateau d'échecs
//             Board board;

//             // Début de la boucle ImGui (elle est gérée automatiquement dans quick_imgui::loop)
//             while (!glfwWindowShouldClose(settings.window)) {
//                 // Gérer les événements
//                 glfwPollEvents();

//                 // Début du frame ImGui
//                 ImGui_ImplOpenGL3_NewFrame();
//                 ImGui_ImplGlfw_NewFrame();
//                 ImGui::NewFrame();

//                 // Afficher le plateau d'échecs
//                 board.render();

//                 // Rendu d'ImGui
//                 ImGui::Render();
//                 glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
//                 glClear(GL_COLOR_BUFFER_BIT);
//                 ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

//                 // Échanger les buffers
//                 glfwSwapBuffers(settings.window);
//             }
//         }
//     );

//     return 0;
// }

#include <imgui.h>
#include <iostream>
#include "quick_imgui/quick_imgui.hpp"
#include "board.hpp"  // Inclure le fichier de la classe Board

int main()
{
    Board board;  // Création de l'objet du plateau de jeu d'échecs

    quick_imgui::loop(
        "Chess",  // Nom de la fenêtre
        [&]() {   // Fonction d'initialisation (optionnelle ici, mais peut être utile pour une préparation)
            // Initialisation, si nécessaire
        },
        [&]() {   // Fonction de boucle (pour chaque frame)
            ImGui::ShowDemoWindow(); // Fenêtre de démo ImGui, facultatif, à enlever si non nécessaire

            ImGui::Begin("Chess Board");

            // Afficher le plateau d'échecs
            board.render();

            ImGui::End();
        }
    );

    return 0;
}

