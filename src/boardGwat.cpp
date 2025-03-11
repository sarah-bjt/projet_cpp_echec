#include "boardGwat.hpp"
#include <imgui.h>
#include <algorithm>
#include <iostream>
#include <vector>
#include "piece.hpp"

// Surcharge de l'opérateur + pour ImVec2
ImVec2 operator+(const ImVec2& lhs, const ImVec2& rhs)
{
    return ImVec2(lhs.x + rhs.x, lhs.y + rhs.y);
}

// Constructeur pour initialiser un plateau vide et placer les pièces
Board::Board()
{
    grid.resize(8, std::vector<Piece*>(8, nullptr));

    // Placer les pièces blanches (en bas sur la ligne 0)
    placePiece(new Piece("Rook", "White", {0, 0}), 0, 0);
    placePiece(new Piece("Rook", "White", {7, 0}), 7, 0);
    placePiece(new Piece("Knight", "White", {1, 0}), 1, 0);
    placePiece(new Piece("Knight", "White", {6, 0}), 6, 0);
    placePiece(new Piece("Bishop", "White", {2, 0}), 2, 0);
    placePiece(new Piece("Bishop", "White", {5, 0}), 5, 0);
    placePiece(new Piece("Queen", "White", {3, 0}), 3, 0);
    placePiece(new Piece("King", "White", {4, 0}), 4, 0);

    // Placer les pions blancs (en ligne 1)
    for (int x = 0; x < 8; ++x)
    {
        placePiece(new Piece("Pawn", "White", {x, 1}), x, 1);
    }

    // Placer les pièces noires (en haut sur la ligne 7)
    placePiece(new Piece("Rook", "Black", {0, 7}), 0, 7);
    placePiece(new Piece("Rook", "Black", {7, 7}), 7, 7);
    placePiece(new Piece("Knight", "Black", {1, 7}), 1, 7);
    placePiece(new Piece("Knight", "Black", {6, 7}), 6, 7);
    placePiece(new Piece("Bishop", "Black", {2, 7}), 2, 7);
    placePiece(new Piece("Bishop", "Black", {5, 7}), 5, 7);
    placePiece(new Piece("Queen", "Black", {3, 7}), 3, 7);
    placePiece(new Piece("King", "Black", {4, 7}), 4, 7);

    // Placer les pions noirs (en ligne 6)
    for (int x = 0; x < 8; ++x)
    {
        placePiece(new Piece("Pawn", "Black", {x, 6}), x, 6);
    }
}

// Destructeur pour libérer la mémoire des pièces
Board::~Board()
{
    for (auto& row : grid)
    {
        for (auto& piece : row)
        {
            delete piece;
        }
    }
}

// Placer une pièce sur une case donnée
void Board::placePiece(Piece* piece, int x, int y)
{
    grid[x][y] = piece;
}

// Afficher le plateau
void Board::render()
{
    for (int y = 7; y >= 0; --y)
    { // Inverser l'ordre des lignes : commence par la ligne 7
        for (int x = 0; x < 8; ++x)
        {
            ImVec2 pos  = board_pos + ImVec2(x * square_size, (7 - y) * square_size); // Inverser y pour que (0,0) soit en bas à gauche
            ImVec2 size = ImVec2(square_size, square_size);

            // Dessiner les cases
            ImU32 color = (x + y) % 2 == 0 ? IM_COL32(240, 217, 181, 255) : IM_COL32(119, 148, 85, 255);
            ImGui::GetWindowDrawList()->AddRectFilled(pos, ImVec2(pos.x + square_size, pos.y + square_size), color);

            // Dessiner la pièce si elle existe
            Piece* piece = grid[x][y];
            if (piece != nullptr)
            {
                std::string piece_str = piece->get_type();

                // Vérifier si la souris survole la case
                if (ImGui::IsMouseHoveringRect(pos, ImVec2(pos.x + square_size, pos.y + square_size)))
                {
                    ImGui::GetWindowDrawList()->AddRect(pos, ImVec2(pos.x + square_size, pos.y + square_size), IM_COL32(255, 0, 0, 255), 0.0f, ImDrawFlags_None, 2.0f);

                    if (ImGui::IsMouseClicked(0))
                    {
                        selected_piece = true;
                        selected_pos   = {x, y};

                        // Afficher le message indiquant la case où l'on a cliqué
                        std::string msg = "Vous avez cliqué sur la case: (" + std::to_string(x) + ", " + std::to_string(y) + ")";
                        std::cout << msg << std::endl;
                    }
                }

                // Afficher la pièce
                ImGui::GetWindowDrawList()->AddText(ImVec2(pos.x + square_size / 4, pos.y + square_size / 4), IM_COL32(0, 0, 0, 255), piece_str.c_str());
            }

            // Afficher les mouvements possibles de la pièce sélectionnée
            if (selected_piece && piece != nullptr && selected_pos == std::make_pair(x, y))
            {
                std::vector<std::pair<int, int>> moves = piece->possible_moves();

                for (const auto& move : moves)
                {
                    // Calculer la position du cercle de mouvement (en tenant compte de l'inversion de y)
                    ImVec2 move_pos = board_pos + ImVec2(move.first * square_size, (7 - move.second) * square_size);

                    // Dessiner un cercle vert à la position du mouvement possible
                    ImGui::GetWindowDrawList()->AddCircleFilled(ImVec2(move_pos.x + square_size / 2, move_pos.y + square_size / 2), 10.0f, IM_COL32(0, 255, 0, 255));

                    // Gérer le clic pour déplacer la pièce
                    if (ImGui::IsMouseHoveringRect(move_pos, ImVec2(move_pos.x + square_size, move_pos.y + square_size)) && ImGui::IsMouseClicked(0))
                    {
                        movePiece(selected_pos, move);
                        selected_piece = false; // Désélectionner après déplacement
                    }
                }
            }
        }
    }

    // Afficher un message en bas indiquant la case sélectionnée
    if (selected_piece)
    {
        std::string msg = "Case sélectionnée : (" + std::to_string(selected_pos.first) + ", " + std::to_string(selected_pos.second) + ")";
        ImGui::Text("%s", msg.c_str());
    }
}

// Déplacer une pièce d'une position à une autre
bool Board::movePiece(const std::pair<int, int>& from, const std::pair<int, int>& to)
{
    Piece* piece = grid[from.first][from.second];
    if (piece == nullptr)
        return false;

    piece->move(to); // Appel de la méthode move de la classe Piece
    grid[to.first][to.second]     = piece;
    grid[from.first][from.second] = nullptr;
    return true;
}

// Informe si une pièce est présente sur une case donné
bool Board::is_piece_at(const std::pair<int, int>& pos)
{
    return grid[pos.first][pos.second] != nullptr;
}
