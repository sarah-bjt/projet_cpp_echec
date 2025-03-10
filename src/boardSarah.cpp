#include "boardSarah.hpp"
#include <imgui.h>
#include <iostream>
#include "quick_imgui/quick_imgui.hpp"
#include "piece.hpp"

// void Board_sarah::draw_table()
// {
//     const int columns = 8;
//     const int rows    = 8;

//     if (ImGui::BeginTable("ChessBoard", columns, ImGuiTableFlags_Borders))
//     {
//         for (int row = 0; row < rows; ++row)
//         {
//             ImGui::TableNextRow(); // Ajoute une nouvelle rangée
//             for (int column = 0; column < columns; ++column)
//             {
//                 ImGui::TableSetColumnIndex(column); // Positionne sur la bonne colonne

//                 // Alterne les couleurs des cases
//                 bool   is_black     = (row + column) % 2 == 0;
//                 ImVec4 color        = is_black ? ImVec4(0.2f, 0.2f, 0.2f, 1.0f) : ImVec4(0.8f, 0.8f, 0.8f, 1.0f);
//                 ImVec4 color_active = ImVec4(0.1f, 0.2f, 0.2f, 1.0f);
//                 ImGui::PushStyleColor(ImGuiCol_Button, color);
//                 ImGui::PushStyleColor(ImGuiCol_ButtonHovered, color);
//                 ImGui::PushStyleColor(ImGuiCol_ButtonActive, color_active);

//                 ImGui::Button("##id", ImVec2{50.f, 50.f});

//                 ImGui::PopStyleColor(3);
//             }
//         }
//         ImGui::EndTable();
//     }
// }

std::vector<Piece> alive_pieces(const std::vector<Piece>& pieces) {
    std::vector<Piece> alive_pieces;

    for (const auto& piece : pieces) {
        if (piece.get_state() == Piece::State::Alive) {
            alive_pieces.push_back(piece);
        }
    }

    return alive_pieces;
}



void Board_sarah::draw_table() {
    const int columns = 8;
    const int rows    = 8;

    if (ImGui::BeginTable("ChessBoard", columns, ImGuiTableFlags_Borders))
    {
        for (int row = 0; row < rows; ++row) {
            ImGui::TableNextRow(); // Ajoute une nouvelle rangée
            for (int column = 0; column < columns; ++column) {
                ImGui::TableSetColumnIndex(column); // Positionne sur la bonne colonne

                // Alterne les couleurs des cases
                bool is_black = (row + column) % 2 == 0;
                ImVec4 color = is_black ? ImVec4(0.2f, 0.2f, 0.2f, 1.0f) : ImVec4(0.8f, 0.8f, 0.8f, 1.0f);
                ImVec4 color_active = ImVec4(0.1f, 0.2f, 0.2f, 1.0f);

                ImGui::PushStyleColor(ImGuiCol_Button, color);
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, color);
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, color_active);

                // Gérer les pièces : afficher la pièce à sa position
                bool piece_found = false;
                for (const auto& piece : pieces) {
                    if (piece.get_position().first == row && piece.get_position().second == column) {
                        // Afficher la pièce sous forme de texte ou d'image
                        ImGui::Button(piece.get_type().c_str(), ImVec2{50.f, 50.f});
                        piece_found = true;
                        break;
                    }
                }

                // Si aucune pièce n'est présente, afficher un bouton vide
                if (!piece_found) {
                    ImGui::Button("##id", ImVec2{50.f, 50.f});
                }

                ImGui::PopStyleColor(3);
            }
        }
        ImGui::EndTable();
    }
}
