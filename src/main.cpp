#include <imgui.h>
#include "board.hpp" // Inclure le fichier de la classe Board
#include "quick_imgui/quick_imgui.hpp"

int main()
{
    Board board; // Création de l'objet du plateau de jeu d'échecs

    quick_imgui::loop(
        "Chess",
        {
            .init = [&]() {},
            .loop =
                [&]() {
                    // ImGui::ShowDemoWindow();
                    ImGui::Begin("Chess 2D");
                    board.render();
                    ImGui::End();
                },
        }
    );
    return 0;
}
