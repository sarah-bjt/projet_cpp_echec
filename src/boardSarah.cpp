#include "boardSarah.hpp"
#include <imgui.h>
#include <iostream>
#include "piece.hpp"
#include "quick_imgui/quick_imgui.hpp"


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

std::vector<Piece> alive_pieces(const std::vector<Piece>& pieces)
{
    std::vector<Piece> alive_pieces;

    for (const auto& piece : pieces)
    {
        if (piece.get_state() == Piece::State::Alive)
        {
            alive_pieces.push_back(piece);
        }
    }

    return alive_pieces;
}

void Board_sarah::draw_table(Board_sarah& board)
{
    const int columns = 8;
    const int rows    = 8;

    static Piece* selected_piece = nullptr;

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

                // Gérer les pièces : afficher la pièce à sa position
                bool piece_found = false;
                for (const auto& piece : board.pieces)
                {
                    if (piece.get_position().first == row && piece.get_position().second == column)
                    {
                        // Afficher la pièce sous forme de texte ou d'image
                        ImGui::Button(piece.get_type().c_str(), ImVec2{50.f, 50.f});
                        // Sélectionner la pièce
                        if (selected_piece == nullptr)
                        {
                            selected_piece = &piece;
                        }
                        else
                        {
                            if (std::find(piece.possible_moves().begin(), piece.possible_moves().end(), std::make_pair(row, column)) != piece.possible_moves().end())
                            {
                                piece.move({row, column});
                            }
                            selected_piece = nullptr; // Désélectionner la pièce
                        }
                        piece_found = true;
                        break;
                    }
                }

                // Si aucune pièce n'est présente, afficher un bouton vide
                if (!piece_found)
                {
                    ImGui::Button("##id", ImVec2{50.f, 50.f});
                }

                ImGui::PopStyleColor(3);
            }
        }
        ImGui::EndTable();
    }
}

Board_sarah::Board_sarah()
{
    board.resize(8, std::vector<Piece*>(8, nullptr));

    // Placer les pièces blanches
    board[0][0] = new Piece("Rook", "White", {0, 0});
    board[0][1] = new Piece("Knight", "White", {0, 1});
    board[0][2] = new Piece("Bishop", "White", {0, 2});
    board[0][3] = new Piece("Queen", "White", {0, 3});
    board[0][4] = new Piece("King", "White", {0, 4});
    board[0][5] = new Piece("Bishop", "White", {0, 5});
    board[0][6] = new Piece("Knight", "White", {0, 6});
    board[0][7] = new Piece("Rook", "White", {0, 7});
    for (int i = 0; i < 8; i++)
    {
        board[1][i] = new Piece("Pawn", "White", {1, i});
    }

    // Placer les pièces noires
    board[7][0] = new Piece("Rook", "Black", {7, 0});
    board[7][1] = new Piece("Knight", "Black", {7, 1});
    board[7][2] = new Piece("Bishop", "Black", {7, 2});
    board[7][3] = new Piece("Queen", "Black", {7, 3});
    board[7][4] = new Piece("King", "Black", {7, 4});
    board[7][5] = new Piece("Bishop", "Black", {7, 5});
    board[7][6] = new Piece("Knight", "Black", {7, 6});
    board[7][7] = new Piece("Rook", "Black", {7, 7});
    for (int i = 0; i < 8; i++)
    {
        board[6][i] = new Piece("Pawn", "Black", {6, i});
    }
}

// ceci est un test pout git
