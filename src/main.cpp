#include <imgui.h>
#include "board.hpp" // Inclure le fichier de la classe Board
#include "quick_imgui/quick_imgui.hpp"
#include "glad/glad.h"

int main()
{
    Board board;

    quick_imgui::loop(
        "Chess",
        {
            .init = [&]() {board.init();},
            .loop =
                [&]() {
                    // ImGui::ShowDemoWindow();
                    ImGui::Begin("Chess 2D");
                    board.render();
                    ImGui::End();
                },
                .key_callback             = [](int key, int scancode, int action, int mods) { std::cout << "Key: " << key << " Scancode: " << scancode << " Action: " << action << " Mods: " << mods << '\n'; },
                .mouse_button_callback    = [](int button, int action, int mods) { std::cout << "Button: " << button << " Action: " << action << " Mods: " << mods << '\n'; },
                .cursor_position_callback = [](double xpos, double ypos) { std::cout << "Position: " << xpos << ' ' << ypos << '\n'; },
                .scroll_callback          = [](double xoffset, double yoffset) { std::cout << "Scroll: " << xoffset << ' ' << yoffset << '\n'; },
                .window_size_callback     = [](int width, int height) { std::cout << "Resized: " << width << ' ' << height << '\n'; },
        }
    );
    return 0;
}
