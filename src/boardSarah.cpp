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

#include "boardSarah.hpp"
#include <imgui.h>
#include <iostream>
#include "quick_imgui/quick_imgui.hpp"
#include "piece.hpp"

// Fonction pour récupérer les pièces vivantes
// std::vector<Piece> alive_pieces(const std::vector<Piece>& pieces) {
//     std::vector<Piece> alive_pieces;
//     for (const auto& piece : pieces) {
//         if (piece.get_state() == Piece::State::Alive) {
//             alive_pieces.push_back(piece);
//         }
//     }
//     return alive_pieces;
// }

// Méthode pour dessiner le tableau de jeu
void Board_sarah::draw_table(Board_sarah& board) {
    const int columns = 8;
    const int rows = 8;

    static Piece* selected_piece = nullptr;

    // Initialisation des pièces si ce n'est pas déjà fait
    if (board.pieces.empty()) {
        for (int i = 0; i < 8; i++) {
            board.pieces.push_back(Piece("Pawn", "White", {1, i}));  // Pions blancs
            board.pieces.push_back(Piece("Pawn", "Black", {6, i}));  // Pions noirs
        }
    }

    // Démarre le tableau ImGui
    if (ImGui::BeginTable("ChessBoard", columns, ImGuiTableFlags_Borders)) {
        for (int row = 0; row < rows; ++row) {
            ImGui::TableNextRow();  // Crée une nouvelle ligne

            for (int column = 0; column < columns; ++column) {
                ImGui::TableSetColumnIndex(column);  // Positionne sur la bonne colonne

                // Alterne les couleurs des cases
                bool is_black = (row + column) % 2 == 0;
                ImVec4 color = is_black ? ImVec4(0.2f, 0.2f, 0.2f, 1.0f) : ImVec4(0.8f, 0.8f, 0.8f, 1.0f);
                ImVec4 color_active = ImVec4(0.1f, 0.2f, 0.2f, 1.0f);

                ImGui::PushStyleColor(ImGuiCol_Button, color);
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, color);
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, color_active);

                // Vérifie si une pièce se trouve à cette position
                bool piece_found = false;
                for (auto& piece : board.pieces) {
                    if (piece.get_position().first == row && piece.get_position().second == column) {
                        // Affiche la pièce sous forme de texte (ou d'image si nécessaire)
                        if (ImGui::Button(piece.get_type().c_str(), ImVec2{50.f, 50.f})) {
                            if (selected_piece == nullptr) {
                                // Sélectionne la pièce si aucune autre n'est sélectionnée
                                selected_piece = &piece;
                            } else {
                                // Si une pièce est sélectionnée, essaye de déplacer cette pièce
                                auto possible_moves = selected_piece->possible_moves();
                                if (std::find(possible_moves.begin(), possible_moves.end(), std::make_pair(row, column)) != possible_moves.end()) {
                                    // Déplace la pièce si le mouvement est valide
                                    selected_piece->move({row, column});
                                }
                                selected_piece = nullptr; // Désélectionne la pièce après le mouvement
                            }
                        }
                        piece_found = true;
                        break;
                    }
                }

                // Si aucune pièce n'est présente, afficher un bouton vide
                if (!piece_found) {
                    ImGui::Button("##id", ImVec2{50.f, 50.f});
                }

                ImGui::PopStyleColor(3);  // Restaure les couleurs d'origine
            }
        }
        ImGui::EndTable();
    }
}

