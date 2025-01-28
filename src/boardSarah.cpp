#include "boardSarah.hpp"
#include <imgui.h>
#include <iostream>
#include "quick_imgui/quick_imgui.hpp"


void Board_sarah::draw_pion_line()
{
    for (int i{0}; i < 8; i++)
    {
        ImGui::Button("Pion", ImVec2{50.f, 50.f});
        ImGui::SameLine();
    }
}

void Board_sarah::draw_vide_line()
{
    for (int i{0}; i < 8; i++)
    {
        ImGui::Button("_", ImVec2{50.f, 50.f});
        ImGui::SameLine();
    }
}

void Board_sarah::draw_royal_line()
{
    ImGui::Button("tour", ImVec2{50.f, 50.f});
    ImGui::SameLine();
    ImGui::Button("cavalier", ImVec2{50.f, 50.f});
    ImGui::SameLine();
    ImGui::Button("fou", ImVec2{50.f, 50.f});
    ImGui::SameLine();
    ImGui::Button("reine", ImVec2{50.f, 50.f});
    ImGui::SameLine();
    ImGui::Button("roi", ImVec2{50.f, 50.f});
    ImGui::SameLine();
    ImGui::Button("fou", ImVec2{50.f, 50.f});
    ImGui::SameLine();
    ImGui::Button("cavalier", ImVec2{50.f, 50.f});
    ImGui::SameLine();
    ImGui::Button("tour", ImVec2{50.f, 50.f});
    ImGui::SameLine();
}

void Board_sarah::draw_board()
{
    Board_sarah::draw_royal_line();
    ImGui::NewLine();
    Board_sarah::draw_pion_line();
    ImGui::NewLine();
    Board_sarah::draw_vide_line();
    ImGui::NewLine();
    Board_sarah::draw_vide_line();
    ImGui::NewLine();
    Board_sarah::draw_vide_line();
    ImGui::NewLine();
    Board_sarah::draw_pion_line();
    ImGui::NewLine();
    Board_sarah::draw_royal_line();
    ImGui::NewLine();
}

void Board_sarah::draw_table()
{
    const int columns = 8;
    const int rows    = 8;

    if (ImGui::BeginTable("ChessBoard", columns, ImGuiTableFlags_Borders))
    {
        for (int row = 0; row < rows; ++row)
        {
            ImGui::TableNextRow(); // Ajoute une nouvelle rangée
            for (int column = 0; column < columns; ++column)
            {
                ImGui::TableSetColumnIndex(column); // Positionne sur la bonne colonne

                // Alterne les couleurs des cases
                bool   is_black     = (row + column) % 2 == 0;
                ImVec4 color        = is_black ? ImVec4(0.2f, 0.2f, 0.2f, 1.0f) : ImVec4(0.8f, 0.8f, 0.8f, 1.0f);
                ImVec4 color_active = ImVec4(0.1f, 0.2f, 0.2f, 1.0f);
                ImGui::PushStyleColor(ImGuiCol_Button, color);
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, color);
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, color_active);

                ImGui::Button("##id", ImVec2{50.f, 50.f});

                ImGui::PopStyleColor(3);
            }
        }
        ImGui::EndTable();
    }
}
